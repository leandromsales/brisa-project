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

public:

    BRisaApplicationManager(QQmlApplicationEngine &engine);
    ~BRisaApplicationManager();

    Q_INVOKABLE QList<QObject *> getListApps();
    Q_INVOKABLE int getNumOfApps();
    Q_INVOKABLE QString getCurrentAppDir();

    Q_INVOKABLE void run(QString name);

    void addApp(QObject *app);
    BRisaApplication *getAppByName(QString appName);

private:

    QList<QObject *> apps;

    QString currentAppDir;

    QQmlContext *ctxt;
    int numOfApps;

};

#endif // BRISAAPPLICATIONMANAGER_H
