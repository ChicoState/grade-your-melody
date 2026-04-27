/*
Author: Julian Kroeger-Miller
Last Edited: 4/6/26
Summary: Data based grid of the staff lines. Currently handles if a note is at
    a specified location or not. Does not use any sort of note struct.
*/

#pragma once

#include <memory>

//Used for easy resizing
#define columnsNum 16
// Row-to-pitch mapping (diatonic white keys only; accidentals stored separately as -1/0/+1):
//   C4=0  D4=1  E4=2  F4=3  G4=4  A4=5  B4=6
//   C5=7  D5=8  E5=9  F5=10 G5=11 A5=12 B5=13
//   C6=14 D6=15 E6=16 F6=17 G6=18 A6=19 B6=20
//   C7=21 D7=22
#define rowsNum 23 // C4 to D7 range (23 diatonic pitches)

struct Note {
    int column = 0; // Column of the note in the measure
    int row = 0; // Row of the note on the staff
    int length = 1; // Length of the note in beats
};

class StaffLineGrid {
public:
    //Constructor
    StaffLineGrid();
    ~StaffLineGrid();

    //Methods
    bool HasNote(const int column, const int row) const;
    void AddNote(const int column, const int row);
    void AddNote(const int column, const int row, int length);
    void RemoveNote(const int column, const int row);
    void ClearGrid();
    void LockGrid();
    void UnlockGrid();
    bool GetLockedStatus();

    static constexpr int columns = columnsNum;
    static constexpr int rows = rowsNum;
    bool ValidPosition(int column, int row) const;

private:


    bool locked = false;
    std::shared_ptr<Note> grid[columnsNum][rowsNum];


};
