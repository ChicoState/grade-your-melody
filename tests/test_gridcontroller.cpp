#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "GridController.h"

using namespace std;

TEST(GridControllerTest, ClearStaffRemovesUserNotesOnly) {
    // Minimal scale question: quarter notes (length 2) on every even beat.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled\n"
        "C major scale,\"(0-C4 2-D4 4-E4 6-F4 8-G4 10-A4 12-B4 14-C5)\",2,\"0 2 4 6 8 10 12 14\",false,true\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    GridController controller;
    controller.loadQuestion(1);

    // Expected answer: C4 at beat 0 (row 0).
    ASSERT_TRUE(controller.hasExpectedNote(0, 0));

    // Place a quarter note that matches the expected first note.
    controller.setNote(0, 0, 0, 2);
    ASSERT_TRUE(controller.hasNote(0, 0));

    // clearStaff() must wipe user notes but leave the expected answer intact.
    controller.clearStaff();
    EXPECT_FALSE(controller.hasNote(0, 0));
    EXPECT_TRUE(controller.hasExpectedNote(0, 0));

    filesystem::remove(filename);
}

TEST(GridControllerTest, FreeStaffBypassesQuestionRestrictions) {
    // Scale question only allows length 2 (quarter) on even beats.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled\n"
        "C major scale,\"(0-C4 2-D4 4-E4 6-F4 8-G4 10-A4 12-B4 14-C5)\",2,\"0 2 4 6 8 10 12 14\",false,true\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    GridController controller;
    controller.loadQuestion(1);

    // Length 1 (eighth) is not in allowedLengths {2} → must be rejected.
    controller.setNote(0, 0, 0, 1);
    EXPECT_FALSE(controller.hasNote(0, 0));

    // Free Staff bypasses CSV restrictions; the same placement must succeed.
    controller.setFreeStaffMode(true);
    controller.setNote(0, 0, 0, 1);
    EXPECT_TRUE(controller.hasNote(0, 0));

    filesystem::remove(filename);
}

TEST(GridControllerTest, RhythmOverlapClearsExistingNote) {
    // Question contents don't matter — Free Staff bypasses the restrictions.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled\n"
        "Free placement,\"(0-C4)\",2,0,false,false\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    GridController controller;
    controller.loadQuestion(1);
    controller.setFreeStaffMode(true);

    // Place a quarter (length 2) at beat 2.
    controller.setNote(2, 0, 0, 2);
    ASSERT_TRUE(controller.hasNote(2, 0));
    ASSERT_EQ(controller.noteLengthForBeat(2), 2);

    // Place a half (length 3) at beat 1 — its range [1..3] overlaps the quarter.
    // The quarter must no longer exist as a note start at beat 2.
    controller.setNote(1, 0, 0, 3);

    EXPECT_TRUE(controller.hasNote(1, 0));
    EXPECT_EQ(controller.noteLengthForBeat(1), 3);
    EXPECT_EQ(controller.noteLengthForBeat(2), 0); // no phantom quarter start

    filesystem::remove(filename);
}

TEST(GridControllerTest, StackingAllowsChordInFreeStaff) {
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled\n"
        "Free placement,\"(0-C4)\",4,0,false,false\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    GridController controller;
    controller.loadQuestion(1);
    controller.setFreeStaffMode(true);

    // Three whole notes at beat 0, same length, different rows — a chord.
    // Same start AND same length means clearOverlappingNotes must keep all three.
    controller.setNote(0, 0, 0, 4); // C4
    controller.setNote(0, 2, 0, 4); // E4
    controller.setNote(0, 4, 0, 4); // G4

    EXPECT_TRUE(controller.hasNote(0, 0));
    EXPECT_TRUE(controller.hasNote(0, 2));
    EXPECT_TRUE(controller.hasNote(0, 4));

    filesystem::remove(filename);
}
