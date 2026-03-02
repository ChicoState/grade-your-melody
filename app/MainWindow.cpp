#include "MainWindow.h"
#include "AspectRatioWidget.h"
#include "StaffAreaWidget.h"
#include "StaffWidget.h"
#include "QuizWidget.h"
#include "QuizSelectionWidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QWidget>
#include <QFont>
#include <QFrame>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Grade Your Melody");

    auto* content = new QWidget;

    // ── Title ─────────────────────────────────────────────────────────
    auto* title = new QLabel("Grade Your Melody", content);
    QFont tf = title->font();
    tf.setPointSize(24);
    tf.setBold(true);
    title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);

    auto* rule = new QFrame(content);
    rule->setFrameShape(QFrame::HLine);
    rule->setFrameShadow(QFrame::Sunken);

    // ── Pages: selection screen vs. game view ─────────────────────────
    pages_ = new QStackedWidget(content);

    // Page 0 — mode selection
    selection_ = new QuizSelectionWidget;
    pages_->addWidget(selection_);

    // Page 1 — staff + quiz
    staffArea_ = new StaffAreaWidget;
    quiz_      = new QuizWidget;

    staffArea_->setMaximumHeight(360);
    staffArea_->setStyleSheet("QWidget { border: 1px solid black; background: white; }");
    quiz_->setStyleSheet(
        "QuizWidget { border: 1px solid black; background: white; }"
        "QGroupBox  { border: 1px solid black; border-radius: 4px; margin-top: 8px;"
        "             background: white; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; }");

    auto* gameView = new QWidget;
    auto* gameLayout = new QVBoxLayout(gameView);
    gameLayout->setSpacing(12);
    gameLayout->setContentsMargins(0, 0, 0, 0);
    gameLayout->addWidget(staffArea_, /*stretch=*/1);
    gameLayout->addWidget(quiz_,      /*stretch=*/0);

    pages_->addWidget(gameView);

    // ── Hint bar ──────────────────────────────────────────────────────
    auto* hint = new QLabel(
        "Click the staff to place or remove a note.", content);
    hint->setAlignment(Qt::AlignCenter);
    QFont hf = hint->font();
    hf.setPointSize(9);
    hint->setFont(hf);
    hint->setStyleSheet("color: #666;");

    // ── Root layout ───────────────────────────────────────────────────
    auto* layout = new QVBoxLayout(content);
    layout->setContentsMargins(16, 16, 16, 12);
    layout->setSpacing(10);
    layout->addWidget(title);
    layout->addWidget(rule);
    layout->addWidget(pages_, /*stretch=*/1);
    layout->addWidget(hint);

    auto* arw = new AspectRatioWidget(content, 4.0 / 3.0, this);
    setCentralWidget(arw);
    resize(960, 720);

    // ── Signal wiring ─────────────────────────────────────────────────
    connect(selection_, &QuizSelectionWidget::modeSelected,
            this,       &MainWindow::onModeSelected);

    connect(quiz_,  &QuizWidget::requestReturnToMenu,
            this,   &MainWindow::onReturnToMenu);

    connect(quiz_,                &QuizWidget::requestStaffClear,
            staffArea_->staff(),  &StaffWidget::clearNotes);

    connect(staffArea_->staff(), &StaffWidget::noteToggled,
            quiz_,               &QuizWidget::onNoteToggled);
}

void MainWindow::onModeSelected(QuizMode mode) {
    pages_->setCurrentIndex(1);
    quiz_->startQuiz(mode);  // startQuiz emits requestStaffClear internally
}

void MainWindow::onReturnToMenu() {
    pages_->setCurrentIndex(0);
}
