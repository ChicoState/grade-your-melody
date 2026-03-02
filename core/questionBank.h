#pragma once
#include "quizTypes.h"

// One generator function per quiz mode.
// Each returns a freshly generated QuizQuestion with randomised parameters.
// Notes may be placed in any combination of the 4 columns:
//   - Harmonic questions put all notes in one column.
//   - Melodic/arpeggiated questions spread notes across consecutive columns.
QuizQuestion generateSingleNote();
QuizQuestion generateInterval();
QuizQuestion generateTriad();
QuizQuestion generateSeventhChord();
QuizQuestion generateInversion();

// Dispatch: call the appropriate generator for the given mode.
QuizQuestion generateQuestion(QuizMode mode);
