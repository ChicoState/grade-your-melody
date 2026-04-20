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
    userAccidental.fill(0);    
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
    userAccidental[beat] = 0;
    userLength[beat] = 0;
    emit beatChanged(beat);
}

void GridController::setNote(int beat, int row, int acc, int length) {
    if (!userGrid.ValidPosition(beat, row)) return;
    if (beat < 0 || beat >= StaffLineGrid::columns) return;
    if (length <= 0) return;
    if (beat + length > StaffLineGrid::columns) return;

    // Question-based restrictions
    if (!isLengthAllowed(length)) return;
    if (!isStartColumnAllowed(beat)) return;

    if (!m_allowStacking) {
        for (int r = 0; r < StaffLineGrid::rows; ++r) {
            if (userGrid.HasNote(beat, r)) {
            return;
            }
        }
    }   

    const int measureStart = (beat / 8) * 8;
    const int measureOffset = beat % 8;

    // Prevent crossing the current measure boundary
    if (measureOffset + length > 8) return;

    // Enforce notation-based start positions for 4/4 with 8 columns per measure
    switch (length) {
        case 8: // whole note: beat 1 only
            if (measureOffset != 0) return;
            break;

        case 4: // half note: beats 1 or 3
            if (measureOffset != 0 && measureOffset != 4) return;
            break;

        case 2: // quarter note: beats 1, 2, 3, 4
            if (measureOffset % 2 != 0) return;
            break;

        case 1: // eighth note: anywhere
            break;

        default:
            return; // unsupported note length
    }

    const int measureEnd = measureStart + 8;

    // Track which beats in this measure need UI refresh
    bool beatsToRefresh[8] = { false };

    // Simulate currently occupied columns in this measure
    bool occupied[8] = { false };
    for (int c = measureStart; c < measureEnd; ++c) {
        if (noteRowForBeat(c) != -1) {
            occupied[c - measureStart] = true;
        }
    }

    // Simulate clearing any note touched by the new note.
    // RemoveNote removes the entire existing note, not just one column.
    for (int c = beat; c < beat + length; ++c) {
        for (int r = 0; r < StaffLineGrid::rows; ++r) {
            if (!userGrid.HasNote(c, r)) continue;

            // Find the start of the existing note on this row
            int noteStart = c;
            while (noteStart > measureStart && userGrid.HasNote(noteStart - 1, r)) {
                --noteStart;
            }

            int existingLength = userLength[noteStart];
            if (existingLength <= 0) existingLength = 1;

            for (int k = noteStart; k < noteStart + existingLength && k < measureEnd; ++k) {
                occupied[k - measureStart] = false;
                beatsToRefresh[k - measureStart] = true;
            }
        }
    }

    int occupiedAfterClear = 0;
    for (int i = 0; i < 8; ++i) {
        if (occupied[i]) {
            ++occupiedAfterClear;
        }
    }

    // Reject if total occupied duration in the measure would exceed capacity
    if (occupiedAfterClear + length > 8) return;

    // Clear all beats this note will occupy
    for (int i = 0; i < length; ++i) {
        clearBeatInternal(beat + i);
        userAccidental[beat + i] = 0;
        userLength[beat + i] = 0;
        beatsToRefresh[beat + i - measureStart] = true;
    }

    // Place the new note
    userGrid.AddNote(beat, row, length);
    userAccidental[beat] = acc;
    userLength[beat] = length;

    // Refresh the new note's occupied columns too
    for (int i = 0; i < length; ++i) {
        beatsToRefresh[beat + i - measureStart] = true;
    }

    // Emit updates for every affected beat in the measure
    for (int i = 0; i < 8; ++i) {
        if (beatsToRefresh[i]) {
            emit beatChanged(measureStart + i);
        }
    }
}

int GridController::accidentalForBeat(int beat) const {
    if (beat < 0 || beat >= (int)userAccidental.size()) return 0;
    return userAccidental[beat];
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

bool GridController::isBeatCorrect(int beat) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return false;
    return noteRowForBeat(beat) == expectedRow[beat]
        && userAccidental[beat] == expectedAccidental[beat];
        
}


QVariantList GridController::incorrectBeats() const {
    QVariantList wrong;
    for (int b = 0; b < StaffLineGrid::columns; ++b) {
        if (expectedRow[b] == -1) continue; // skip beats with no expected note
        if (!isBeatCorrect(b)) wrong.append(b);
    }
    return wrong;
}

int GridController::score() const {
    int s = 0;
    for (int b = 0; b < StaffLineGrid::columns; ++b) {
        if (expectedRow[b] == -1) continue;
        if (isBeatCorrect(b)) ++s;
    }
    return s;
}

int GridController::totalExpected() const {
    int t = 0;
    for (int b = 0; b < StaffLineGrid::columns; ++b) {
        if (expectedRow[b] != -1) ++t;
    }
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


void GridController::loadQuestion(int questionNum) {
    // reset expected answers to empty
    expectedRow.fill(-1);
    expectedAccidental.fill(0);    

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


    // copy notes from question vector into arrays
    for (const NoteInfo& note : q.notes) { if (note.beat >= 0 && note.beat < StaffLineGrid::columns && note.row >= 0 && note.row < StaffLineGrid::rows) {        
            expectedRow[note.beat] = note.row;
            expectedAccidental[note.beat] = note.accent;
        }
    }
    // notify QML/UI that everything changed
    // if we do not do this everytiime we load a question the ui will knot know to look for a new question. definitions in .h
    for (int b = 0; b < StaffLineGrid::columns; ++b) {
        emit expectedChanged(b); 
        emit beatChanged(b);
    }
}
