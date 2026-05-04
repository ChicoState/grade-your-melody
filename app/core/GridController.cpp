#include "GridController.h"

GridController::GridController(QObject *parent)
    : QObject(parent),
      m_expectedRows(16, -1),
      m_expectedAccidentals(16, -1),
      m_noteRows(32, -1),
      m_noteAccidentals(32, -1)
{
}

void GridController::setExpectedRow(int index, int row, int accidental)
{
    if (index < 0) {
        return;
    }

    if (index >= m_expectedRows.size()) {
        m_expectedRows.resize(index + 1);
        m_expectedAccidentals.resize(index + 1, -1);
    }

    m_expectedRows[index] = row;
    m_expectedAccidentals[index] = accidental;
}

int GridController::expectedRow(int index) const
{
    if (index < 0 || index >= m_expectedRows.size()) {
        return -1;
    }
    return m_expectedRows[index];
}

int GridController::expectedAccidental(int index) const
{
    if (index < 0 || index >= m_expectedAccidentals.size()) {
        return -1;
    }
    return m_expectedAccidentals[index];
}

void GridController::setNote(int beat, int row, int accidental)
{
    if (beat < 0) {
        return;
    }

    if (beat >= m_noteRows.size()) {
        m_noteRows.resize(beat + 1, -1);
        m_noteAccidentals.resize(beat + 1, -1);
    }

    m_noteRows[beat] = row;
    m_noteAccidentals[beat] = accidental;
    emit beatChanged(beat);
}

void GridController::clearBeat(int beat)
{
    if (beat < 0 || beat >= m_noteRows.size()) {
        return;
    }

    m_noteRows[beat] = -1;
    m_noteAccidentals[beat] = -1;
    emit beatChanged(beat);
}

bool GridController::hasNote(int beat, int row) const
{
    if (beat < 0 || beat >= m_noteRows.size()) {
        return false;
    }
    return m_noteRows[beat] == row;
}

int GridController::score() const
{
    int correct = 0;
    int limit = qMin(m_expectedRows.size(), m_noteRows.size());

    for (int i = 0; i < limit; ++i) {
        if (m_expectedRows[i] != -1 && m_noteRows[i] == m_expectedRows[i]) {
            ++correct;
        }
    }

    return correct;
}

QVariantList GridController::incorrectBeats() const
{
    QVariantList wrong;
    int limit = qMin(m_expectedRows.size(), m_noteRows.size());

    for (int i = 0; i < limit; ++i) {
        if (m_expectedRows[i] != -1 && m_noteRows[i] != m_expectedRows[i]) {
            wrong.append(i);
        }
    }

    return wrong;
}

int GridController::totalExpected() const
{
    int total = 0;

    for (int i = 0; i < m_expectedRows.size(); ++i) {
        if (m_expectedRows[i] != -1) {
            ++total;
        }
    }

    return total;
}