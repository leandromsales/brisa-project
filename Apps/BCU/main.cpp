#include <QApplication>
#include <QQmlApplicationEngine>

#include "myclasstest.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

//    MyClassTest *mct = new MyClassTest();
//    mct->startCP();

    return app.exec();
}
