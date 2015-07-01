#-------------------------------------------------
#
# Project created by QtCreator 2009-11-17T14:45:35
#
#-------------------------------------------------


TARGET = brisabinarylight
TEMPLATE = app
CONFIG += QT BRISA
CONFIG += qt qxt
QT += core network xml
BRISA += upnp core utils
QXT += core web


SOURCES += main.cpp\
        light.cpp

HEADERS  += light.h\
            switchPower.h

FORMS    += light.ui
