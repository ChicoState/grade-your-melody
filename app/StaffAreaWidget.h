#pragma once
#include <QWidget>

class StaffWidget;
class ClefWidget;

// Container that draws the 5 staff lines across its full width (so they appear
// behind both the ClefWidget on the left and StaffWidget on the right).
class StaffAreaWidget : public QWidget {
    Q_OBJECT

public:
    explicit StaffAreaWidget(QWidget* parent = nullptr);

    // Exposes the inner StaffWidget so MainWindow can wire signals.
    StaffWidget* staff() const { return staff_; }

protected:
    void paintEvent(QPaintEvent*) override;
    // Updates clef's fixed width proportional to height so it scales with the staff.
    void resizeEvent(QResizeEvent*) override;

private:
    StaffWidget* staff_;
    ClefWidget*  clef_;
};
