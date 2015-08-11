# -------------------------------------------------
# Project created by QtCreator 2009-11-24T16:16:12
# -------------------------------------------------
QT += phonon
TARGET = BrisaMediaRenderer
TEMPLATE = app
CONFIG += QT \
    BRISA
CONFIG += qt
QT += core \
    network \
    xml
BRISA += upnp \
    core \
    utils
SOURCES += main.cpp \
    mainwindow.cpp \
    avtransport.cpp \
    connectionmanager.cpp \
    rendercontrol.cpp \
    controlpoint.cpp \
    brisamediarenderer.cpp
HEADERS += mainwindow.h \
    connectionmanager.h \
    rendercontrol.h \
    avtransport.h \
    controlpoint.h \
    brisamediarenderer.h
FORMS += mainwindow.ui
