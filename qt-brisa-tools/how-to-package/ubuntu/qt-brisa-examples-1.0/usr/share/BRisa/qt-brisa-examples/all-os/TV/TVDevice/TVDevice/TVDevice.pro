# -------------------------------------------------
# Project created by QtCreator 2010-07-04T11:30:08
# -------------------------------------------------
TARGET = TVDevice
CONFIG += QT \
    BRISA
CONFIG += qt \
    qxt
QT += core \
    network \
    xml
BRISA += upnp \
    core \
    utils
QXT += core \
    web
TEMPLATE = app
SOURCES += main.cpp \
    tvdevice.cpp \
    tvoperations.cpp
HEADERS += tvdevice.h \
    tvoperations.h
FORMS += tvdevice.ui
