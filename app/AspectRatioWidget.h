#pragma once
#include <QWidget>

// Layout-only container that keeps one child widget at a fixed aspect ratio
// (width / height).  Extra space is filled with a neutral letterbox colour.
// The child is re-parented and its geometry is managed entirely here.
class AspectRatioWidget : public QWidget {
    Q_OBJECT

public:
    // ratio = width / height  (e.g. 4.0/3.0)
    AspectRatioWidget(QWidget* child, double ratio, QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent*) override;
    void paintEvent(QPaintEvent*)  override;

private:
    QWidget* child_;
    double   ratio_;  // width / height

    void repositionChild();
};
