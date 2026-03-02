#pragma once
#include <QWidget>
#include "staffLineGrid.h"
#include "presenter.h"
#include "staffState.h"
#include "StaffGeometry.h"

class StaffWidget : public QWidget {
    Q_OBJECT

public:
    explicit StaffWidget(QWidget* parent = nullptr);

    // Exposed so StaffAreaWidget can draw staff lines at the same y positions
    // and truncate them at the bar line.
    int cellHeight() const { return cellH; }
    int barX()       const { return barX_; }  // bar line x in widget coords

public slots:
    void clearNotes();  // resets all placed notes and repaints

signals:
    void noteToggled(StaffState state);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    StaffLineGrid grid;
    MelodyPresenter presenter;

    // Fixed logical constants — ROWS, marginY, and COLS come from StaffGeometry
    static constexpr int ROWS    = StaffGeometry::ROWS;
    static constexpr int COLS    = StaffGeometry::kMeasureColumns;
    static constexpr int marginX = 10;  // small left padding before the first beat column
    static constexpr int marginY = StaffGeometry::marginY;

    // Dynamic layout values — recomputed on every resize
    int    cellH = 18;
    double colW  = 80.0;  // float so columns span exactly [marginX, barX_]
    int    barX_ = 0;     // bar line x position (widget coords); defines measure end

    void updateLayout();

    // Convert a pixel position to a grid (col, row).  Returns false if out of bounds.
    bool pixelToGrid(int x, int y, int& col, int& row) const;

    // Convert a grid position to the pixel center of that cell.
    void gridToPixel(int col, int row, int& cx, int& cy) const;

    // Human-readable note name for a row index.
    static const char* noteName(int row);
};
