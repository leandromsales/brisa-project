#ifndef BRISAAPPLICATIONMANAGER_H
#define BRISAAPPLICATIONMANAGER_H

#include <QObject>
#include <QDebug>

#include "brisaapplication.h"

class BRisaApplicationManager : public QObject
{
    Q_OBJECT

public:

    BRisaApplicationManager();
    ~BRisaApplicationManager();

    Q_INVOKABLE QList<QObject *> getListApps();
    Q_INVOKABLE int getNumOfApps();

    void addApp(QObject *app);

private:

    QList<QObject *> apps;

    int numOfApps;

};

#endif // BRISAAPPLICATIONMANAGER_H
