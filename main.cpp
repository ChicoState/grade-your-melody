#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
//main widget to draw the staff lines and notes
//
class StaffWidget : public QWidget {
public:
    StaffWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setMinimumSize(600, 300);
    }

    void paintEvent(QPaintEvent *event) override {
        (void)event;
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);

        const int leftMargin  = 40;
        const int rightMargin = 40;
        const int staffWidth  = width() - leftMargin - rightMargin;

        const int staffCenterY = height() / 2;  // center the staff vertically
        const int lineSpacing  = 12;            // distance between staff lines
        const int staffLines   = 5;



        // sstaff line positions 5 lines
        const int topLineY = staffCenterY - (staffLines - 1) * lineSpacing / 2;
        const int x1 = leftMargin;
        const int x2 = leftMargin + staffWidth;

        // draw the actual lines
        for (int i = 0; i < staffLines; ++i) {
            const int y = topLineY + i * lineSpacing;
            p.drawLine(x1, y, x2, y);
        }

         
        auto drawLedgerLine = [&](int y, int widthPx) {
            const int noteX = width() / 2;
            const int halfW = widthPx / 2;
            p.drawLine(noteX - halfW, y, noteX + halfW, y);
        };
    //define the amount of musical steps
        const int step = lineSpacing / 2;

        //top and bottom staff line
        const int staffTopY    = topLineY;
        const int staffBottomY = topLineY + (staffLines - 1) * lineSpacing;

        // ledger lines above the staff
        for (int k = 1; k <= 3; ++k) {
            const int y = staffTopY - (2 * k) * step; // go up by 2 steps 
            drawLedgerLine(y, 70);
        }

        //ledger lines below the staff 
        for (int k = 1; k <= 3; ++k) {
            const int y = staffBottomY + (2 * k) * step; // go down by 2
            drawLedgerLine(y, 70);
        }
//draw a circle above or below
        p.setBrush(Qt::NoBrush);
        const int noteX = width() / 2;
        p.drawEllipse(QPoint(noteX, staffTopY - 4 * step), 6, 4);    // above
        p.drawEllipse(QPoint(noteX, staffBottomY + 4 * step), 6, 4); // below
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow window;
    
    window.setWindowTitle("Ledger");
    
    window.resize(900, 400);

    auto *staff = new StaffWidget();
    window.setCentralWidget(staff);

    
    window.show();
    return app.exec();
}
