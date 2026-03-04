#include "questionBank.h"
#include <random>
#include <string>

static const char* ROW_NAMES[11] = {
    "C4","D4","E4","F4","G4","A4","B4","C5","D5","E5","F5"
};

static const char* ROW_DESC[11] = {
    "C4 is on the ledger line just below the staff",
    "D4 is in the space just below the first staff line",
    "E4 is on the first (bottom) staff line",
    "F4 is in the first space",
    "G4 is on the second staff line",
    "A4 is in the second space",
    "B4 is on the middle (third) staff line",
    "C5 is in the third space",
    "D5 is on the fourth staff line",
    "E5 is in the fourth space",
    "F5 is on the top (fifth) staff line"
};

// Diatonic interval names indexed by row-difference 1..7.
static const char* INTERVAL_NAMES[8] = {
    "", "2nd", "3rd", "4th", "5th", "6th", "7th", "octave"
};

// Beat label helper ("beat 1" .. "beat 4").
static std::string beatLabel(int col) {
    return "beat " + std::to_string(col + 1);
}

static std::mt19937& rng() {
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

static int randInt(int lo, int hi) {  // inclusive
    return std::uniform_int_distribution<int>(lo, hi)(rng());
}

static bool coinFlip() { return randInt(0, 1) == 1; }

QuizQuestion generateSingleNote() {
    int row = randInt(0, 10);
    int col = randInt(0, 3);
    NoteGrid expected{};
    expected[col].insert(row);
    return {
        std::string("Place ") + ROW_NAMES[row] + " on " + beatLabel(col) + ".",
        ROW_DESC[row],
        expected
    };
}

QuizQuestion generateInterval() {
    int root = randInt(0, 9);
    int maxInterval = std::min(7, 10 - root);
    int interval = randInt(1, maxInterval);
    int top = root + interval;

    NoteGrid expected{};
    std::string prompt, explanation;

    explanation =
        std::string(ROW_NAMES[root]) + " and " + ROW_NAMES[top] +
        " are a " + INTERVAL_NAMES[interval] + " apart.";

    if (coinFlip()) {
        // Harmonic: both notes in the same random column.
        int col = randInt(0, 3);
        expected[col].insert(root);
        expected[col].insert(top);
        prompt = std::string("Place a ") + INTERVAL_NAMES[interval] +
                 " above " + ROW_NAMES[root] +
                 " (both notes together on " + beatLabel(col) + ").";
    } else {
        // Melodic: root in col, top note in col+1.
        int col = randInt(0, 2);
        expected[col].insert(root);
        expected[col + 1].insert(top);
        prompt = std::string("Place ") + ROW_NAMES[root] +
                 " on " + beatLabel(col) + " and a " +
                 INTERVAL_NAMES[interval] + " above it on " +
                 beatLabel(col + 1) + ".";
    }

    return { prompt, explanation, expected };
}

QuizQuestion generateTriad() {
    // root+4 must fit within row 10.
    int root = randInt(0, 6);
    int r3 = root + 2, r5 = root + 4;

    NoteGrid expected{};
    std::string prompt, explanation;

    explanation =
        std::string("The ") + ROW_NAMES[root] + " triad uses " +
        ROW_NAMES[root] + ", " + ROW_NAMES[r3] + ", and " + ROW_NAMES[r5] + ".";

    if (coinFlip()) {
        // Harmonic: all three notes in the same column.
        int col = randInt(0, 3);
        expected[col].insert(root);
        expected[col].insert(r3);
        expected[col].insert(r5);
        prompt = std::string("Place a ") + ROW_NAMES[root] +
                 " triad (root, 3rd, 5th) on " + beatLabel(col) + ".";
    } else {
        // Melodic: root→3rd→5th across three consecutive columns.
        int col = randInt(0, 1);  // col, col+1, col+2 must all be ≤ 3
        expected[col].insert(root);
        expected[col + 1].insert(r3);
        expected[col + 2].insert(r5);
        prompt = std::string("Arpeggiate a ") + ROW_NAMES[root] +
                 " triad ascending: root on " + beatLabel(col) +
                 ", 3rd on " + beatLabel(col + 1) +
                 ", 5th on " + beatLabel(col + 2) + ".";
    }

    return { prompt, explanation, expected };
}

QuizQuestion generateSeventhChord() {
    // root+6 must fit within row 10.
    int root = randInt(0, 4);
    int r3 = root + 2, r5 = root + 4, r7 = root + 6;

    NoteGrid expected{};
    std::string prompt, explanation;

    explanation =
        std::string("The ") + ROW_NAMES[root] + " seventh chord uses " +
        ROW_NAMES[root] + ", " + ROW_NAMES[r3] + ", " +
        ROW_NAMES[r5] + ", and " + ROW_NAMES[r7] + ".";

    if (coinFlip()) {
        // Harmonic: all four notes in the same column.
        int col = randInt(0, 3);
        expected[col].insert(root);
        expected[col].insert(r3);
        expected[col].insert(r5);
        expected[col].insert(r7);
        prompt = std::string("Place a ") + ROW_NAMES[root] +
                 " seventh chord (root, 3rd, 5th, 7th) on " + beatLabel(col) + ".";
    } else {
        // Melodic: one note per beat across all four columns (must start at col 0).
        expected[0].insert(root);
        expected[1].insert(r3);
        expected[2].insert(r5);
        expected[3].insert(r7);
        prompt = std::string("Arpeggiate a ") + ROW_NAMES[root] +
                 " seventh chord across beats 1–4: " +
                 ROW_NAMES[root] + ", " + ROW_NAMES[r3] + ", " +
                 ROW_NAMES[r5] + ", " + ROW_NAMES[r7] + ".";
    }

    return { prompt, explanation, expected };
}

// Root-position data for C, F, G major (fits all inversions within rows 0..10).
static const int INV_ROOTS[3]   = { 0, 3, 4 };
static const char* INV_NAMES[3] = { "C", "F", "G" };
static const char* INV_LABEL[3] = { "root position", "first inversion", "second inversion" };

QuizQuestion generateInversion() {
    int chordIdx = randInt(0, 2);
    int inv      = randInt(0, 2);
    int r        = INV_ROOTS[chordIdx];

    // Compute the three note rows for the chosen inversion.
    int notes[3];
    if (inv == 0) {
        notes[0] = r;     notes[1] = r + 2; notes[2] = r + 4;
    } else if (inv == 1) {
        // 3rd on bottom; original root reappears an octave up (diatonic +7)
        notes[0] = r + 2; notes[1] = r + 4; notes[2] = r + 7;
    } else {
        // 5th on bottom; root and 3rd an octave up
        notes[0] = r + 4; notes[1] = r + 7; notes[2] = r + 9;
    }

    NoteGrid expected{};
    std::string prompt, explanation;

    // Build note list string for explanation.
    std::string noteList =
        std::string(ROW_NAMES[notes[0]]) + ", " +
        ROW_NAMES[notes[1]] + ", " + ROW_NAMES[notes[2]];

    explanation =
        std::string(INV_NAMES[chordIdx]) + " major " + INV_LABEL[inv] +
        " uses: " + noteList + ".";

    if (coinFlip()) {
        // Harmonic: all three notes in the same column.
        int col = randInt(0, 3);
        for (int i = 0; i < 3; ++i) expected[col].insert(notes[i]);
        prompt = std::string("Place ") + INV_NAMES[chordIdx] + " major in " +
                 INV_LABEL[inv] + " on " + beatLabel(col) + ".";
    } else {
        // Melodic: ascending across three consecutive columns.
        int col = randInt(0, 1);
        for (int i = 0; i < 3; ++i) expected[col + i].insert(notes[i]);
        prompt = std::string("Arpeggiate ") + INV_NAMES[chordIdx] + " major in " +
                 INV_LABEL[inv] + " ascending across " +
                 beatLabel(col) + "–" + beatLabel(col + 2) + ".";
    }

    return { prompt, explanation, expected };
}

QuizQuestion generateQuestion(QuizMode mode) {
    switch (mode) {
        case QuizMode::SingleNotes:   return generateSingleNote();
        case QuizMode::Intervals:     return generateInterval();
        case QuizMode::Triads:        return generateTriad();
        case QuizMode::SeventhChords: return generateSeventhChord();
        case QuizMode::Inversions:    return generateInversion();
    }
    return generateSingleNote();  // unreachable; satisfies compiler
}
