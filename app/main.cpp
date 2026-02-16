#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QLabel label("Grade Your Melody (Qt) — Docker build OK");
  label.resize(360, 80);
  label.show();
  return app.exec();
}
