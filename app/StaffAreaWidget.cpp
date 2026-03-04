#include "StaffAreaWidget.h"
#include "StaffWidget.h"
#include "StaffGeometry.h"
#include "ClefWidget.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QResizeEvent>

StaffAreaWidget::StaffAreaWidget(QWidget* parent)
    : QWidget(parent)
{
    clef_  = new ClefWidget(this);
    staff_ = new StaffWidget(this);

    // Initial width; resizeEvent keeps this proportional to height.
    clef_->setFixedWidth(52);

    auto* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(clef_);
    layout->addWidget(staff_, /*stretch=*/1);
}

void StaffAreaWidget::resizeEvent(QResizeEvent* e) {
    QWidget::resizeEvent(e);
    // Scale clef width proportional to height using the SVG aspect ratio.
    clef_->setFixedWidth(qRound(height() * ClefWidget::kAspectRatio));
}

void StaffAreaWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);

    // White base for the entire area (clef gutter + staff).
    p.fillRect(rect(), Qt::white);

    // Draw staff lines from the left edge to the bar line.
    const int cellH    = staff_->cellHeight();
    const int lineEndX = staff_->x() + staff_->barX();
    p.setPen(QPen(Qt::black, 1.5));
    for (int row : StaffGeometry::lineRows) {
        int y = StaffGeometry::staffTopY() + (StaffGeometry::ROWS - 1 - row) * cellH;
        p.drawLine(0, y, lineEndX, y);
    }
}
