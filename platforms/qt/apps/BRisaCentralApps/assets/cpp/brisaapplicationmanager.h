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

#include "bcajson.h"
#include "brisaapplication.h"
#include "FolderCompressor.h"
#include "essencials/qqmlhelpers.h"

class BRisaApplicationManager : public QObject
{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(QList<QObject *>, apps)
    QML_WRITABLE_PROPERTY(int, numOfApps)
    QML_WRITABLE_PROPERTY(QString, currentAppDir)
public:
    BRisaApplicationManager(QQmlApplicationEngine &engine, QByteArray dirPath);
    bool generateJSONFile();
    bool readJSONFile();
    Q_INVOKABLE void refreshAppList();
    Q_INVOKABLE bool fileExists(QString path);
    Q_INVOKABLE bool createAnApp(QJSValue theApp);
    Q_INVOKABLE bool removeAnApp(QByteArray appName);
    Q_INVOKABLE void run(QString name, int type);
    void addApp(QObject *app);
    BRisaApplication *getAppByName(QString appName);
signals:
    void mainQMLPathSetted();
    void webSourceUrlSetted();
private:
    QQmlApplicationEngine *mainEngine;
    QQmlContext *ctxt;
    QByteArray m_dirPath;
};

#endif // BRISAAPPLICATIONMANAGER_H
