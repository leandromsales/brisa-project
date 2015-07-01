#-------------------------------------------------
#
# Project created by QtCreator 2010-07-03T16:12:00
#
#-------------------------------------------------

QT       += network xml

TARGET =AirControlPoint
TEMPLATE = app


CONFIG += QT BRISA
CONFIG += qt qxt
QT += core network xml
BRISA += upnp core utils
QXT += core web


SOURCES += main.cpp\
        aircontrolpoint.cpp

HEADERS  += aircontrolpoint.h

FORMS    += aircontrolpoint.ui

RESOURCES += \
    air.qrc
