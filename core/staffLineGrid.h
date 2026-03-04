#pragma once

// Grid size constants
#define columnsNum 4
#define rowsNum 11

class StaffLineGrid {
public:
    StaffLineGrid();
    ~StaffLineGrid();

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