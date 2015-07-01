#-------------------------------------------------
#
# Project created by QtCreator 2010-11-04T19:19:36
#
#-------------------------------------------------

# BEFORE RUNNING MAKE:
# $ qdbusxml2cpp -v -c DBusServerInterface -p dbusserverinterface.h:dbusserverinterface.cpp ../common/br.ufal.ic.BrisaUpnp.xml

QT += core xml dbus

TARGET = ../../bin/usr/bin/upnpfs
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += QT BRISA
BRISA += upnp core utils

TEMPLATE = app

LIBS += $(shell pkg-config fuse --cflags --libs)
LIBS += -lfuse -lpthread

QMAKE_CXXFLAGS+=-D_FILE_OFFSET_BITS=64  #-DFUSE_USE_VERSION=28

SOURCES += main_fuse.cpp \
    filesystem.cpp \
    dbusserverinterface.cpp \
    dbusclient.cpp \
    ../common/logger.cpp

HEADERS +=  fusexx.hpp \
    dbusserverinterface.h \
    filesystem.h \
    fusexx.hpp \
    dbusclient.h \
    main_fuse.h \
    ../common/logger.h 
    
OTHER_FILES += ../common/br.ufal.ic.BrisaUpnp.xml \
    ../../doc/history.txt \
    ../../doc/tasklist.txt

