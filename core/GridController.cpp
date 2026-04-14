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
    if (beat + length > StaffLineGrid::columns) return;

    // Clear all beats this note will occupy
    for (int i = 0; i < length; i++) {
        clearBeatInternal(beat + i);
        userAccidental[beat + i] = 0;
        userLength[beat + i] = 0;
    }

    userGrid.AddNote(beat, row, length);
    userAccidental[beat] = acc;
    userLength[beat] = length;

    for (int i = 0; i < length; i++) {
        emit beatChanged(beat + i);
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
