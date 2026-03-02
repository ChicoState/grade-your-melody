#include "QuizWidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QString>

// ── Local helper ──────────────────────────────────────────────────────────────

static char letterGrade(double pct) {
    if (pct >= 90.0) return 'A';
    if (pct >= 80.0) return 'B';
    if (pct >= 70.0) return 'C';
    if (pct >= 60.0) return 'D';
    return 'F';
}

// ── Styling ───────────────────────────────────────────────────────────────────

static const char* BUTTON_STYLE =
    "QPushButton {"
    "  padding: 8px 16px;"
    "  border: 1px solid #b0b0b0;"
    "  border-radius: 4px;"
    "  background-color: #f5f5f5;"
    "  min-height: 32px;"
    "}"
    "QPushButton:hover:enabled {"
    "  background-color: #dce8ff;"
    "  border-color: #6699cc;"
    "}"
    "QPushButton:disabled { color: #aaa; }";

// ── Constructor ───────────────────────────────────────────────────────────────

QuizWidget::QuizWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumHeight(180);

    modeLabel_ = new QLabel(this);
    modeLabel_->setStyleSheet("color: #555; font-style: italic;");

    questionNumLabel_ = new QLabel(this);
    questionNumLabel_->setStyleSheet("color: #888;");

    promptLabel_ = new QLabel(this);
    promptLabel_->setWordWrap(true);
    QFont pf = promptLabel_->font();
    pf.setPointSize(13);
    promptLabel_->setFont(pf);

    feedbackLabel_ = new QLabel(this);
    QFont ff = feedbackLabel_->font();
    ff.setPointSize(12);
    ff.setBold(true);
    feedbackLabel_->setFont(ff);

    explanationLabel_ = new QLabel(this);
    explanationLabel_->setWordWrap(true);
    explanationLabel_->setStyleSheet("color: #336699;");
    explanationLabel_->hide();

    scoreLabel_ = new QLabel(this);

    checkButton_ = new QPushButton("Check Answer", this);
    checkButton_->setStyleSheet(BUTTON_STYLE);
    checkButton_->setEnabled(false);
    connect(checkButton_, &QPushButton::clicked, this, &QuizWidget::onCheckClicked);

    nextButton_ = new QPushButton("Next Question", this);
    nextButton_->setStyleSheet(BUTTON_STYLE);
    nextButton_->hide();
    connect(nextButton_, &QPushButton::clicked, this, &QuizWidget::onNextClicked);

    menuButton_ = new QPushButton("Back to Menu", this);
    menuButton_->setStyleSheet(BUTTON_STYLE);
    menuButton_->hide();
    connect(menuButton_, &QPushButton::clicked, this, &QuizWidget::requestReturnToMenu);

    auto* btnRow = new QHBoxLayout;
    btnRow->setSpacing(8);
    btnRow->addWidget(checkButton_);
    btnRow->addWidget(nextButton_);
    btnRow->addStretch();
    btnRow->addWidget(menuButton_);

    auto* inner = new QVBoxLayout;
    inner->setContentsMargins(12, 12, 12, 12);
    inner->setSpacing(8);
    inner->addWidget(modeLabel_);
    inner->addWidget(questionNumLabel_);
    inner->addWidget(promptLabel_);
    inner->addWidget(feedbackLabel_);
    inner->addWidget(explanationLabel_);
    inner->addLayout(btnRow);
    inner->addWidget(scoreLabel_);
    inner->addStretch();

    auto* group = new QGroupBox("Quiz", this);
    group->setLayout(inner);

    auto* outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->addWidget(group);
}

// ── Public API ────────────────────────────────────────────────────────────────

void QuizWidget::startQuiz(QuizMode mode) {
    session_ = std::make_unique<QuizSession>(mode, 10);
    placed_  = NoteGrid{};
    emit requestStaffClear();

    static const char* MODE_NAMES[] = {
        "Single Notes", "Intervals", "Triads", "Seventh Chords", "Inversions"
    };
    modeLabel_->setText(
        QString("Mode: %1").arg(MODE_NAMES[static_cast<int>(mode)]));

    showQuestion();
}

// ── Slots ─────────────────────────────────────────────────────────────────────

void QuizWidget::onNoteToggled(StaffState state) {
    if (!session_ || session_->isFinished()) return;
    if (!state.col.has_value() || !state.row.has_value()) return;
    if (checked_) return;  // ignore staff changes after answer is checked

    int col = *state.col;
    int row = *state.row;
    if (state.added)
        placed_[col].insert(row);
    else
        placed_[col].erase(row);

    checkButton_->setEnabled(anyNotePlaced());
}

// ── Private helpers ───────────────────────────────────────────────────────────

bool QuizWidget::anyNotePlaced() const {
    for (const auto& col : placed_)
        if (!col.empty()) return true;
    return false;
}

std::string QuizWidget::expectedNotesString(const NoteGrid& grid) {
    static const char* ROW_NAMES[11] = {
        "C4","D4","E4","F4","G4","A4","B4","C5","D5","E5","F5"
    };
    std::string result;
    for (int col = 0; col < 4; ++col) {
        for (int row : grid[col]) {
            if (!result.empty()) result += ", ";
            result += ROW_NAMES[row];
            result += " (beat " + std::to_string(col + 1) + ")";
        }
    }
    return result;
}

void QuizWidget::showQuestion() {
    if (!session_ || session_->isFinished()) { showResult(); return; }

    checked_ = false;
    placed_  = NoteGrid{};

    const QuizQuestion& q = session_->currentQuestion();

    questionNumLabel_->setText(
        QString("Question %1 of %2")
            .arg(session_->questionNumber())
            .arg(session_->totalQuestions()));
    promptLabel_->setText(QString::fromStdString(q.prompt));
    feedbackLabel_->clear();
    explanationLabel_->hide();
    scoreLabel_->setText(
        QString("Score: %1 / %2")
            .arg(session_->correctCount())
            .arg(session_->questionNumber() - 1));

    checkButton_->setEnabled(false);
    checkButton_->show();
    nextButton_->hide();
    menuButton_->hide();
}

void QuizWidget::onCheckClicked() {
    if (!session_ || session_->isFinished() || checked_) return;
    checked_ = true;

    // Capture question data BEFORE advancing the session index.
    const QuizQuestion q = session_->currentQuestion();
    bool correct = session_->checkAnswer(placed_);
    session_->recordAnswer(correct);

    if (correct) {
        feedbackLabel_->setStyleSheet("color: #2a7a2a; font-weight: bold;");
        feedbackLabel_->setText("Correct!");
    } else {
        feedbackLabel_->setStyleSheet("color: #aa2222; font-weight: bold;");
        feedbackLabel_->setText(
            QString("Wrong — expected: %1")
                .arg(QString::fromStdString(expectedNotesString(q.expected))));
    }

    explanationLabel_->setText(QString::fromStdString(q.explanation));
    explanationLabel_->show();

    scoreLabel_->setText(
        QString("Score: %1 / %2")
            .arg(session_->correctCount())
            .arg(session_->questionNumber() - 1));

    checkButton_->setEnabled(false);
    checkButton_->hide();
    nextButton_->show();
    if (session_->isFinished()) menuButton_->show();
}

void QuizWidget::onNextClicked() {
    emit requestStaffClear();
    showQuestion();
}

void QuizWidget::showResult() {
    questionNumLabel_->setText("Quiz Complete!");
    promptLabel_->setText(
        QString("You scored %1 out of %2.")
            .arg(session_->correctCount())
            .arg(session_->totalQuestions()));
    scoreLabel_->setText(
        QString("Final Grade: %1")
            .arg(letterGrade(session_->percent())));

    feedbackLabel_->clear();
    explanationLabel_->hide();
    checkButton_->hide();
    nextButton_->hide();
    menuButton_->show();
}
