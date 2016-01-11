#ifndef SERVICEAPP_H
#define SERVICEAPP_H

#include <QStringList>
#include <QString>
#include <QObject>

#include "essencials/qqmlhelpers.h"

class ServiceApp : public QObject
{
    Q_OBJECT
    QML_READONLY_PROPERTY(QString, title)
    QML_READONLY_PROPERTY(QString, description)
public:
    explicit ServiceApp(QString newTitle, QString newDescription);
    QStringList getString();
};

#endif // SERVICEAPP_H
