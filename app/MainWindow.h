#pragma once
#include <QMainWindow>
#include "quizTypes.h"

class QStackedWidget;
class StaffAreaWidget;
class QuizWidget;
class QuizSelectionWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onModeSelected(QuizMode mode);
    void onReturnToMenu();

private:
    QStackedWidget*      pages_;
    QuizSelectionWidget* selection_;
    StaffAreaWidget*     staffArea_;
    QuizWidget*          quiz_;
};
