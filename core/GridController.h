#pragma once
#include <QObject>
#include <QList>
#include <QVariant>
#include <QString>
#include <array>
#include <vector>
#include "staffLineGrid.h"
#include "questionHandler.h"
#include "AudioEngine.h"

class GridController : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString currentQuestionText READ currentQuestionText NOTIFY questionChanged)
    Q_PROPERTY(int currentQuestionNum READ currentQuestionNum NOTIFY questionChanged)
    Q_PROPERTY(int tempoBpm READ tempoBpm WRITE setTempoBpm NOTIFY tempoChanged)
    Q_PROPERTY(int currentPlaybackBeat READ currentPlaybackBeat NOTIFY playbackBeatChanged)
public:
    explicit GridController(QObject *parent = nullptr);

    // UI queries / actions
    Q_INVOKABLE bool hasNote(int beat, int row) const;
    Q_INVOKABLE void setNote(int beat, int row, int acc, int length = 1); // clears beat first, then sets
    Q_INVOKABLE void clearBeat(int beat);            // removes any note in that beat
    Q_INVOKABLE int  noteRowForBeat(int beat) const; // -1 if none
    Q_INVOKABLE int accidentalForBeat(int beat) const;
    Q_INVOKABLE int accidentalForBeatRow(int beat, int row) const;
    Q_INVOKABLE int noteLengthForBeat(int beat) const;
    // grading
    Q_INVOKABLE void setExpectedRow(int beat, int row, int acc, int length = 1); // row -1 means "should be empty"
    Q_INVOKABLE int  expectedRowForBeat(int beat) const;      // get expected row at this beat (-1 if empty)
    Q_INVOKABLE int  expectedLengthForBeat(int beat) const;   // get expected length at this beat
    Q_INVOKABLE int  expectedAccForBeat(int beat) const;      // get expected accidental at this beat
    Q_INVOKABLE bool hasExpectedNote(int beat, int row) const; // check if (beat, row) is expected (for stacking)
    Q_INVOKABLE int  expectedNoteLengthAt(int beat, int row) const; // get expected length at (beat, row)
    Q_INVOKABLE bool isBeatCorrect(int beat) const;
    Q_INVOKABLE bool isNoteIncorrect(int beat, int row) const;
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

    // Audio preparation: returns all currently placed note starts with pitch data
    Q_INVOKABLE QVariantList getCurrentNotes() const;

    // Play the first occupied beat as a chord using the loaded SoundFont
    Q_INVOKABLE void playCurrentNotes();

    // Play the correct answer for the current question (Ear Training mode)
    Q_INVOKABLE void playExpectedAnswer();

    // Stop any currently playing audio immediately
    Q_INVOKABLE void stopPlayback();

    int  tempoBpm() const;
    void setTempoBpm(int bpm);         // clamped to [40, 200]
    Q_INVOKABLE void decreaseTempo();  // decreases by 5, clamped
    Q_INVOKABLE void increaseTempo();  // increases by 5, clamped

    int currentPlaybackBeat() const;   // -1 = none, 0..15 = active beat
signals:
    void beatChanged(int beat);     // beat changed, QML should refresh visuals
    void expectedChanged(int beat); // expected answer changed (optional)
    void benchmarkFinished(QString result);
    void questionChanged();
    void tempoChanged();
    void playbackBeatChanged(); // currentPlaybackBeat changed; -1 = idle
private:
    StaffLineGrid userGrid;
    QuestionHandler questionHandler;
    
    // expected answer: for each beat store the one correct row, or -1 for empty
    std::array<int, StaffLineGrid::columns> expectedRow{};

    //-1 = flat, 0 = natural, +1 = sharp — indexed [beat][row] to support stacked notes
    std::array<int, StaffLineGrid::columns> expectedAccidental{};
    std::array<std::array<int, StaffLineGrid::rows>, StaffLineGrid::columns> userAccidental{};
    std::array<int, StaffLineGrid::columns> userLength{};
    std::array<int, StaffLineGrid::columns> expectedLength{};

    // Full list of expected notes; supports multiple entries per beat for chord questions
    std::vector<NoteInfo> m_expectedNotes;

    void clearBeatInternal(int beat);

    AudioEngine m_audioEngine;

    QString m_currentQuestionText;
    int m_currentQuestionNum = 0;
    int m_tempoBpm           = 100; // clamped to [40, 200]
    int m_currentPlaybackBeat = -1; // -1 = idle, 0..15 = active beat
    std::vector<int> m_allowedLengths;
    std::vector<int> m_allowedStartColumns;
    bool m_allowStacking = false;
    bool m_requireAllFilled = false;
};
