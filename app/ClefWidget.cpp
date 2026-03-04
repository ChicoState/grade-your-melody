#include "ClefWidget.h"
#include <QPainter>

ClefWidget::ClefWidget(QWidget* parent)
    : QWidget(parent), renderer_(QString(":/treble_clef.svg"))
{
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(false);
}

void ClefWidget::paintEvent(QPaintEvent*) {
    if (!renderer_.isValid()) return;
    QPainter p(this);

    // Scale SVG to fill height; clamp to width if it overflows.
    int drawH = height();
    int drawW = qRound(drawH * kAspectRatio);
    if (drawW > width()) {
        drawW = width();
        drawH = qRound(drawW / kAspectRatio);
    }

    // Align to top (matches staff line y=marginY), center horizontally.
    QRect target((width() - drawW) / 2, 0, drawW, drawH);
    renderer_.render(&p, target);
}
