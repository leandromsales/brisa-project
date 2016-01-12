#ifndef BRISAAPPLICATION_H
#define BRISAAPPLICATION_H

#include <QStringList>
#include <QString>
#include <QObject>
#include <QDebug>
#include <QList>
#include <QDir>

#include "serviceApp.h"
#include "essencials/qqmlhelpers.h"
#include "essencials/qqmlobjectlistmodel.h"

class BRisaApplication : public QObject
{
    Q_OBJECT
public:
    enum AppType { QMLApp, WebApp };
    Q_ENUM(AppType)
private:
    QML_OBJMODEL_PROPERTY(ServiceApp, services)
    QML_WRITABLE_PROPERTY(QString, description)
    QML_WRITABLE_PROPERTY(QString, iconPath)
    QML_WRITABLE_PROPERTY(QString, title)
    QML_WRITABLE_PROPERTY(QString, url)
    QML_WRITABLE_PROPERTY(QString, mainQMLFile)
    QML_WRITABLE_PROPERTY(AppType,type)
public:
    BRisaApplication(QVariantMap app, QDir dir);
};

#endif // BRISAAPPLICATION_H
