#include "staffLineGrid.h"

StaffLineGrid::StaffLineGrid(){
    locked = false;
}

StaffLineGrid::~StaffLineGrid(){

}

// Returns true if the given column and row cell contains a note.
bool StaffLineGrid::HasNote(const int column, const int row){
    if (!ValidPosition(column, row)) return false;

    return grid[column][row];
}

// Places a note at the given column and row; ignored if locked.
void StaffLineGrid::AddNote(const int column, const int row){
    if (!ValidPosition(column, row)) return;

    if (locked) return;

    grid[column][row] = true;
}

// Clears the note at the given column and row; ignored if locked.
void StaffLineGrid::RemoveNote(const int column, const int row){
    if (!ValidPosition(column, row)) return;

    if (locked) return;

    grid[column][row] = false;
}

// Resets all cells to false; ignored if locked.
void StaffLineGrid::ClearGrid(){
    if (locked) return;

    for(auto& column : grid){
        for(auto& row : column){
            row = false;
        }
    }
}

// Prevents further note changes.
void StaffLineGrid::LockGrid(){
    locked = true;
}

// Allows note changes again.
void StaffLineGrid::UnlockGrid(){
    locked = false;
}

// Returns true if the grid is locked.
bool StaffLineGrid::GetLockedStatus(){
    return locked;
}

// Returns true if the column and row are within grid bounds.
bool StaffLineGrid::ValidPosition(const int column, const int row){
    if (column < columns && column >= 0 && row < rows && row >= 0)
        return true;
    else
        return false;
}
