#-------------------------------------------------
#
# Project created by QtCreator 2009-11-18T15:54:37
#
#-------------------------------------------------

TARGET = brisalightswitcher
TEMPLATE = app
CONFIG += QT BRISA
CONFIG += qt qxt
QT += core network xml
BRISA += upnp core utils
QXT += core web


SOURCES += main.cpp\
        lightSwitcher.cpp

HEADERS  += lightSwitcher.h

FORMS    += lightSwitcher.ui
