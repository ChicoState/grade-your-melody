#pragma once
#include <array>
#include <set>
#include <string>

enum class QuizMode { SingleNotes, Intervals, Triads, SeventhChords, Inversions };

// Per-column set of row indices the user must place (rows 0=C4 .. 10=F5).
using NoteGrid = std::array<std::set<int>, 4>;

struct QuizQuestion {
    std::string prompt;       // instruction shown to user before placing notes
    std::string explanation;  // shown after checking the answer
    NoteGrid    expected;     // expected[col] = set of rows that must be occupied
};
