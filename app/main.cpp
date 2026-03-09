#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "GridController.h"
#include <QResource>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    Q_INIT_RESOURCE(app_resources);
    QQmlApplicationEngine engine;

    GridController controller;
    engine.rootContext()->setContextProperty("gridController", &controller);

    // Example: set the correct answer (you can do this elsewhere too)
    //controller.setExpectedRow(0, 4);
    //controller.setExpectedRow(1, 4);
    //controller.setExpectedRow(2, 5);
    //controller.setExpectedRow(3, 4);
    for (int i = 0; i < 16; i++) {
    controller.setExpectedRow(i, 4);
}

    const QUrl url(u"qrc:/app/App.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

qDebug() << "exists :/App.qml =" << QFile(":/App.qml").exists();
qDebug() << "exists :/app/App.qml =" << QFile(":/app/App.qml").exists();
    engine.load(url);

    return app.exec();
}
