TEMPLATE = app
CONFIG += QT BRISA
CONFIG += qt qxt
QT += core network xml
BRISA += upnp utils core
QXT += core web

SOURCES += main.cpp switcher.cpp
HEADERS += switcher.h
include(../../deploy/qt/brisa.prf)
