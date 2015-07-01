#-------------------------------------------------
#
# Project created by QtCreator 2010-07-03T16:12:00
#
#-------------------------------------------------

QT       += network xml

TARGET = PhoneDirectoryControlPoint
TEMPLATE = app


CONFIG += QT BRISA
CONFIG += qt qxt
QT += core network xml
BRISA += upnp core utils
QXT += core web


SOURCES += main.cpp\
        phoneDirectoryControlPoint.cpp

HEADERS  += phoneDirectoryControlPoint.h

FORMS    += phoneDirectoryControlPoint.ui
