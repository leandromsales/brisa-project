#-------------------------------------------------
#
# Project created by QtCreator 2010-11-04T19:19:36
#
#-------------------------------------------------

# BEFORE RUNNING MAKE:
# $ qdbusxml2cpp -c DBusInterfaceAdaptor -a dbusinterfaceadaptor.h:dbusinterfaceadaptor.cpp ../common/br.ufal.ic.BrisaUpnp.xml


QT += core network xml dbus

#DBUS_ADAPTORS += com.nokia.Demo.xml

TARGET = ../../bin/usr/bin/brisa_upnpfs
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += QT BRISA
BRISA += upnp core utils

TEMPLATE = app

QMAKE_CXXFLAGS+=-D_FILE_OFFSET_BITS=64  #-DFUSE_USE_VERSION=28

SOURCES += main_brisa.cpp \
    controlpoint.cpp \
    dbusinterfaceadaptor.cpp \
    filemanager.cpp \
    ../common/logger.cpp

HEADERS +=  controlpoint.h \
    dbusinterfaceadaptor.h \
    filemanager.h \
    ../common/logger.h

OTHER_FILES += ../common/br.ufal.ic.BrisaUpnp.xml \
    ../../doc/history.txt \
    ../../doc/tasklist.txt
