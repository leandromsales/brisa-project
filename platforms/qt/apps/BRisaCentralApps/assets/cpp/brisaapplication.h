#ifndef BRISAAPPLICATION_H
#define BRISAAPPLICATION_H

#include <QStringList>
#include <QString>
#include <QObject>
#include <QDebug>
#include <QList>

#include "essencials/qqmlhelpers.h"
#include "serviceApp.h"

class BRisaApplication : public QObject
{
    Q_OBJECT
    QML_WRITABLE_PROPERTY(QList<ServiceApp *>,services)
    QML_WRITABLE_PROPERTY(QString,description)
    QML_WRITABLE_PROPERTY(QString,iconPath)
    QML_WRITABLE_PROPERTY(QString,title)
    QML_WRITABLE_PROPERTY(QString,url)
public:
    BRisaApplication(QString newIconPath, QString newTitle, QString newDescription, QList<ServiceApp *>newServices);
    BRisaApplication(QString newIconPath, QString newTitle, QString newUrl, QList<QString> *jsonFile);
    Q_INVOKABLE QStringList getString() const;
};

#endif // BRISAAPPLICATION_H
