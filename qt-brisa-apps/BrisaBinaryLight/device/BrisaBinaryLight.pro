# -------------------------------------------------
# Project created by QtCreator 2009-11-17T14:45:35
# -------------------------------------------------
TARGET = brisabinarylight
TEMPLATE = app
CONFIG += QT \
    BRISA
CONFIG += qt
QT += core \
    network \
    xml
BRISA += upnp \
    core \
    utils
SOURCES += main.cpp \
    light.cpp \
    switchPower.cpp
HEADERS += light.h \
    switchPower.h
FORMS += light.ui
OTHER_FILES += SwitchPower-scpd.xml
