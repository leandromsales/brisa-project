TARGET = brisacontrolpoint
TEMPLATE = app
DESTDIR = android/libs/armeabi
CONFIG += QT \
    BRISA
CONFIG += qt \
    qxt
QT += core \
    network \
    xml

BRISA += upnp \
    core \
  utils


QXT += core \
    web


FORMS = controlpointgui.ui
HEADERS = controlpointgui.h
SOURCES = controlpointgui.cpp \
    main.cpp
include(../../../deploy/qt/brisa.prf)
