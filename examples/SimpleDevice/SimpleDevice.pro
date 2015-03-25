#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T09:28:26
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = SimpleDevice
CONFIG   += console
CONFIG   -= app_bundle

include(brisa.pri)

TEMPLATE = app


SOURCES += main.cpp \
    simpledevice.cpp \
    functions.cpp

HEADERS += \
    simpledevice.h \
    functions.h \
    simpledevice_ui.h

RESOURCES += \
    resource.qrc

