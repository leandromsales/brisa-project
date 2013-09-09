#ifndef DIDLLITE_H
#define DIDLLITE_H

#include <QObject>

#include "container.h"

class DIDLLite : public QObject
{
    Q_OBJECT
public:
    explicit DIDLLite(QObject *parent = 0);
    Container *parse(const QString &);

signals:
    
public slots:
    
};

#endif // DIDLLITE_H
