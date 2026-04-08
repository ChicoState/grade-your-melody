/*
Author: Julian Kroeger-Miller
Last Edited: 3/23/26
Summary: Class used for reading in CSV file containing questions 
    and answers for the quiz.
*/

#include "questionHandler.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

QuestionHandler::QuestionHandler(){
    bHasReadCSV = false;
}

QuestionHandler::~QuestionHandler(){

}

/// @brief Gets a specific question
/// @param question Question number
/// @return Question if found, default question if not
Question QuestionHandler::GetQuestion(const int question){
    if(!bHasReadCSV) ReadCSV();
    if (questions.size() <= 0) return Question();
    return questions[question - 1];
}

/// @brief Gets all questions
/// @return Vector containing all questions.
vector<Question> QuestionHandler::GetQuestions(){
    if(!bHasReadCSV) ReadCSV();
    return questions;
}

/// @brief Reads in data from the question CSV file. Currently doesn't handle error checking so be specific in the file.
void QuestionHandler::ReadCSV(){
    questions.clear();

    ifstream file(QUESTION_CSV_FILE);

    if(!file.is_open()){
        cerr << "Error opening question CSV file" << endl;
    }

    string line;

    //Skip header line
    getline(file, line);

    //Skip example line
    getline(file, line);

    int numQuestions = 0;

    //Take in question data
    while (getline(file,line)){
        Question questionData;

        numQuestions++;
        questionData.num = numQuestions;

        stringstream ss(line);
        string cell;

        //Get question text
        getline(ss, cell, ',');
        questionData.questionText = cell;

        getline(ss, cell, ',');
        stringstream cs(cell); //Cell stream

        string noteHolder;

        //Get answer key
        while (getline(cs, noteHolder, ' ')){
            NoteInfo note;
            stringstream ns(noteHolder); //Note stream

            //Format of notes expected is (Beat-Row-Accent) (Beat-Row-Accent)...

            ns.ignore(); //Ignore (
            ns >> note.beat;
            ns.ignore(); //Ignore dash
            ns >> note.row;
            ns.ignore(); //Ignore dash
            ns >> note.accent;
            //No need to ignore final )

            questionData.notes.push_back(note);
        }

        questions.push_back(questionData);
    }

    bHasReadCSV = true;
}