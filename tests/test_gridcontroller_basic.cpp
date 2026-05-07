#include <gtest/gtest.h>

#include "GridController.h"

TEST(GridControllerTest, NotePlacement) {
    GridController gc;

    // Place a note at beat 0, row 4, natural, length 1
    gc.setNote(0, 4, 0, 1);
    EXPECT_TRUE(gc.hasNote(0, 4));
    EXPECT_EQ(gc.accidentalForBeat(0), 0);
    EXPECT_EQ(gc.noteLengthForBeat(0), 1);
    EXPECT_EQ(gc.noteRowForBeat(0), 4);
}

TEST(GridControllerTest, NoteClearing) {
    GridController gc;

    // Place a note and then clear it
    gc.setNote(0, 4, 0, 1);
    gc.clearBeat(0);
    EXPECT_FALSE(gc.hasNote(0, 4));
    EXPECT_EQ(gc.accidentalForBeat(0), 0);
    EXPECT_EQ(gc.noteLengthForBeat(0), 0);
    EXPECT_EQ(gc.noteRowForBeat(0), -1);
}

TEST(GridControllerTest, Grading) {
    GridController gc;

    // Set expected note at beat 0, row 4, natural, length 1
    gc.setExpectedRow(0, 4, 0, 1);

    // Initially should be incorrect
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));

    // Place incorrect note
    gc.setNote(0, 3, 0, 1);
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 3));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));

    // Clear beat and check again
    gc.clearBeat(0);
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));

    // Place correct note
    gc.setNote(0, 4, 0, 1);
    EXPECT_TRUE(gc.isBeatCorrect(0));
    EXPECT_FALSE(gc.isNoteIncorrect(0, 4));

    // Clear beat and check again
    gc.clearBeat(0);
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));
}

TEST(GridControllerTest, ScoreAndTotal) {
    GridController gc;

    // Set expected notes at beat 0 and 1
    gc.setExpectedRow(0, 4, 0, 1);
    gc.setExpectedRow(1, 5, 0, 1);

    // Initially score should be 0 and total should be 2
    EXPECT_EQ(gc.score(), 0);
    EXPECT_EQ(gc.totalExpected(), 2);

    // Place one correct note
    gc.setNote(0, 4, 0, 1);
    EXPECT_EQ(gc.score(), 1);
    EXPECT_EQ(gc.totalExpected(), 2);

    // Place second correct note
    gc.setNote(1, 5, 0, 1);
    EXPECT_EQ(gc.score(), 2);
    EXPECT_EQ(gc.totalExpected(), 2);

    // Clear one beat and check score again
    gc.clearBeat(0);
    EXPECT_EQ(gc.score(), 1);
}

TEST(GridControllerTest, IncorrectBeats) {
    GridController gc;

    // Set expected notes at beat 0 and 1
    gc.setExpectedRow(0, 4, 0, 1);
    gc.setExpectedRow(1, 5, 0, 1);

    // Initially both beats should be incorrect
    QVariantList incorrect = gc.incorrectBeats();
    EXPECT_EQ(incorrect.size(), 2);
    EXPECT_TRUE(incorrect.contains(0));
    EXPECT_TRUE(incorrect.contains(1));

    // Place correct note at beat 0
    gc.setNote(0, 4, 0, 1);
    incorrect = gc.incorrectBeats();
    EXPECT_EQ(incorrect.size(), 1);
    EXPECT_FALSE(incorrect.contains(0));
    EXPECT_TRUE(incorrect.contains(1));

    // Place correct note at beat 1
    gc.setNote(1, 5, 0, 1);
    incorrect = gc.incorrectBeats();
    EXPECT_EQ(incorrect.size(), 0);

    // Clear beat 0 and check again
    gc.clearBeat(0);
    incorrect = gc.incorrectBeats();
    EXPECT_EQ(incorrect.size(), 1);
    EXPECT_TRUE(incorrect.contains(0));
}

TEST(GridControllerTest, AccidentalScoring_Flat) {
    GridController gc;

    // Set expected note with accidental at beat 0
    gc.setExpectedRow(0, 4, -1, 1); // Flat

    // Clear beat and place correct note with incorrect accidental
    gc.clearBeat(0);
    gc.setNote(0, 4, 1, 1); // Sharp
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));

    // Clear beat and place incorrect note with correct accidental
    gc.clearBeat(0);
    gc.setNote(0, 3, -1, 1); // Flat but wrong row
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 3));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));

    // Place correct note and accidental
    gc.clearBeat(0);
    gc.setNote(0, 4, -1, 1); // Flat
    EXPECT_TRUE(gc.isBeatCorrect(0));
    EXPECT_FALSE(gc.isNoteIncorrect(0, 4));
}

TEST(GridControllerTest, AccidentalScoring_Sharp) {
    GridController gc;

    // Set expected note with accidental at beat 0
    gc.setExpectedRow(0, 4, 1, 1); // Sharp

    // Clear beat and place correct note with incorrect accidental
    gc.clearBeat(0);
    gc.setNote(0, 4, -1, 1); // Flat
    EXPECT_FALSE(gc.isBeatCorrect(0));

    // Clear beat and place incorrect note with correct accidental
    gc.clearBeat(0);
    gc.setNote(0, 3, 1, 1); // Sharp but wrong row
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 3));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));

    // Place correct note and accidental
    gc.clearBeat(0);
    gc.setNote(0, 4, 1, 1); // Sharp
    EXPECT_TRUE(gc.isBeatCorrect(0));
    EXPECT_FALSE(gc.isNoteIncorrect(0, 4));
}

TEST(GridControllerTest, AccidentalScoring_Natural) {
    GridController gc;

    // Set expected note with accidental at beat 0
    gc.setExpectedRow(0, 4, 0, 1); // Natural

    // Clear beat and place correct note with incorrect accidental
    gc.clearBeat(0);
    gc.setNote(0, 4, 1, 1); // Sharp
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));

    // Clear beat and place incorrect note with correct accidental
    gc.clearBeat(0);
    gc.setNote(0, 3, 0, 1); // Natural but wrong row
    EXPECT_FALSE(gc.isBeatCorrect(0));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 3));
    EXPECT_TRUE(gc.isNoteIncorrect(0, 4));

    // Place correct note and accidental
    gc.clearBeat(0);
    gc.setNote(0, 4, 0, 1); // Natural
    EXPECT_TRUE(gc.isBeatCorrect(0));
    EXPECT_FALSE(gc.isNoteIncorrect(0, 4));
}