#pragma once
#include "quizTypes.h"
#include <vector>

class QuizSession {
public:
    QuizSession(QuizMode mode, int numQuestions);

    const QuizQuestion& currentQuestion() const;
    int  questionNumber()  const;  // 1-based
    int  totalQuestions()  const;
    bool isFinished()      const;

    // Returns true if `placed` exactly matches the expected NoteGrid.
    bool checkAnswer(const NoteGrid& placed) const;

    // Record the result of the current question and advance the index.
    void recordAnswer(bool correct);

    int    correctCount() const;
    double percent()      const;  // 0.0–100.0

private:
    std::vector<QuizQuestion> questions_;
    int index_   = 0;
    int correct_ = 0;
    int asked_   = 0;
};
