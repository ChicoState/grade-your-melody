#include "AspectRatioWidget.h"
#include <QPainter>
#include <QResizeEvent>

AspectRatioWidget::AspectRatioWidget(QWidget* child, double ratio, QWidget* parent)
    : QWidget(parent), child_(child), ratio_(ratio)
{
    child_->setParent(this);
    child_->show();
}

void AspectRatioWidget::repositionChild() {
    const int w = width();
    const int h = height();

    int childW, childH;
    if (static_cast<double>(w) / h > ratio_) {
        // Wider than target: constrained by height
        childH = h;
        childW = qRound(h * ratio_);
    } else {
        // Taller than target: constrained by width
        childW = w;
        childH = qRound(w / ratio_);
    }

    // Center in the available space (letterbox margins fill the rest)
    const int x = (w - childW) / 2;
    const int y = (h - childH) / 2;
    child_->setGeometry(x, y, childW, childH);
}

void AspectRatioWidget::resizeEvent(QResizeEvent* e) {
    QWidget::resizeEvent(e);
    repositionChild();
}

void AspectRatioWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    // Letterbox bars — neutral dark fill so they read as borders, not content
    p.fillRect(rect(), Qt::white);
}
