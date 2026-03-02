#include "quizSession.h"
#include "questionBank.h"

QuizSession::QuizSession(QuizMode mode, int numQuestions) {
    questions_.reserve(numQuestions);
    for (int i = 0; i < numQuestions; ++i)
        questions_.push_back(generateQuestion(mode));
}

const QuizQuestion& QuizSession::currentQuestion() const {
    return questions_[index_];
}

int  QuizSession::questionNumber()  const { return index_ + 1; }
int  QuizSession::totalQuestions()  const { return static_cast<int>(questions_.size()); }
bool QuizSession::isFinished()      const { return index_ >= static_cast<int>(questions_.size()); }

bool QuizSession::checkAnswer(const NoteGrid& placed) const {
    return placed == questions_[index_].expected;
}

void QuizSession::recordAnswer(bool correct) {
    if (correct) ++correct_;
    ++asked_;
    ++index_;
}

int    QuizSession::correctCount() const { return correct_; }
double QuizSession::percent()      const {
    if (asked_ == 0) return 0.0;
    return (static_cast<double>(correct_) / asked_) * 100.0;
}
