#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T13:06:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG   += console
CONFIG   -= app_bundle

TARGET    = BrisaControlPoint

TEMPLATE  = app

include(brisa.pri)

SOURCES += main.cpp \
           controlpointgui.cpp

HEADERS  += controlpointgui.h \
            ui_controlpointgui.h

FORMS    += controlpointgui.ui
