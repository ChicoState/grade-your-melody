#ifndef GRIDCONTROLLER_H
#define GRIDCONTROLLER_H

#include <QObject>
#include <QVector>
#include <QVariantList>

class GridController : public QObject
{
    Q_OBJECT

public:
    explicit GridController(QObject *parent = nullptr);

    Q_INVOKABLE void setExpectedRow(int index, int row, int accidental = -1);
    Q_INVOKABLE int expectedRow(int index) const;
    Q_INVOKABLE int expectedAccidental(int index) const;

    Q_INVOKABLE void setNote(int beat, int row, int accidental);
    Q_INVOKABLE void clearBeat(int beat);
    Q_INVOKABLE bool hasNote(int beat, int row) const;
    Q_INVOKABLE int score() const;
    Q_INVOKABLE QVariantList incorrectBeats() const;
    Q_INVOKABLE int totalExpected() const;

signals:
    void beatChanged(int beat);

private:
    QVector<int> m_expectedRows;
    QVector<int> m_expectedAccidentals;
    QVector<int> m_noteRows;
    QVector<int> m_noteAccidentals;
};

#endif