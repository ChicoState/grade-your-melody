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
    if (questions.empty()) return Question();
    if (question <= 0 || question > (int)questions.size()) return Question();
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

    
    //ifstream file(QUESTION_CSV_FILE);

        std::ifstream file("questions.csv");
    if (!file.is_open()) file.open("../questions.csv");
    if (!file.is_open()) file.open("/app/questions.csv");
    if (!file.is_open()) file.open("/app/build/questions.csv");
    

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
            while (getline(cs, noteHolder, ' ')) {
                 
                if (noteHolder.front() == '(') noteHolder.erase(0, 1);
                if (noteHolder.back() == ')') noteHolder.pop_back();


                NoteInfo note;
                stringstream ns(noteHolder);
             
                string part;

                char dash; //dummy var
                ns >> note.beat >> dash >> note.row >> dash >> note.accent;
                questionData.notes.push_back(note);


            }
        questions.push_back(questionData);
    }

    bHasReadCSV = true;
}
