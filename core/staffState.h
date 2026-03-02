#pragma once
#include <optional>

// Records a single toggle event on the staff.
// col/row are always set when a cell is clicked.
// added: true = note was placed, false = note was removed.
struct StaffState {
    std::optional<int> col;            // which column was toggled
    std::optional<int> row;            // which row was toggled
    bool               added = false;  // true=placed, false=removed
};
