#include "GridController.h"
#include <QVariantList>
#include <QDebug>
#include <QString>
#include <iostream>
#include <chrono>
#include <random>
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
}

//Getter for question text
QString GridController::currentQuestionText() const {
    return m_currentQuestionText;
}

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

    // Question-based restrictions (apply to both paths)
    if (!isLengthAllowed(length)) return;
    if (!isStartColumnAllowed(beat)) return;

    // ── Stacking path ────────────────────────────────────────────────────────
    // Only touch the target (beat, row). All other rows at this beat survive.
    if (m_allowStacking) {
        userGrid.RemoveNote(beat, row);       // no-op if the cell was empty
        userAccidental[beat][row] = 0;

        userGrid.AddNote(beat, row, length);
        userAccidental[beat][row] = acc;
        userLength[beat] = length;            // all chord notes share one duration

        for (int i = 0; i < length; ++i)
            emit beatChanged(beat + i);
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

    // Reset expected answers to empty
    expectedRow.fill(-1);
    expectedAccidental.fill(0);
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

    emit questionChanged();


    // Copy expected notes into both storage structures.
    // m_expectedNotes holds the full list (supports multiple per beat for chords).
    // expectedRow/Accidental flat arrays are populated for non-stacking questions
    // and used by the non-stacking grading path.
    for (const NoteInfo& note : q.notes) {
        if (note.beat >= 0 && note.beat < StaffLineGrid::columns
            && note.row >= 0 && note.row < StaffLineGrid::rows) {
            m_expectedNotes.push_back(note);
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
