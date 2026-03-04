#pragma once

// Shared staff layout constants for StaffAreaWidget and StaffWidget.
struct StaffGeometry {
    static constexpr int ROWS            = 11;  // rows 0 (C4) .. 10 (F5)
    static constexpr int marginY         = 35;  // top (and bottom) margin in pixels
    static constexpr int lineRows[5]     = {2, 4, 6, 8, 10}; // staff-line row indices
    static constexpr int kMeasureColumns = 4;   // interactive columns per measure

    // Y pixel of the top staff line (row 10 = F5).
    static constexpr int staffTopY() { return marginY; }

    // Pixel span between the top and bottom staff lines for a given cellH.
    static constexpr int staffSpanPx(int cellH) {
        return (lineRows[4] - lineRows[0]) * cellH;
    }

    // Y pixel of the bottom staff line (row 2 = E4).
    static constexpr int staffBottomY(int cellH) { return marginY + staffSpanPx(cellH); }
};
