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
    dataList.append(new DataObject("Item 1", "red", "icon1", "url1"));
    dataList.append(new DataObject("Item 2", "green", "icon2", "url2");
    dataList.append(new DataObject("Item 3", "blue", "icon3", "url3"));
    dataList.append(new DataObject("Item 4", "yellow", "icon4", "url4"));
    DataObject * dtSelected = new DataObject();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QString("myModel"), QVariant::fromValue(dataList));
    engine.rootContext()->setContextProperty(QString("dtS"), dtSelected);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    MyClassTest *mct = new MyClassTest();
    mct->startCP();

    return app.exec();
}
