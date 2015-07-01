#include <QApplication>
#include <QQmlApplicationEngine>

#include "myclasstest.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    MyClassTest *mct = new MyClassTest();
    mct->startCP();
    mct->compress("/home/larissa/UFAL/Compelab/git/brisa-project/Apps/BCU/teste",
                  "/home/larissa/UFAL/Compelab/git/brisa-project/Apps/BCU/teste.compe");
    mct->uncompress("/home/larissa/UFAL/Compelab/git/brisa-project/Apps/BCU/teste.compe",
                  "/home/larissa/UFAL/Compelab/git/brisa-project/Apps/BCU/teste2");

    return app.exec();
}
