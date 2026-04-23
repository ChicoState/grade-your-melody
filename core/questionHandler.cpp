/*
Author: Julian Kroeger-Miller
Last Edited: 4/9/26
Summary: Class used for reading in CSV file containing questions 
    and answers for the quiz.
*/

#include "questionHandler.h"

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//f the field is: "0 2 4 6 8 10 12 14" then stringstream reads one integer at a time, separated by spaces. 
//So it becomes: {0, 2, 4, 6, 8, 10, 12, 14}
static vector<int> ParseIntList(const string& field) {
    vector<int> values;
    stringstream ss(field);
    int value;

    while (ss >> value) {
        values.push_back(value);
    }

    return values;
}

//This lets you write any of these in CSV: true True 1 and they all mean true. Everything else becomes false.
static bool ParseBool(const string& field) {
    return field == "true" || field == "True" || field == "1";
}

// If the CSV field is: (0-4-0 2-5-0 4-6-0) it first removes the outer parentheses, leaving: 0-4-0 2-5-0 4-6-0
//Then it reads each token: 0-4-0 2-5-0 4-6-0 and splits each one at the dashes.
static vector<NoteInfo> ParseNotes(const string& field) {
    vector<NoteInfo> notes;

    string cleaned = field;

    if (!cleaned.empty() && cleaned.front() == '(') {
        cleaned.erase(0, 1);
    }
    if (!cleaned.empty() && cleaned.back() == ')') {
        cleaned.pop_back();
    }

    stringstream ss(cleaned);
    string token;

    while (ss >> token) {
        size_t firstDash = token.find('-');
        size_t secondDash = token.find('-', firstDash + 1);

        if (firstDash == string::npos || secondDash == string::npos) {
            continue;
        }

        NoteInfo note;
        note.beat = stoi(token.substr(0, firstDash));
        note.row = stoi(token.substr(firstDash + 1, secondDash - firstDash - 1));
        note.accent = stoi(token.substr(secondDash + 1));

        notes.push_back(note);
    }

    return notes;
}

//For a line like: "Write a C major scale","(0-4-0 2-5-0 4-6-0)","2","0 2 4 6",false,true
//it will correctly produce 6 fields, even though some fields contain spaces.
static vector<string> SplitCSVLine(const string& line) {
    vector<string> fields;
    string current;
    bool inQuotes = false;

    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == ',' && !inQuotes) {
            fields.push_back(current);
            current.clear();
        } else {
            current += ch;
        }
    }

    fields.push_back(current);
    return fields;
}

void QuestionHandler::ReadCSV() {
    if (bHasReadCSV) return;

    ifstream file(QUESTION_CSV_FILE);

    if (!file.is_open()) file.open("questions.csv");
    if (!file.is_open()) file.open("../questions.csv");
    if (!file.is_open()) file.open("core/questions.csv");
    if (!file.is_open()) file.open("../core/questions.csv");
    if (!file.is_open()) file.open("/app/questions.csv");
    if (!file.is_open()) file.open("/app/build/questions.csv");

    if (!file.is_open()) {
        cerr << "ERROR: Could not open questions.csv" << endl;
      return;
    }

    string line;
    bool isHeader = true;
    int questionNum = 0;

    while (getline(file, line)) {
        if (isHeader) {
            isHeader = false;
            continue;
        }

        if (line.empty()) continue;

        vector<string> fields = SplitCSVLine(line);
        if (fields.size() < 6) continue;

        Question q;
        q.num = ++questionNum;
        q.questionText = fields[0];
        q.notes = ParseNotes(fields[1]);

        q.allowedLengths = ParseIntList(fields[2]);
        q.allowedStartColumns = ParseIntList(fields[3]);
        q.allowStacking = ParseBool(fields[4]);
        q.requireAllFilled = ParseBool(fields[5]);

        questions.push_back(q);
    }

    bHasReadCSV = true;
}

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
/*void QuestionHandler::ReadCSV(){
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
                

                //Format of notes expected is (Beat-Row-Accent) (Beat-Row-Accent)...

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
}*/
