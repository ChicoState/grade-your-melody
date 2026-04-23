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
