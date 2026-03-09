/*
Author: Julian Kroeger-Miller
Last Edited: 3/8/26
Summary: Data based grid of the staff lines. Currently handles if a note is at
    a specified location or not. Does not use any sort of note struct.
*/

#pragma once

//Used for easy resizing
#define columnsNum 16
#define rowsNum 9 //36 slots 4 beats per measure

class StaffLineGrid {
public:
    //Constructor
    StaffLineGrid();
    ~StaffLineGrid();

    //Methods
    bool HasNote(const int column, const int row) const;
    void AddNote(const int column, const int row);
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
    bool grid[columnsNum][rowsNum] = {};


};
