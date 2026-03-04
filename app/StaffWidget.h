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

    // Geometry accessors used by StaffAreaWidget for drawing staff lines.
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

    // Constants from StaffGeometry
    static constexpr int ROWS    = StaffGeometry::ROWS;
    static constexpr int COLS    = StaffGeometry::kMeasureColumns;
    static constexpr int marginX = 10;  // small left padding before the first beat column
    static constexpr int marginY = StaffGeometry::marginY;

    // Layout values recomputed on every resize
    int    cellH = 18;
    double colW  = 80.0;  // float so columns span exactly [marginX, barX_]
    int    barX_ = 0;     // bar line x position (widget coords); defines measure end

    void updateLayout();

    // Maps a pixel position to a grid cell; returns false if out of bounds.
    bool pixelToGrid(int x, int y, int& col, int& row) const;

    // Maps a grid cell to its pixel center.
    void gridToPixel(int col, int row, int& cx, int& cy) const;

    // Returns the note name for a given row index.
    static const char* noteName(int row);
};
