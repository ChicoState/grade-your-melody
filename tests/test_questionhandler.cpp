#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

#include "questionHandler.h"

using namespace std;

TEST(QuestionHandlerTest, ReadsCsvAndCreatesQuestions) {
    // Write a minimal 6-column CSV matching the current parser format.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled\n"
        "Test first question,\"(1-2-0 2-3-1)\",1 2,0 2,false,false\n"
        "Test second question,\"(4-5-0)\",1,0,false,false\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    QuestionHandler handler;
    vector<Question> questions = handler.GetQuestions();

    EXPECT_EQ(questions.size(), 2u);

    EXPECT_EQ(questions[0].num, 1);
    EXPECT_EQ(questions[0].questionText, "Test first question");
    ASSERT_EQ(questions[0].notes.size(), 2u);
    EXPECT_EQ(questions[0].notes[0].beat, 1);
    EXPECT_EQ(questions[0].notes[0].row, 2);
    EXPECT_EQ(questions[0].notes[0].accent, 0);
    EXPECT_EQ(questions[0].notes[1].beat, 2);
    EXPECT_EQ(questions[0].notes[1].row, 3);
    EXPECT_EQ(questions[0].notes[1].accent, 1);

    EXPECT_EQ(questions[1].num, 2);
    EXPECT_EQ(questions[1].questionText, "Test second question");
    ASSERT_EQ(questions[1].notes.size(), 1u);
    EXPECT_EQ(questions[1].notes[0].beat, 4);
    EXPECT_EQ(questions[1].notes[0].row, 5);
    EXPECT_EQ(questions[1].notes[0].accent, 0);

    // Clean up.
    filesystem::remove(filename);
}

TEST(QuestionHandlerTest, PitchNameParsing) {
    // Verify that pitch-name tokens like "0-F4" and "6-Bb4" are parsed correctly
    // and produce the expected diatonic row and accidental values.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled\n"
        "F major scale,\"(0-F4 2-G4 6-Bb4 8-C5)\",2,\"0 2 6 8\",false,false\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    QuestionHandler handler;
    vector<Question> questions = handler.GetQuestions();

    ASSERT_EQ(questions.size(), 1u);
    ASSERT_EQ(questions[0].notes.size(), 4u);

    // F4: diatonic offset 3, octave 4 → row = (4-4)*7 + 3 = 3, accent = 0
    EXPECT_EQ(questions[0].notes[0].beat,   0);
    EXPECT_EQ(questions[0].notes[0].row,    3);
    EXPECT_EQ(questions[0].notes[0].accent, 0);

    // G4: diatonic offset 4, octave 4 → row = 4, accent = 0
    EXPECT_EQ(questions[0].notes[1].beat,   2);
    EXPECT_EQ(questions[0].notes[1].row,    4);
    EXPECT_EQ(questions[0].notes[1].accent, 0);

    // Bb4: B = offset 6, flat → row = 6, accent = -1
    EXPECT_EQ(questions[0].notes[2].beat,   6);
    EXPECT_EQ(questions[0].notes[2].row,    6);
    EXPECT_EQ(questions[0].notes[2].accent, -1);

    // C5: diatonic offset 0, octave 5 → row = (5-4)*7 + 0 = 7, accent = 0
    EXPECT_EQ(questions[0].notes[3].beat,   8);
    EXPECT_EQ(questions[0].notes[3].row,    7);
    EXPECT_EQ(questions[0].notes[3].accent, 0);

    filesystem::remove(filename);
}
