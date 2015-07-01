TEMPLATE = app
CONFIG += QT BRISA
CONFIG += qt qxt
QT += core network xml
BRISA += upnp core utils
QXT += core web

SOURCES   += main.cpp
HEADERS   += switchPower.h
include(../../deploy/qt/brisa.prf)
