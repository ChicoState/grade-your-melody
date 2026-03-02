#pragma once
#include "staffLineGrid.h"
#include <string>

struct PresenterResult {
    bool success;
    std::string message;
};

class MelodyPresenter {
public:
    explicit MelodyPresenter(StaffLineGrid& grid);

    PresenterResult toggleNote(int col, int row);
    bool hasNote(int col, int row) const;

private:
    StaffLineGrid& grid;
};
