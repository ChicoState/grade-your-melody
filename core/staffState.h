#pragma once
#include <optional>

// Represents a single note toggle event from the staff.
struct StaffState {
    std::optional<int> col;            // which column was toggled
    std::optional<int> row;            // which row was toggled
    bool               added = false;  // true=placed, false=removed
};
