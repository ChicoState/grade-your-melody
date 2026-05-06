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

TEST(QuestionHandlerTest, ChordWithFlatsParsesDistinctly) {
    // Regression: C minor chord (with Eb4) must NOT parse the same as C major chord.
    // Each stacked note's accidental must survive into m_expectedNotes.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled,ChoiceA,ChoiceB,CorrectChoice\n"
        "C major chord,\"(0-C4 0-E4 0-G4)\",4,0,true,false,Major chord,Minor chord,A\n"
        "C minor chord,\"(0-C4 0-Eb4 0-G4)\",4,0,true,false,Major chord,Minor chord,B\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    QuestionHandler handler;
    vector<Question> questions = handler.GetQuestions();

    ASSERT_EQ(questions.size(), 2u);

    // C major chord: C E G — all naturals
    ASSERT_EQ(questions[0].notes.size(), 3u);
    EXPECT_EQ(questions[0].notes[1].row,    2);  // E4
    EXPECT_EQ(questions[0].notes[1].accent, 0);  // natural
    EXPECT_EQ(questions[0].correctChoice,  "A");

    // C minor chord: C Eb G — middle note is flat
    ASSERT_EQ(questions[1].notes.size(), 3u);
    EXPECT_EQ(questions[1].notes[0].row,    0);  // C4
    EXPECT_EQ(questions[1].notes[0].accent, 0);
    EXPECT_EQ(questions[1].notes[1].row,    2);  // Eb4 → row of E
    EXPECT_EQ(questions[1].notes[1].accent, -1); // ← flat
    EXPECT_EQ(questions[1].notes[2].row,    4);  // G4
    EXPECT_EQ(questions[1].notes[2].accent, 0);
    EXPECT_EQ(questions[1].correctChoice,  "B");

    filesystem::remove(filename);
}

TEST(QuestionHandlerTest, EarTrainingChoiceTextRoundTrip) {
    // Verify that ChoiceA, ChoiceB, and CorrectChoice from a 9-column CSV row
    // make it into the parsed Question unchanged.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled,ChoiceA,ChoiceB,CorrectChoice\n"
        "Write a C minor chord,\"(0-C4 0-Eb4 0-G4)\",4,0,true,false,Major chord,Minor chord,B\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    QuestionHandler handler;
    vector<Question> questions = handler.GetQuestions();

    ASSERT_EQ(questions.size(), 1u);
    EXPECT_EQ(questions[0].choiceA,       "Major chord");
    EXPECT_EQ(questions[0].choiceB,       "Minor chord");
    EXPECT_EQ(questions[0].correctChoice, "B");

    filesystem::remove(filename);
}

TEST(QuestionHandlerTest, LegacySixColumnDefaultsChoices) {
    // Backward compatibility: a 6-column CSV (no ChoiceA/ChoiceB/CorrectChoice)
    // must still parse, and the missing fields must fall back to safe defaults.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled\n"
        "Legacy question,\"(0-C4)\",2,0,false,false\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    QuestionHandler handler;
    vector<Question> questions = handler.GetQuestions();

    ASSERT_EQ(questions.size(), 1u);
    EXPECT_EQ(questions[0].choiceA,       "Legacy question"); // defaults to questionText
    EXPECT_EQ(questions[0].choiceB,       "");
    EXPECT_EQ(questions[0].correctChoice, "A");

    filesystem::remove(filename);
}

TEST(QuestionHandlerTest, MalformedRowsAreSkipped) {
    // A row with fewer than 6 fields must be skipped without crashing,
    // and valid rows on either side of it must still parse normally.
    const string filename = "questions.csv";
    const string data =
        "Question,Answer,AllowedLengths,AllowedStartColumns,AllowStacking,RequireAllFilled\n"
        "First valid question,\"(0-C4)\",2,0,false,false\n"
        "broken,row,only-three\n"
        "Second valid question,\"(0-D4)\",2,0,false,false\n";

    {
        ofstream out(filename);
        ASSERT_TRUE(out.is_open()) << "Unable to create temporary CSV file: " << filename;
        out << data;
    }

    QuestionHandler handler;
    vector<Question> questions = handler.GetQuestions();

    ASSERT_EQ(questions.size(), 2u);
    EXPECT_EQ(questions[0].questionText, "First valid question");
    EXPECT_EQ(questions[1].questionText, "Second valid question");

    filesystem::remove(filename);
}
