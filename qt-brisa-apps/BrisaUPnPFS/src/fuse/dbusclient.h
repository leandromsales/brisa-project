#ifndef CLIENTBRISAUPNP_H
#define CLIENTBRISAUPNP_H

#include <QObject>
#include <QDebug>

#include "dbusserverinterface.h"

#define BRISA_SERVICE_NAME "br.ufal.ic.BrisaUpnp"

class DBusClient : public QObject
{
    Q_OBJECT
public:
    explicit DBusClient(QObject *parent = 0);

    void fileWrite(const QString &filepath);
    void fileOpen(const QString &filepath);
    void fsUmounted();

private:
    DBusServerInterface *server;

};

#endif // CLIENTBRISAUPNP_H
