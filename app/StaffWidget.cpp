#include "StaffWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>
#include <cmath>

void StaffWidget::updateLayout() {
    static constexpr int rightPad = 20;  // fixed gap between bar line and right edge

    int availH = height() - 2 * marginY;
    cellH = std::min(28, std::max(10, availH / (ROWS - 1)));

    // Bar line is pinned near the right edge; columns span [marginX, barX_].
    barX_ = std::max(marginX + COLS * 20, width() - rightPad);
    colW = std::max(20.0, static_cast<double>(barX_ - marginX) / COLS);
}

StaffWidget::StaffWidget(QWidget* parent)
    : QWidget(parent), grid(), presenter(grid)
{
    setAutoFillBackground(false);
    setMinimumSize(marginX + COLS * 40 + 30, marginY + (ROWS - 1) * 10 + marginY);
    resize(marginX + qRound(COLS * colW) + 30, marginY + (ROWS - 1) * cellH + marginY);
    setWindowTitle("Grade Your Melody");
}

void StaffWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    updateLayout();
    update();
}

void StaffWidget::gridToPixel(int col, int row, int& cx, int& cy) const {
    cx = qRound(marginX + (col + 0.5) * colW);
    cy = marginY + (ROWS - 1 - row) * cellH;
}

bool StaffWidget::pixelToGrid(int x, int y, int& col, int& row) const {
    // Reject clicks outside the interactive measure area.
    if (x < marginX) return false;
    if (x >= barX_)  return false;

    // Snap y to the nearest row
    float rowF = (float)(marginY + (ROWS - 1) * cellH - y) / cellH;
    row = (int)std::round(rowF);
    col = static_cast<int>((x - marginX) / colW);

    if (row < 0 || row >= ROWS) return false;
    if (col < 0 || col >= COLS) return false;
    return true;
}

const char* StaffWidget::noteName(int row) {
    static const char* names[11] = {
        "C4", "D4", "E4", "F4", "G4",
        "A4", "B4", "C5", "D5", "E5", "F5"
    };
    if (row < 0 || row > 10) return "";
    return names[row];
}

void StaffWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // Note head: oval tilted -20°, scaled to nearly fill one staff space.
    static constexpr double kAspect = 1.35;
    const double staffStep = cellH;
    const double noteHpx = std::max(6.0,
        std::min(staffStep * 1.8,           // nearly fills one space (line→space gap)
                 colW     * 0.92 / kAspect)); // hard cap: must fit inside column
    const double noteWpx = noteHpx * kAspect;
    const int    ledgerHalf = std::max((int)(colW / 4) + 3, 6);

    int staffLeft = marginX;

    // Faint C4 ledger line stub (always visible as a position reference)
    {
        int cx, cy;
        QPen faintPen(QColor(180, 180, 180), 1);
        p.setPen(faintPen);
        for (int col = 0; col < COLS; ++col) {
            gridToPixel(col, 0, cx, cy);
            p.drawLine(cx - ledgerHalf, cy, cx + ledgerHalf, cy);
        }
    }

    // Beat column dividers (light guides)
    {
        QPen divPen(QColor(220, 220, 220), 1, Qt::DashLine);
        p.setPen(divPen);
        for (int col = 1; col < COLS; ++col) {
            int x = qRound(staffLeft + col * colW);
            int topCy, botCy, dummy;
            gridToPixel(0, 10, dummy, topCy);
            gridToPixel(0, 0,  dummy, botCy);
            p.drawLine(x, topCy, x, botCy);
        }
    }

    // Bar line at the measure end.
    {
        int topY, botY, dummy;
        gridToPixel(0, StaffGeometry::lineRows[4], dummy, topY); // top staff line (row 10)
        gridToPixel(0, StaffGeometry::lineRows[0], dummy, botY); // bottom staff line (row 2)
        p.setPen(QPen(Qt::black, 1.5));
        p.drawLine(barX_, topY, barX_, botY);
    }

    // Note heads — per-column pass so adjacent pairs can be offset horizontally.
    for (int col = 0; col < COLS; ++col) {
        // Collect occupied rows for this column, ascending (low pitch → high pitch).
        int occupied[ROWS];
        int numOcc = 0;
        for (int row = 0; row < ROWS; ++row)
            if (presenter.hasNote(col, row)) occupied[numOcc++] = row;
        if (numOcc == 0) continue;

        // Assign zigzag horizontal offsets to adjacent-step note clusters.
        double dx[ROWS] = {};
        for (int i = 0; i < numOcc; ) {
            // Extend cluster while each consecutive pair is exactly 1 step apart.
            int end = i;
            while (end + 1 < numOcc && occupied[end + 1] - occupied[end] == 1)
                ++end;

            if (end > i) {  // cluster of 2 or more
                // Half-width + 5% padding to clear the rotated oval edge.
                const double dxMag = noteWpx * 0.85;
                for (int j = 0, sz = end - i + 1; j < sz; ++j)
                    dx[occupied[i + j]] = (j % 2 == 0) ? 0.0 : dxMag;
            }
            i = end + 1;
        }

        for (int i = 0; i < numOcc; ++i) {
            const int row = occupied[i];
            int cx, cy;
            gridToPixel(col, row, cx, cy);

            // Ledger line stays at original cx — never moved by the notehead offset.
            if (row == 0) {
                p.setPen(QPen(Qt::black, 1.5));
                p.drawLine(cx - ledgerHalf, cy, cx + ledgerHalf, cy);
            }

            // Notehead: translate to (cx + offset, cy) then rotate.
            p.save();
            p.translate(cx + dx[row], cy);
            p.rotate(-20.0);
            p.setPen(Qt::NoPen);
            p.setBrush(Qt::black);
            p.drawEllipse(QRectF(-noteWpx / 2.0, -noteHpx / 2.0, noteWpx, noteHpx));
            p.restore();
        }
    }
}

void StaffWidget::mousePressEvent(QMouseEvent* event) {
    int col, row;
    if (pixelToGrid(event->x(), event->y(), col, row)) {
        bool wasPresent = presenter.hasNote(col, row);
        presenter.toggleNote(col, row);
        update();
        StaffState state;
        state.col   = col;
        state.row   = row;
        state.added = !wasPresent;
        emit noteToggled(state);
    }
}

void StaffWidget::clearNotes() {
    grid.ClearGrid();
    update();
}
