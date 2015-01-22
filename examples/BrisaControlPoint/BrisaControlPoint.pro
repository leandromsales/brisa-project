#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T13:06:27
#
#-------------------------------------------------

DEFINES += PROJECT_PATH=\"\\\"$$PWD\\\"\"
DEFINES += BUILD_PATH=\"\\\"$$OUT_PWD\\\"\"

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

OTHER_FILES += \
    upnp/device/av/TVOperations.xml \
    brisa.png \
    brisa_logo.png \
    BrisaSplashScreenControlPoint.png \
    call.png \
    clear.png \
    device.png \
    exit.png \
    info.png \
    minus.png \
    network.png \
    plus.png \
    qt.png \
    service.png \
    upnp.png \
    BrisaControlPoint.pro.user.3.2-pre1
