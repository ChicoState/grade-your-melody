#include "GridController.h"
#include <QVariantList>

GridController::GridController(QObject *parent)
    : QObject(parent)
{
    // Default: expect empty on every beat
    expectedRow.fill(-1);
    

    expectedAccidental.fill(0);
    userAccidental.fill(0);
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
    emit beatChanged(beat);
    userAccidental[beat] = 0;
}

void GridController::setNote(int beat, int row, int acc) {
    if (!userGrid.ValidPosition(beat, row)) return;

    // Enforce ONE note per beat:
    clearBeatInternal(beat);
      userGrid.AddNote(beat, row);
    userAccidental[beat] = acc;
    emit beatChanged(beat);
}

int GridController::accidentalForBeat(int beat) const {
    if (beat < 0 || beat >= (int)userAccidental.size()) return 0;
    return userAccidental[beat];
}

int GridController::noteRowForBeat(int beat) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return -1;

    for (int r = 0; r < StaffLineGrid::rows; ++r) {
        if (userGrid.HasNote(beat, r)) return r;
    }
    return -1;
}

void GridController::setExpectedRow(int beat, int row, int acc = 0) {
    if (beat < 0 || beat >= StaffLineGrid::columns) return;
    if (row < -1 || row >= StaffLineGrid::rows) return;
    
    expectedRow[beat] = row;
    expectedAccidental[beat] = acc;
    emit expectedChanged(beat);
}

bool GridController::isBeatCorrect(int beat) const {
    if (beat < 0 || beat >= StaffLineGrid::columns) return false;
    return noteRowForBeat(beat) == expectedRow[beat] && userAccidental[beat] == expectedAccidental[beat];
}


QVariantList GridController::incorrectBeats() const {
    QVariantList wrong;
    for (int b = 0; b < StaffLineGrid::columns; ++b) {
        if (!isBeatCorrect(b)) wrong.append(b);
    }
    return wrong;
}

int GridController::score() const {
    int s = 0;
    for (int b = 0; b < StaffLineGrid::columns; ++b) {
        if (isBeatCorrect(b)) ++s;
    }
    return s;
}
