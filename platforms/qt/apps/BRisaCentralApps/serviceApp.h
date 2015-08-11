#ifndef SERVICEAPP_H
#define SERVICEAPP_H

#include <QStringList>
#include <QString>
#include <QObject>

class ServiceApp : public QObject
{
    Q_OBJECT

public:
    explicit ServiceApp(QString newTitle, QString newDescription);
    ~ServiceApp();

    Q_PROPERTY(QString title READ getTitle)
    Q_PROPERTY(QString description READ getDescription)

    QString getTitle();
    QString getDescription();

    QStringList getString();


private:
    QString title;
    QString description;
};

#endif // SERVICEAPP_H
