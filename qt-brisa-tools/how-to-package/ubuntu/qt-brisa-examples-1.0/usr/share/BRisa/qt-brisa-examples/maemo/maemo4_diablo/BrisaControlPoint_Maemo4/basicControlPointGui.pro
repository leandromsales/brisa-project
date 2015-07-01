TARGET = brisacontrolpoint
TEMPLATE = app
CONFIG += QT BRISA
CONFIG += qt qxt
QT += core network xml
BRISA += upnp utils core
QXT += core web

HEADERS += controlpointgui.h
FORMS += controlpointgui.ui 
SOURCES += controlpointgui.cpp \
    main.cpp

include(../../deploy/qt/brisa.prf)
