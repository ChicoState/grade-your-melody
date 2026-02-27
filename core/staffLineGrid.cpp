#include "staffLineGrid.h"

StaffLineGrid::StaffLineGrid(){
    locked = false;
}

StaffLineGrid::~StaffLineGrid(){

}

/// @brief Check if a specific spot on the grid holds a note.
/// @param column Column to check
/// @param row Row to check
/// @return true if has note, false if no note or invalid position
bool StaffLineGrid::HasNote(const int column, const int row){
    if (!ValidPosition(column, row)) return false;

    return grid[column][row];
}

/// @brief Adds a note to a specific spot on the grid.
/// @param column Column of added note
/// @param row Row of added note
void StaffLineGrid::AddNote(const int column, const int row){
    if (!ValidPosition(column, row)) return;

    if (locked) return;

    grid[column][row] = true;
}

/// @brief Removes a note from a specific spot on the grid.
/// @param column Column of removed note
/// @param row Row of added note
void StaffLineGrid::RemoveNote(const int column, const int row){
    if (!ValidPosition(column, row)) return;

    if (locked) return;

    grid[column][row] = false;
}

/// @brief Clears the grid setting all notes to false
void StaffLineGrid::ClearGrid(){
    if (locked) return;

    for(auto& column : grid){
        for(auto& row : column){
            row = false;
        }
    }
}

/// @brief Locks the grid, preventing changes
void StaffLineGrid::LockGrid(){
    locked = true;
}

/// @brief Unlocks the grid, allowing changes
void StaffLineGrid::UnlockGrid(){
    locked = false;
}

/// @brief Checks the locked status of the grid.
/// @return True if locked, false if not
bool StaffLineGrid::GetLockedStatus(){
    return locked;
}

/// @brief Checks if a specified position is valid on the grid.
/// @param column Column to check
/// @param row Row to check
/// @return True if valid position, false if not
bool StaffLineGrid::ValidPosition(const int column, const int row){
    if (column < columns && column >= 0 && row < rows && row >= 0)
        return true;
    else
        return false;
}
