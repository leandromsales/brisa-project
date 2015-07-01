TEMPLATE = app
TARGET = brisadevicegen
CONFIG += QT BRISA
CONFIG += qt qxt
QT        += core gui xml network
BRISA += upnp utils core
QXT += core web
HEADERS   += basicsdevicesgui.h
SOURCES   += main.cpp \
    basicdevicesgui.cpp
FORMS     += basicsdevicesgui.ui    
RESOURCES +=
include(../../deploy/qt/brisa.prf)



