#include "presenter.h"

MelodyPresenter::MelodyPresenter(StaffLineGrid& grid) : grid(grid) {}

PresenterResult MelodyPresenter::toggleNote(int col, int row) {
    if (grid.HasNote(col, row)) {
        grid.RemoveNote(col, row);
        return {true, "Note removed"};
    }
    grid.AddNote(col, row);
    return {true, "Note added"};
}

bool MelodyPresenter::hasNote(int col, int row) const {
    return grid.HasNote(col, row);
}
