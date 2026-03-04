#pragma once
#include "quizTypes.h"

// One generator per quiz mode; each returns a randomised QuizQuestion.
QuizQuestion generateSingleNote();
QuizQuestion generateInterval();
QuizQuestion generateTriad();
QuizQuestion generateSeventhChord();
QuizQuestion generateInversion();

// Calls the appropriate generator for the given mode.
QuizQuestion generateQuestion(QuizMode mode);
