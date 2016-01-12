#ifndef BRISAAPPLICATIONMANAGER_H
#define BRISAAPPLICATIONMANAGER_H

#include <QQuickView>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "brisaapplication.h"
#include "essencials/qqmlhelpers.h"

class BRisaApplicationManager : public QObject
{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(QList<QObject *>, apps)
    QML_WRITABLE_PROPERTY(int, numOfApps)
    QML_WRITABLE_PROPERTY(QString, currentAppDir)
public:

    BRisaApplicationManager(QQmlApplicationEngine &engine);

    Q_INVOKABLE bool fileExists(QString path);
    Q_INVOKABLE bool createAnApp(QJSValue theApp);
    Q_INVOKABLE void run(QString name);

    void addApp(QObject *app);
    BRisaApplication *getAppByName(QString appName);

private:
    QQmlApplicationEngine *mainEngine;
    QQmlContext *ctxt;

};

#endif // BRISAAPPLICATIONMANAGER_H
