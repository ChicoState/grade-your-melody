#pragma once
#include <QObject>
#include <QList>
#include <QVariant>

#include <array>
#include "staffLineGrid.h"

class GridController : public QObject {
    Q_OBJECT

public:
    explicit GridController(QObject *parent = nullptr);

    // UI queries / actions
    Q_INVOKABLE bool hasNote(int beat, int row) const;
    Q_INVOKABLE void setNote(int beat, int row);     // clears beat first, then sets
    Q_INVOKABLE void clearBeat(int beat);            // removes any note in that beat
    Q_INVOKABLE int  noteRowForBeat(int beat) const; // -1 if none

    // grading
    Q_INVOKABLE void setExpectedRow(int beat, int row); // row -1 means "should be empty"
    Q_INVOKABLE bool isBeatCorrect(int beat) const;
    Q_INVOKABLE int  score() const; // 0..16
    Q_INVOKABLE QVariantList incorrectBeats() const;
signals:
    void beatChanged(int beat);     // beat changed, QML should refresh visuals
    void expectedChanged(int beat); // expected answer changed (optional)

private:
    StaffLineGrid userGrid;

    // expected answer: for each beat store the one correct row, or -1 for empty
    std::array<int, StaffLineGrid::columns> expectedRow{};

    void clearBeatInternal(int beat);
};
