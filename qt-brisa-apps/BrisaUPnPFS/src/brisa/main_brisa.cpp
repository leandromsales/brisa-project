//============================================================================
// Name        : main_brisa.cpp
// Author      : Wendell Silva Soares
// Version     :
// Copyright   :
// Description : UPNP file system
//============================================================================

#include <QtGui/QApplication>
#include <QProcess>
#include <QDebug>
#include <QDir>

#include <iostream>
#include <cstdio>
#include <istream>
#include <fstream>

#include "controlpoint.h"
#include "dbusinterfaceadaptor.h"
#include "../common/logger.h"

#define ON_OPEN ("--onOpen")
#define ONLY_ON_SAVE ("--onlyOnSave")

using namespace std;

ofstream *out;

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    bool onOpen = true;

    if(a.arguments().size() > 1)
        if(a.arguments().at(1) == ONLY_ON_SAVE) {
            onOpen = false;
        }

    //Here I report the Fuse the path of the root directory of the file system
    QDir rootDir(QDir::home());
    rootDir.mkdir(".upnpfs");
    rootDir.cd(".upnpfs");
    rootDir.mkdir(".debug");

    out = new ofstream(rootDir.absolutePath().append(DEFAULT_BRISA_LOG).toAscii());
    if( !(*out) ) {
        qFatal("The log file could not be opened!");
    }
    Logger log(*out);

    //Creates the control point
    ControlPoint *cp = new ControlPoint(&rootDir, onOpen);
    new DBusInterfaceAdaptor(cp);

    //Start D-Bus Server
    log << "Conecting dbus..." << endl;
    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService("br.ufal.ic.BrisaUpnp");
    ret = connection.registerObject("/", cp);

    //Start the control point
    cp->start();

    return a.exec();
}
