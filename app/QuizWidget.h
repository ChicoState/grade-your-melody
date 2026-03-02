#pragma once
#include <QWidget>
#include <memory>
#include "quizTypes.h"
#include "quizSession.h"
#include "staffState.h"

class QLabel;
class QPushButton;

class QuizWidget : public QWidget {
    Q_OBJECT

public:
    explicit QuizWidget(QWidget* parent = nullptr);

    // Call to initialise a fresh session for the given mode.
    void startQuiz(QuizMode mode);

public slots:
    // Mirror staff toggle events into the local placed_ NoteGrid.
    void onNoteToggled(StaffState state);

signals:
    void requestStaffClear();    // connected to StaffWidget::clearNotes()
    void requestReturnToMenu();  // connected to MainWindow::onReturnToMenu()

private:
    std::unique_ptr<QuizSession> session_;
    NoteGrid placed_{};   // mirrors what is currently on the staff
    bool     checked_ = false;

    QLabel*      modeLabel_;
    QLabel*      questionNumLabel_;
    QLabel*      promptLabel_;
    QLabel*      explanationLabel_;
    QLabel*      feedbackLabel_;
    QLabel*      scoreLabel_;
    QPushButton* checkButton_;
    QPushButton* nextButton_;
    QPushButton* menuButton_;

    void showQuestion();
    void onCheckClicked();
    void onNextClicked();
    void showResult();

    // True if placed_ has at least one note anywhere.
    bool anyNotePlaced() const;
    // Human-readable list of notes in the expected grid (for wrong-answer feedback).
    static std::string expectedNotesString(const NoteGrid& grid);
};
