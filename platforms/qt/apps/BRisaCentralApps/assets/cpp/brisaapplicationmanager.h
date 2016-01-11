#ifndef BRISAAPPLICATIONMANAGER_H
#define BRISAAPPLICATIONMANAGER_H

#include <QQuickView>
#include <QObject>
#include <QDebug>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "brisaapplication.h"

class BRisaApplicationManager : public QObject
{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(int, numOfApps)
public:

    BRisaApplicationManager(QQmlApplicationEngine &engine);
    Q_INVOKABLE QList<QObject *> getListApps();
    Q_INVOKABLE QString getCurrentAppDir();

    Q_INVOKABLE void run(QString name);

    void addApp(QObject *app);
    BRisaApplication *getAppByName(QString appName);

private:

    QList<QObject *> apps;

    QString currentAppDir;

    QQmlContext *ctxt;

};

#endif // BRISAAPPLICATIONMANAGER_H
