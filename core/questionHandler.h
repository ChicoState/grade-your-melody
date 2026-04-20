/*
Author: Julian Kroeger-Miller
Last Edited: 3/23/26
Summary: Class used for reading in CSV file containing questions 
    and answers for the quiz.
*/

#pragma once

#include <string>
#include <vector>

#define QUESTION_CSV_FILE "questions.csv"

using namespace std;

struct NoteInfo {
    int beat;
    int row;
    int accent;

    vector<int> allowedLengths;
    vector<int> allowedColumns;
    bool allowStacking = false;
    bool requireAllFilled = false;
};

struct Question {
    int num = 0;
    string questionText = "Invalid Question";
    vector<NoteInfo> notes;

    vector<int> allowedLengths;
    vector<int> allowedStartColumns;
    bool allowStacking = false;
    bool requireAllFilled = false;
};

class QuestionHandler {
public:
    QuestionHandler();
    ~QuestionHandler();

    Question GetQuestion(const int question);
    vector<Question> GetQuestions();

private:
    void ReadCSV();
    
    vector<Question> questions;
    bool bHasReadCSV;
};
