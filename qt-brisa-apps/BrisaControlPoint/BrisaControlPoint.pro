TEMPLATE = app
TARGET = brisacontrolpoint
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += qt brisa 
CONFIG += QT BRISA 
BRISA += upnp utils core
QT += core network xml

FORMS = controlpointgui.ui
HEADERS = controlpointgui.h
SOURCES = controlpointgui.cpp main.cpp 
