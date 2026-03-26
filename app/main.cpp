#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "GridController.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    GridController controller;
    engine.rootContext()->setContextProperty("gridController", &controller);

    // Example: set the correct answer (you can do this elsewhere too)
    //controller.setExpectedRow(0, 4);
    //controller.setExpectedRow(1, 4);
    //controller.setExpectedRow(2, 5);
    //controller.setExpectedRow(3, 4);
    for (int i = 0; i < 16; i++) {
    controller.setExpectedRow(i, 4, -1);
    
}

    const QUrl url(u"qrc:/qt/qml/GradeYourMelodyUI/App.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
