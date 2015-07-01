#ifndef BRISAAPPLICATION_H
#define BRISAAPPLICATION_H

#include <QStringList>
#include <QString>
#include <QObject>
#include <QList>

#include "serviceApp.h"

class BRisaApplication : public QObject
{

    Q_OBJECT

public:

    Q_PROPERTY(QList<ServiceApp *> services READ getServices)
    Q_PROPERTY(QString description READ getDescription)
    Q_PROPERTY(QString iconPath READ getIconPath)
    Q_PROPERTY(QString title READ getTitle)

    BRisaApplication(QString newIconPath, QString newTitle, QString newDescription, QList<ServiceApp *>newServices);
    ~BRisaApplication();

    Q_INVOKABLE QStringList getString() const;
    Q_INVOKABLE QString getDescription() const;
    Q_INVOKABLE QString getIconPath() const;
    Q_INVOKABLE QString getTitle() const;

    QList<ServiceApp *> getServices();

private:

    QList<ServiceApp *> services;
    QString description;
    QString iconPath;
    QString title;

};

#endif // BRISAAPPLICATION_H
