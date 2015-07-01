#include "dbusclient.h"

DBusClient::DBusClient(QObject *parent) :
    QObject(parent)
{
    server = new DBusServerInterface("br.ufal.ic.BrisaUpnp", "/", QDBusConnection::sessionBus(), 0);
}

void DBusClient::fileWrite(const QString &filepath)
{
    server->fileWrite(filepath);
}

void DBusClient::fileOpen(const QString &filepath)
{
    server->fileOpen(filepath);
}

void DBusClient::fsUmounted()
{
    server->fsUmounted();
}
