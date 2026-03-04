#pragma once
#include <QWidget>

// Keeps a child widget at a fixed aspect ratio, letterboxing extra space.
class AspectRatioWidget : public QWidget {
    Q_OBJECT

public:
    // ratio is width / height (e.g. 4.0/3.0)
    AspectRatioWidget(QWidget* child, double ratio, QWidget* parent = nullptr);

protected:
    void resizeEvent(QResizeEvent*) override;
    void paintEvent(QPaintEvent*)  override;

private:
    QWidget* child_;
    double   ratio_;  // width / height

    void repositionChild();
};
