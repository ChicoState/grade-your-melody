#include "GridController.h"
#include <QVariantList>
#include <QDebug>
#include <QString>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
GridController::GridController(QObject *parent)
    : QObject(parent)
{
    // Default: expect empty on every beat
    expectedRow.fill(-1);

    expectedAccidental.fill(0);
    for (auto& rowArr : userAccidental) rowArr.fill(0);
    userLength.fill(0);
    expectedLength.fill(0);
    // Example answer (change these to your melody):
    // expectedRow[0] = 4; expectedRow[1] = 4; expectedRow[2] = 5; ...

    // Forward AudioEngine beat notifications to QML via playbackBeatChanged
    connect(&m_audioEngine, &AudioEngine::beatAdvanced, this, [this](int beat) {
        qDebug() << "[GridController] beatAdvanced received:" << beat
                 << "→ m_currentPlaybackBeat was" << m_currentPlaybackBeat;
        if (m_currentPlaybackBeat == beat) return;
        m_currentPlaybackBeat = beat;
        emit playbackBeatChanged();
    });
}

//Getter for question text
QString GridController::currentQuestionText() const {
    return m_currentQuestionText;
}

QString GridController::currentChoiceA() const       { return m_currentChoiceA; }
QString GridController::currentChoiceB() const       { return m_currentChoiceB; }
QString GridController::currentCorrectChoice() const { return m_currentCorrectChoice; }

//Helper: is note length allowed
bool GridController::isLengthAllowed(int length) const {
    if (m_allowedLengths.empty()) return true;

    for (int allowed : m_allowedLengths) {
        if (allowed == length) return true;
    }
    return false;
}

//Helper: is beat allowed
bool GridController::isStartColumnAllowed(int beat) const {
    if (m_allowedStartColumns.empty()) return true;

    for (int allowed : m_allowedStartColumns) {
        if (allowed == beat) return true;
    }
    return false;
}

//Helper: can grade yet
bool GridController::canGrade() const {
    if (!m_requireAllFilled) return true;

    for (int beat : m_allowedStartColumns) {
        if (noteLengthForBeat(beat) <= 0) {
            return false;
        }
    }

    return true;
}

bool GridController::hasNote(int beat, int row) const {
    return userGrid.HasNote(beat, row);
}

void GridController::clearBeatInternal(int beat) {
    for (int r = 0; r < StaffLineGrid::rows; ++r) {
        userGrid.RemoveNote(beat, r);
    }
}

// Removes any existing note whose occupied range overlaps [newBeat, newBeat+newLength-1].
// Why: prevents a longer/shorter note from leaving stale fragments behind when the user
// places something on top of it (e.g., placing a half over an existing quarter).
// Chord-preservation rule: notes with the same start AND same length as the new note
// are kept so users can stack multiple rows at the same beat.
void GridController::clearOverlappingNotes(int newBeat, int newLength) {
    if (newLength <= 0) return;

    // Pass 1 — collect (start, row) pairs to remove
    // Use small flat arrays since the grid is bounded by columns x rows.
    bool toRemove[StaffLineGrid::columns][StaffLineGrid::rows] = {};
    bool startTouched[StaffLineGrid::columns] = {};

    for (int col = newBeat; col < newBeat + newLength && col < StaffLineGrid::columns; ++col) {
        for (int r = 0; r < StaffLineGrid::rows; ++r) {
            if (!userGrid.HasNote(col, r)) continue;

            // Walk back to find the existing note's start beat
            int start = col;
            while (start > 0 && userGrid.HasNote(start - 1, r))
                --start;

            int existingLen = userLength[start];
            if (existingLen <= 0) existingLen = 1;

            // Chord member that exactly matches the new note's footprint — keep it
            if (start == newBeat && existingLen == newLength) continue;

            toRemove[start][r] = true;
            startTouched[start] = true;
        }
    }

    // Pass 2 — remove marked notes; remember which beats need a UI refresh
    bool refreshBeat[StaffLineGrid::columns] = {};
    for (int s = 0; s < StaffLineGrid::columns; ++s) {
        if (!startTouched[s]) continue;
        for (int r = 0; r < StaffLineGrid::rows; ++r) {
            if (!toRemove[s][r]) continue;
            int len = userLength[s];
            if (len <= 0) len = 1;
            userGrid.RemoveNote(s, r);
            userAccidental[s][r] = 0;
            for (int k = s; k < s + len && k < StaffLineGrid::columns; ++k)
                refreshBeat[k] = true;
        }
    }

    // Pass 3 — for each touched start, zero userLength only if no chord notes remain
    for (int s = 0; s < StaffLineGrid::columns; ++s) {
        if (!startTouched[s]) continue;
        bool anyRemains = false;
        for (int r = 0; r < StaffLineGrid::rows; ++r) {
            if (userGrid.HasNote(s, r)) { anyRemains = true; break; }
        }
        if (!anyRemains) userLength[s] = 0;
    }

    for (int b = 0; b < StaffLineGrid::columns; ++b)
        if (refreshBeat[b]) emit beatChanged(b);
}

void GridController::clearBeat(int beat) {
    if (beat < 0 || beat >= StaffLineGrid::columns) return;
    clearBeatInternal(beat);
    userAccidental[beat].fill(0);
    userLength[beat] = 0;
    emit beatChanged(beat);
}

void GridController::setNote(int beat, int row, int acc, int length) {
    if (!userGrid.ValidPosition(beat, row)) return;
    if (beat < 0 || beat >= StaffLineGrid::columns) return;
    if (length <= 0) return;
    if (beat + length > StaffLineGrid::columns) return;

    // Question-based restrictions (apply to both paths) — bypassed in Free Staff mode
    if (!m_freeStaffMode) {
        if (!isLengthAllowed(length)) return;
        if (!isStartColumnAllowed(beat)) return;
    }

    // ── Stacking path ────────────────────────────────────────────────────────
    // Only touch the target (beat, row). All other rows at this beat survive.
    // Free Staff mode always allows stacking, regardless of the current question's
    // m_allowStacking value (which belongs to the CSV question and must stay intact).
    if (m_allowStacking || m_freeStaffMode) {
        // Rhythm-collision: clear any existing note whose duration overlaps the new
        // note's range, keeping same-start/same-length chord members.
        clearOverlappingNotes(beat, length);

        userGrid.RemoveNote(beat, row);       // no-op if the cell was empty
        userAccidental[beat][row] = 0;

        userGrid.AddNote(beat, row, length);
        userAccidental[beat][row] = acc;
        userLength[beat] = length;            // all chord notes share one duration

        for (int i = 0; i < length; ++i)
            emit beatChanged(beat + i);
        m_audioEngine.playPreview(row, acc);
        return;
    }

    // ── Non-stacking path (original behaviour) ────────────────────────────────
    // Abort if any note already exists anywhere on this beat
    for (int r = 0; r < StaffLineGrid::rows; ++r) {
        if (userGrid.HasNote(beat, r)) return;
    }

    const int measureStart = (beat / 8) * 8;
    const int measureOffset = beat % 8;

    // Prevent crossing the current measure boundary
    if (measureOffset + length > 8) return;

    // Enforce notation-based start positions for 4/4 with 8 columns per measure.
    // length uses the UI type-ID convention: 4=whole, 3=half, 2=quarter, 1=eighth.
    switch (length) {
        case 4: if (measureOffset != 0) return; break;                       // whole: beat 1 only
        case 3: if (measureOffset != 0 && measureOffset != 4) return; break; // half: beats 1 or 3
        case 2: if (measureOffset % 2 != 0) return; break;                   // quarter: beats 1-4
        case 1: break;                                                         // eighth: anywhere
        default: return;
    }

    const int measureEnd = measureStart + 8;

    bool beatsToRefresh[8] = { false };

    bool occupied[8] = { false };
    for (int c = measureStart; c < measureEnd; ++c) {
        if (noteRowForBeat(c) != -1) {
            occupied[c - measureStart] = true;
        }
    }

    for (int c = beat; c < beat + length; ++c) {
        for (int r = 0; r < StaffLineGrid::rows; ++r) {
            if (!userGrid.HasNote(c, r)) continue;

            int noteStart = c;
            while (noteStart > measureStart && userGrid.HasNote(noteStart - 1, r))
                --noteStart;

            int existingLength = userLength[noteStart];
            if (existingLength <= 0) existingLength = 1;

            for (int k = noteStart; k < noteStart + existingLength && k < measureEnd; ++k) {
                occupied[k - measureStart] = false;
                beatsToRefresh[k - measureStart] = true;
            }
        }
    }

    int occupiedAfterClear = 0;
    for (int i = 0; i < 8; ++i)
        if (occupied[i]) ++occupiedAfterClear;

    if (occupiedAfterClear + length > 8) return;

    // Rhythm-collision: catches notes that started before `beat` but extend into
    // the new note's range — the local loop below only handles columns within
    // [beat, beat+length).
    clearOverlappingNotes(beat, length);

    for (int i = 0; i < length; ++i) {
        clearBeatInternal(beat + i);
        userAccidental[beat + i].fill(0);
        userLength[beat + i] = 0;
        beatsToRefresh[beat + i - measureStart] = true;
    }

    userGrid.AddNote(beat, row, length);
    userAccidental[beat][row] = acc;
    userLength[beat] = length;

    for (int i = 0; i < length; ++i)
        beatsToRefresh[beat + i - measureStart] = true;

    for (int i = 0; i < 8; ++i)
        if (beatsToRefresh[i]) emit beatChanged(measureStart + i);

    m_audioEngine.playPreview(row, acc);
}

int GridController::accidentalForBeat(int beat) const {
    // Returns the accidental of the lowest placed note at this beat.
    // Use accidentalForBeatRow() when per-row precision is needed.
    int row = noteRowForBeat(beat);
    if (row < 0 || beat < 0 || beat >= StaffLineGrid::columns) return 0;
    return userAccidental[beat][row];
}

int GridController::accidentalForBeatRow(int beat, int row) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return 0;
    if (row < 0 || row >= StaffLineGrid::rows) return 0;
    return userAccidental[beat][row];
}

int GridController::noteLengthForBeat(int beat) const {
    if (beat < 0 || beat >= (int)userLength.size()) return 0;
    return userLength[beat];
}

int GridController::noteRowForBeat(int beat) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return -1;

    for (int r = 0; r < StaffLineGrid::rows; ++r) {
        if (userGrid.HasNote(beat, r)) return r;
    }
    return -1;
}

void GridController::setExpectedRow(int beat, int row, int acc, int length) {
    if (beat < 0 || beat >= StaffLineGrid::columns) return;
    if (row < -1 || row >= StaffLineGrid::rows) return;

    expectedRow[beat] = row;
    expectedAccidental[beat] = acc;
    expectedLength[beat] = length;
    emit expectedChanged(beat);
}

int GridController::expectedRowForBeat(int beat) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return -1;
    return expectedRow[beat];
}

int GridController::expectedLengthForBeat(int beat) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return 0;
    return expectedLength[beat];
}

int GridController::expectedAccForBeat(int beat) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return 0;
    return expectedAccidental[beat];
}

// Per-row variant. Required for chord answers where two notes at the same beat
// can have different accidentals (e.g. C minor chord = C natural, Eb, G natural).
// expectedAccForBeat() can't represent that since it only stores one value per beat.
int GridController::expectedAccForBeatRow(int beat, int row) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return 0;
    if (row  < 0 || row  >= StaffLineGrid::rows)    return 0;

    if (!m_allowStacking) {
        // Non-stacking: per-beat accidental applies only when this row matches the expected row
        return (expectedRow[beat] == row) ? expectedAccidental[beat] : 0;
    }

    // Stacking: each chord note carries its own accidental in m_expectedNotes
    for (const NoteInfo& note : m_expectedNotes) {
        if (note.beat == beat && note.row == row) return note.accent;
    }
    return 0;
}
bool GridController::hasExpectedNote(int beat, int row) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return false;
    if (row < 0 || row >= StaffLineGrid::rows) return false;

    if (!m_allowStacking) {
        // Non-stacking: check flat array
        return expectedRow[beat] == row;
    }

    // Stacking: search m_expectedNotes
    for (const NoteInfo& note : m_expectedNotes) {
        if (note.beat == beat && note.row == row) return true;
    }
    return false;
}

int GridController::expectedNoteLengthAt(int beat, int row) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return 0;
    if (row < 0 || row >= StaffLineGrid::rows) return 0;

    // Check if this cell has an expected note
    if (!hasExpectedNote(beat, row)) return 0;

    // Return the expected length at this beat (shared by all notes on the beat)
    return expectedLength[beat];
}
bool GridController::isNoteIncorrect(int beat, int row) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return false;
    if (row < 0 || row >= StaffLineGrid::rows) return false;

    if (!m_allowStacking) {
        // Non-stacking: a placed note is wrong iff the whole beat is wrong
        return !isBeatCorrect(beat);
    }

    // Stacking: evaluate only this specific (beat, row) note
    if (!userGrid.HasNote(beat, row)) return false; // nothing placed here

    for (const NoteInfo& exp : m_expectedNotes) {
        if (exp.beat == beat && exp.row == row)
            return userAccidental[beat][row] != exp.accent; // expected but wrong accidental?
    }
    return true; // placed but not expected → wrong
}

bool GridController::isBeatCorrect(int beat) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return false;

    if (!m_allowStacking) {
        // Non-stacking: one expected row per beat
        int userRow = noteRowForBeat(beat);
        if (userRow != expectedRow[beat]) return false;
        if (expectedRow[beat] == -1) return true; // both empty
        return userAccidental[beat][userRow] == expectedAccidental[beat];
    }

    // Stacking: every expected note at this beat must be present with the
    // correct accidental, and no unexpected notes may exist.
    bool anyExpected = false;
    for (const NoteInfo& exp : m_expectedNotes) {
        if (exp.beat != beat) continue;
        anyExpected = true;
        if (!userGrid.HasNote(beat, exp.row)) return false;
        if (userAccidental[beat][exp.row] != exp.accent) return false;
    }
    if (!anyExpected) return true;

    for (int r = 0; r < StaffLineGrid::rows; ++r) {
        if (!userGrid.HasNote(beat, r)) continue;
        bool isExpected = false;
        for (const NoteInfo& exp : m_expectedNotes)
            if (exp.beat == beat && exp.row == r) { isExpected = true; break; }
        if (!isExpected) return false;
    }
    return true;
}


QVariantList GridController::incorrectBeats() const {
    QVariantList wrong;
    if (!m_allowStacking) {
        for (int b = 0; b < StaffLineGrid::columns; ++b) {
            if (expectedRow[b] == -1) continue;
            if (!isBeatCorrect(b)) wrong.append(b);
        }
        return wrong;
    }
    // Stacking: collect distinct beats that have expected notes and are wrong
    bool hasBeat[StaffLineGrid::columns] = {};
    for (const NoteInfo& note : m_expectedNotes)
        if (note.beat >= 0 && note.beat < StaffLineGrid::columns)
            hasBeat[note.beat] = true;
    for (int b = 0; b < StaffLineGrid::columns; ++b)
        if (hasBeat[b] && !isBeatCorrect(b)) wrong.append(b);
    return wrong;
}

int GridController::score() const {
    if (!m_allowStacking) {
        int s = 0;
        for (int b = 0; b < StaffLineGrid::columns; ++b) {
            if (expectedRow[b] == -1) continue;
            if (isBeatCorrect(b)) ++s;
        }
        return s;
    }
    // Stacking: one point per beat where every expected chord note is correct
    bool hasBeat[StaffLineGrid::columns] = {};
    for (const NoteInfo& note : m_expectedNotes)
        if (note.beat >= 0 && note.beat < StaffLineGrid::columns)
            hasBeat[note.beat] = true;
    int s = 0;
    for (int b = 0; b < StaffLineGrid::columns; ++b)
        if (hasBeat[b] && isBeatCorrect(b)) ++s;
    return s;
}

int GridController::totalExpected() const {
    if (!m_allowStacking) {
        int t = 0;
        for (int b = 0; b < StaffLineGrid::columns; ++b)
            if (expectedRow[b] != -1) ++t;
        return t;
    }
    // Stacking: one expected "slot" per distinct beat that has expected notes
    bool hasBeat[StaffLineGrid::columns] = {};
    for (const NoteInfo& note : m_expectedNotes)
        if (note.beat >= 0 && note.beat < StaffLineGrid::columns)
            hasBeat[note.beat] = true;
    int t = 0;
    for (int b = 0; b < StaffLineGrid::columns; ++b)
        if (hasBeat[b]) ++t;
    return t;
}

void GridController::runMillion() {
    
     int melodyCount = 10000000; // do NOT try above 10,000,000 unless you hate your WSL instance
     
     int beatsPerMelody = StaffLineGrid::columns;
    
     int rows = StaffLineGrid::rows;
    

    int randomRow = rand() % rows; //random place in the melody row
    int randomAcc = (rand() % 3) - 1; // -1, 0, 1


    int perfectMatches = 0;
    long long totalCorrectBeats = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int m = 0; m < melodyCount; ++m) {
        int correctBeats = 0;
        for (int b = 0; b < beatsPerMelody; ++b) {
            int randomRow = rand() % rows;
            int randomAcc = (rand() % 3) -1;

            if (randomRow == expectedRow[b] &&
                randomAcc == expectedAccidental[b]) {
                ++correctBeats; }}

        totalCorrectBeats += correctBeats;

        if (correctBeats == beatsPerMelody) {
            ++perfectMatches;
        }
    }
    //clock mat
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    double melodiesPerSecond = (melodyCount * 1000.0) / ms;
    
    QString result =
    "melodies: " + QString::number(melodyCount) + "\n" +
    "time: " + QString::number(ms) + " ms\n" +
    "melodies per second (MPS): " + QString::number(melodiesPerSecond, 'f', 0) +
    
    " perfect melodies:" + QString::number(perfectMatches) + "\n" +
    
    "amount of beats correct:" + QString::number(totalCorrectBeats);

    emit benchmarkFinished(result);    
}


// ── Audio preparation helpers ─────────────────────────────────────────────────

// Converts a diatonic row index and accidental to the nearest equal-tempered
// frequency in Hz, using C4 = 261.63 Hz as the reference pitch.
// Row 0 = C4; each group of 7 rows is one octave (white keys only).
// acc: -1 = flat, 0 = natural, +1 = sharp.
static double pitchToFrequency(int row, int acc) {
    static const int diatonicOffsets[] = {0, 2, 4, 5, 7, 9, 11}; // C D E F G A B
    const int octave      = row / 7;
    const int scaleDegree = row % 7;
    const int semitone    = octave * 12 + diatonicOffsets[scaleDegree] + acc;
    return 261.63 * std::pow(2.0, semitone / 12.0);
}

// Returns one entry per placed note start: {beat, row, acc, length, frequency}.
// Continuation columns are skipped (userLength[beat] == 0 for them).
// Supports both single notes (non-stacking) and chords (stacking).
QVariantList GridController::getCurrentNotes() const {
    QVariantList result;
    for (int beat = 0; beat < StaffLineGrid::columns; ++beat) {
        const int len = userLength[beat];
        if (len <= 0) continue; // empty beat or continuation column

        for (int row = 0; row < StaffLineGrid::rows; ++row) {
            if (!userGrid.HasNote(beat, row)) continue;

            const int acc = userAccidental[beat][row];
            QVariantMap note;
            note["beat"]      = beat;
            note["row"]       = row;
            note["acc"]       = acc;
            note["length"]    = len;
            note["frequency"] = pitchToFrequency(row, acc);
            result.append(note);
        }
    }
    return result;
}

int GridController::currentPlaybackBeat() const {
    return m_currentPlaybackBeat;
}

void GridController::stopPlayback() {
    m_audioEngine.stop();
    // Clear cursor immediately — pending beat timers are already invalidated by stop()
    if (m_currentPlaybackBeat != -1) {
        m_currentPlaybackBeat = -1;
        emit playbackBeatChanged();
    }
}

void GridController::clearStaff() {
    // Clear user-placed notes only. Expected-answer arrays are intentionally untouched.
    userGrid.ClearGrid();
    for (auto& rowArr : userAccidental) rowArr.fill(0);
    userLength.fill(0);
    for (int b = 0; b < StaffLineGrid::columns; ++b) emit beatChanged(b);
}

// Free Staff mode = a runtime gate, not a question reload. It bypasses CSV
// length/start-column rules and forces the stacking placement path on, while
// leaving m_allowStacking untouched so Normal Mode grading still respects the
// loaded question's CSV settings.
void GridController::setFreeStaffMode(bool enabled) {
    m_freeStaffMode = enabled;
}

int GridController::tempoBpm() const {
    return m_tempoBpm;
}

void GridController::setTempoBpm(int bpm) {
    const int clamped = qBound(40, bpm, 200);
    if (clamped == m_tempoBpm) return;
    m_tempoBpm = clamped;
    emit tempoChanged();
}

void GridController::decreaseTempo() { setTempoBpm(m_tempoBpm - 5); }
void GridController::increaseTempo() { setTempoBpm(m_tempoBpm + 5); }

void GridController::playCurrentNotes() {
    const QVariantList all = getCurrentNotes();
    if (all.isEmpty()) return;

    // Convert QVariantList → QList<QVariantMap> and play all beats in sequence
    QList<QVariantMap> notes;
    notes.reserve(all.size());
    for (const QVariant& v : all)
        notes.append(v.toMap());

    m_audioEngine.playSequence(notes, static_cast<double>(m_tempoBpm));
}

void GridController::playExpectedAnswer() {
    if (m_expectedNotes.empty()) return;

    // Stop any existing playback before starting the expected answer
    m_audioEngine.stop();

    QList<QVariantMap> notes;
    notes.reserve(static_cast<int>(m_expectedNotes.size()));
    for (const NoteInfo& note : m_expectedNotes) {
        QVariantMap m;
        m["beat"]   = note.beat;
        m["row"]    = note.row;
        m["acc"]    = note.accent;
        m["length"] = expectedNoteLengthAt(note.beat, note.row);
        notes.append(m);
    }

    m_audioEngine.playSequence(notes, static_cast<double>(m_tempoBpm));
}

int GridController::currentQuestionNum() const {
    return m_currentQuestionNum;
}

int GridController::totalQuestionsAvailable() {
    return (int)questionHandler.GetQuestions().size();
}

void GridController::nextQuestion() {
    int total = (int)questionHandler.GetQuestions().size();
    if (m_currentQuestionNum < total)
        loadQuestion(m_currentQuestionNum + 1);
}

void GridController::previousQuestion() {
    if (m_currentQuestionNum > 1)
        loadQuestion(m_currentQuestionNum - 1);
}

void GridController::loadQuestion(int questionNum) {
    int total = (int)questionHandler.GetQuestions().size();
    if (questionNum < 1 || questionNum > total) return;

    m_currentQuestionNum = questionNum;

    // Clear user-placed notes and per-beat state
    userGrid.ClearGrid();
    for (auto& rowArr : userAccidental) rowArr.fill(0);
    userLength.fill(0);

    // Reset expected answers to empty.
    // expectedLength must also be reset — otherwise stale lengths from the previous
    // question can leak into the new one and corrupt Show Answer for chord beats.
    expectedRow.fill(-1);
    expectedAccidental.fill(0);
    expectedLength.fill(0);
    m_expectedNotes.clear();

    Question q = questionHandler.GetQuestion(questionNum);
    //debug question logic
    //for (const NoteInfo& note : q.notes) 
    //{
     //   qDebug() << "Beat:" << note.beat
       //         << "Row:" << note.row
         //       << "Acc:" << note.accent;

    //}
    
    //when a question loads:the text updates the restrictions are stored QML can query them
    m_currentQuestionText = QString::fromStdString(q.questionText);
    m_allowedLengths = q.allowedLengths;
    m_allowedStartColumns = q.allowedStartColumns;
    m_allowStacking = q.allowStacking;
    m_requireAllFilled = q.requireAllFilled;

    // Ear Training answer choices
    m_currentChoiceA       = QString::fromStdString(q.choiceA);
    m_currentChoiceB       = QString::fromStdString(q.choiceB);
    m_currentCorrectChoice = QString::fromStdString(q.correctChoice);

    emit questionChanged();


    // Copy expected notes into both storage structures.
    // m_expectedNotes holds the full list (supports multiple per beat for chords).
    // expectedRow/Accidental flat arrays are populated for non-stacking questions
    // and used by the non-stacking grading path.
    int defaultExpectedLength = q.allowedLengths.empty() ? 1 : q.allowedLengths[0];
    for (const NoteInfo& note : q.notes) {
        if (note.beat >= 0 && note.beat < StaffLineGrid::columns
            && note.row >= 0 && note.row < StaffLineGrid::rows) {
            m_expectedNotes.push_back(note);
            expectedLength[note.beat] = defaultExpectedLength;
            if (!m_allowStacking) {
                expectedRow[note.beat] = note.row;
                expectedAccidental[note.beat] = note.accent;
            }
        }
    }
    // notify QML/UI that everything changed
    // if we do not do this everytiime we load a question the ui will knot know to look for a new question. definitions in .h
    for (int b = 0; b < StaffLineGrid::columns; ++b) {
        emit expectedChanged(b); 
        emit beatChanged(b);
    }
}
