#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "core/GridController.h"
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    GridController controller;
    engine.rootContext()->setContextProperty("gridController", &controller);

    for (int i = 0; i < 16; i++) {
        controller.setExpectedRow(i, 4, -1);
    }

    const QUrl url(u"qrc:/qt/qml/GradeYourMelodyUI/App.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    qDebug() << "exists :/qt/qml/GradeYourMelodyUI/App.qml ="
             << QFile(":/qt/qml/GradeYourMelodyUI/App.qml").exists();

    engine.load(url);

    return app.exec();
}