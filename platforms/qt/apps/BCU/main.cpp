#include <QQmlContext>
#include <QQuickView>
#include <QApplication>
#include <QQmlApplicationEngine>

#include "myclasstest.h"
#include "dataobject.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QList<QObject*> dataList;
    dataList.append(new DataObject("Item 1", "red"));
    dataList.append(new DataObject("Item 2", "green"));
    dataList.append(new DataObject("Item 3", "blue"));
    dataList.append(new DataObject("Item 4", "yellow"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QString("myModel"), QVariant::fromValue(dataList));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    MyClassTest *mct = new MyClassTest();
    mct->startCP();

    return app.exec();
}
