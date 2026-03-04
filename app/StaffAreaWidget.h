#pragma once
#include <QWidget>

class StaffWidget;
class ClefWidget;

// Draws the 5 staff lines behind the clef and note areas.
class StaffAreaWidget : public QWidget {
    Q_OBJECT

public:
    explicit StaffAreaWidget(QWidget* parent = nullptr);

    // Exposes the inner StaffWidget so MainWindow can wire signals.
    StaffWidget* staff() const { return staff_; }

protected:
    void paintEvent(QPaintEvent*) override;
    // Keeps clef width proportional to height so it scales with the staff.
    void resizeEvent(QResizeEvent*) override;

private:
    StaffWidget* staff_;
    ClefWidget*  clef_;
};
