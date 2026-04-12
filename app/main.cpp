#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QResource>
#include <QFile>
#include <QDebug>
#include <iostream>

#include "GridController.h"
#include "questionHandler.h"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    GridController controller;
    engine.rootContext()->setContextProperty("gridController", &controller);
    
    //Load Questions
    controller.loadQuestion(1);
    // Example: set the correct answer (you can do this elsewhere too)
    //controller.setExpectedRow(0, 4);
    //controller.setExpectedRow(1, 4);
    //controller.setExpectedRow(2, 5);
    //controller.setExpectedRow(3, 4);
   // for (int i = 0; i < 16; i++) {
    //controller.setExpectedRow(i, 4, -1);
    
//}

   
    

    // TODO: hardcoded to question 1 for demo — replace with dynamic question selection from UI
    QuestionHandler qh;
    Question q = qh.GetQuestion(1);
    engine.rootContext()->setContextProperty("questionText", QString::fromStdString(q.questionText));
    const QUrl url(u"qrc:/qt/qml/GradeYourMelodyUI/App.qml"_qs);
    //const QUrl url(u"qrc:/app/App.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
