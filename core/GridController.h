#pragma once
#include <QObject>
#include <QList>
#include <QVariant>
#include <QString>
#include <array>
#include <vector>
#include "staffLineGrid.h"
#include "questionHandler.h"

class GridController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentQuestionText READ currentQuestionText NOTIFY questionChanged)
    Q_PROPERTY(int currentQuestionNum READ currentQuestionNum NOTIFY questionChanged)
public:
    explicit GridController(QObject *parent = nullptr);

    // UI queries / actions
    Q_INVOKABLE bool hasNote(int beat, int row) const;
    Q_INVOKABLE void setNote(int beat, int row, int acc, int length = 1); // clears beat first, then sets
    Q_INVOKABLE void clearBeat(int beat);            // removes any note in that beat
    Q_INVOKABLE int  noteRowForBeat(int beat) const; // -1 if none
    Q_INVOKABLE int accidentalForBeat(int beat) const;
    Q_INVOKABLE int noteLengthForBeat(int beat) const;
    // grading
    Q_INVOKABLE void setExpectedRow(int beat, int row, int acc, int length = 1); // row -1 means "should be empty"
    Q_INVOKABLE bool isBeatCorrect(int beat) const;
    Q_INVOKABLE int  score() const;
    Q_INVOKABLE int  totalExpected() const;
    Q_INVOKABLE QVariantList incorrectBeats() const;
    Q_INVOKABLE void loadQuestion(int questionNum);
    Q_INVOKABLE void nextQuestion();
    Q_INVOKABLE void previousQuestion();
    Q_INVOKABLE int  currentQuestionNum() const;
    Q_INVOKABLE int  totalQuestionsAvailable();
    Q_INVOKABLE void runMillion();

    QString currentQuestionText() const;

    Q_INVOKABLE bool isLengthAllowed(int length) const;
    Q_INVOKABLE bool isStartColumnAllowed(int beat) const;
    Q_INVOKABLE bool canGrade() const;
signals:
    void beatChanged(int beat);     // beat changed, QML should refresh visuals
    void expectedChanged(int beat); // expected answer changed (optional)
    void benchmarkFinished(QString result);
    void questionChanged();
private:
    StaffLineGrid userGrid;
    QuestionHandler questionHandler;
    
    // expected answer: for each beat store the one correct row, or -1 for empty
    std::array<int, StaffLineGrid::columns> expectedRow{};

    //-1 = fat, 0 = norm, +1 = sharp
    std::array<int, StaffLineGrid::columns> expectedAccidental{};
    std::array<int, StaffLineGrid::columns> userAccidental{};
    std::array<int, StaffLineGrid::columns> userLength{};
    std::array<int, StaffLineGrid::columns> expectedLength{};

    void clearBeatInternal(int beat);

    QString m_currentQuestionText;
    int m_currentQuestionNum = 0;
    std::vector<int> m_allowedLengths;
    std::vector<int> m_allowedStartColumns;
    bool m_allowStacking = false;
    bool m_requireAllFilled = false;
};
