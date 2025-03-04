/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c DBusInterfaceAdaptor -a dbusinterfaceadaptor.h:dbusinterfaceadaptor.cpp ../common/br.ufal.ic.BrisaUpnp.xml
 *
 * qdbusxml2cpp is Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "dbusinterfaceadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class DBusInterfaceAdaptor
 */

DBusInterfaceAdaptor::DBusInterfaceAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

DBusInterfaceAdaptor::~DBusInterfaceAdaptor()
{
    // destructor
}

void DBusInterfaceAdaptor::fileOpen(const QString &filepath)
{
    // handle method call br.ufal.ic.BrisaUpnp.fileOpen
    QMetaObject::invokeMethod(parent(), "fileOpen", Q_ARG(QString, filepath));
}

void DBusInterfaceAdaptor::fileWrite(const QString &filepath)
{
    // handle method call br.ufal.ic.BrisaUpnp.fileWrite
    QMetaObject::invokeMethod(parent(), "fileWrite", Q_ARG(QString, filepath));
}

void DBusInterfaceAdaptor::fsUmounted()
{
    // handle method call br.ufal.ic.BrisaUpnp.fsUmounted
    QMetaObject::invokeMethod(parent(), "fsUmounted");
}

