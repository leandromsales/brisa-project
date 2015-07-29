#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T13:06:27
#
#-------------------------------------------------

include(../defaults.pri)

QT       += core gui network xml

CONFIG += qt
CONFIG += console
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

TARGET = brisabinarylight

SOURCES += main.cpp \
           light.cpp \
           switchPower.cpp

HEADERS  += light.h \
            switchPower.h \
            ui_light.h

FORMS    += light.ui

LIBS += -L../brisa -lbrisa

OTHER_FILES += SwitchPower-scpd.xml

