#pragma once
#include <QWidget>
#include <QSvgRenderer>

// Renders the treble clef SVG with a preserved aspect ratio using QSvgRenderer.
// The parent (StaffAreaWidget) is responsible for setting this widget's fixed width
// proportional to its height via kAspectRatio so the clef scales with the staff.
class ClefWidget : public QWidget {
    Q_OBJECT

public:
    explicit ClefWidget(QWidget* parent = nullptr);

    // SVG natural aspect ratio (width / height) from viewBox="0 0 95.116 153.12".
    static constexpr double kAspectRatio = 95.116 / 153.12;

protected:
    void paintEvent(QPaintEvent*) override;

private:
    QSvgRenderer renderer_;
};
