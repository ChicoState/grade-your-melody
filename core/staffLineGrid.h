/*
Author: Julian Kroeger-Miller
Last Edited: 2/27/2026
Summary: Data based grid of the staff lines. Currently handles if a note is at
    a specified location or not. Does not use any sort of note struct.
*/

#pragma once

//Used for easy resizing
#define columnsNum 4
#define rowsNum 11

class StaffLineGrid {
public:
    //Constructor
    StaffLineGrid();
    ~StaffLineGrid();

    //Methods
    bool HasNote(const int column, const int row);
    void AddNote(const int column, const int row);
    void RemoveNote(const int column, const int row);
    void ClearGrid();
    void LockGrid();
    void UnlockGrid();
    bool GetLockedStatus();

private:

    bool ValidPosition(const int column, const int row);

    bool locked;
    bool grid[columnsNum][rowsNum];

    int columns = columnsNum;
    int rows = rowsNum;

};