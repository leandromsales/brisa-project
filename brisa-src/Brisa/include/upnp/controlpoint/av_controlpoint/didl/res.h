#ifndef RES_H
#define RES_H

#include <QObject>
#include <QHash>

class Res : public QObject
{
    Q_OBJECT
public:
    explicit Res(QObject *parent = 0);
    explicit Res(const Res &);
    ~Res();
    Res & operator=(const Res &);
    
    void setValue(const QString &);
    QString getValue();

    bool addProperty(const QString &, const QString &);
    QString getProperty(const QString &);
    QHash<QString, QString> getProperties() const;

private:

    QString value;
    QHash<QString, QString> properties;

signals:
    
public slots:
    
};

#endif // RES_H
