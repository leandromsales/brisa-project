#-------------------------------------------------
#
# Project created by QtCreator 2010-07-03T23:23:25
#
#-------------------------------------------------

TARGET = TVControlPoint
TEMPLATE = app

CONFIG += QT BRISA
CONFIG += qt qxt
QT += core network xml
BRISA += upnp core utils
QXT += core web

SOURCES += main.cpp\
        tvcontrolpoint.cpp

HEADERS  += tvcontrolpoint.h

FORMS    += tvcontrolpoint.ui
