#include "staffLineGrid.h"

StaffLineGrid::StaffLineGrid(){
    locked = false;
    ClearGrid();
}

StaffLineGrid::~StaffLineGrid(){

}

/// @brief Check if a specific spot on the grid holds a note.
/// @param column Column to check
/// @param row Row to check
/// @return true if has note, false if no note or invalid position
bool StaffLineGrid::HasNote(const int column, const int row) const {
    if (!ValidPosition(column, row)) return false;

    return grid[column][row] != nullptr;
}

/// @brief Adds a note to a specific spot on the grid.
/// @param column Column of added note
/// @param row Row of added note
void StaffLineGrid::AddNote(const int column, const int row){
    if (!ValidPosition(column, row)) return;

    if (locked) return;

    std::shared_ptr<Note> note = std::make_shared<Note>();
    note->row = row;
    note->column = column;

    grid[column][row] = note;
}

/// @brief Adds a note of specified length to the grid starting at a specific spot.
/// @param column Column of added note
/// @param row Row of added note
/// @param length Length of the note in beats
void StaffLineGrid::AddNote(const int column, const int row, int length)
{
    if (column + length > columns) return; // Check if the note would go out of bounds

    if (locked) return;

    std::shared_ptr<Note> note = std::make_shared<Note>();
    note->length = length;
    note->row = row;
    note->column = column;

    for (int i = 0; i < length; i++) {
        if (ValidPosition(column + i, row)) {
            grid[column + i][row] = note;
        }
    }
}

/// @brief Removes a note from a specific spot on the grid.
/// @param column Column of removed note
/// @param row Row of added note
void StaffLineGrid::RemoveNote(const int column, const int row){
    if (!ValidPosition(column, row)) return;

    if (locked) return;

    std::shared_ptr<Note> note = grid[column][row];

    if(note == nullptr || !ValidPosition(note->column, note->row)) return;

    int length = note->length;
    for (int i = 0; i < length; i++) {
        if (ValidPosition(note->column + i, note->row) && grid[note->column + i][note->row] == note) {
            grid[note->column + i][note->row] = nullptr;
        }
    }
}

/// @brief Clears the grid setting all notes to false
void StaffLineGrid::ClearGrid(){
    if (locked) return;

    for(auto& column : grid){
        for(auto& cell : column){
            cell = nullptr;
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
bool StaffLineGrid::ValidPosition(const int column, const int row) const {
    if (column < columns && column >= 0 && row < rows && row >= 0)
        return true;
    else
        return false;
}
