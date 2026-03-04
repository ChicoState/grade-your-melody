#pragma once
#include <QWidget>
#include <QSvgRenderer>

// Renders the treble clef SVG, scaled to fit the allocated widget area.
class ClefWidget : public QWidget {
    Q_OBJECT

public:
    explicit ClefWidget(QWidget* parent = nullptr);

    // SVG aspect ratio (width / height) derived from its viewBox.
    static constexpr double kAspectRatio = 95.116 / 153.12;

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QSvgRenderer renderer_;
};
