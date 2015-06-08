#ifndef BRISAAPPLICATION_H
#define BRISAAPPLICATION_H

#include <QStringList>
#include <QString>
#include <QObject>

class BRisaApplication : public QObject
{

    Q_OBJECT

public:

    Q_PROPERTY(QStringList services READ getServices)
    Q_PROPERTY(QString description READ getDescription)
    Q_PROPERTY(QString iconPath READ getIconPath)
    Q_PROPERTY(QString title READ getTitle)

    BRisaApplication(QString newIconPath, QString newTitle, QString newDescription, QStringList newServices);
    ~BRisaApplication();

    Q_INVOKABLE QStringList getServices() const;
    Q_INVOKABLE QString getDescription() const;
    Q_INVOKABLE QString getIconPath() const;
    Q_INVOKABLE QString getTitle() const;

private:

    QStringList services;
    QString description;
    QString iconPath;
    QString title;

};

#endif // BRISAAPPLICATION_H
