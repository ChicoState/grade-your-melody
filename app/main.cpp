#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QObject>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;
  engine.addImportPath("qrc:/qt/qml");
  engine.load(QUrl(QStringLiteral("qrc:/qt/qml/GradeYourMelodyUI/App.qml")));
  if (engine.rootObjects().isEmpty())
    return -1;
  return app.exec();
}
