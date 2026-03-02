#pragma once
#include <QWidget>
#include "quizTypes.h"

class QuizSelectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit QuizSelectionWidget(QWidget* parent = nullptr);

signals:
    void modeSelected(QuizMode mode);
};
