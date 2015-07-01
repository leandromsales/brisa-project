# -------------------------------------------------
# Project created by QtCreator 2010-07-03T10:17:12
# -------------------------------------------------
QT += network \
    xml
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
TARGET = PhoneDirectoryDevice
TEMPLATE = app
SOURCES += main.cpp \
    phoneDirectoryDevice.cpp \
    phoneDirectoryFunctions.cpp
HEADERS += phoneDirectoryDevice.h \
    phoneDirectoryFunctions.h
FORMS += phoneDirectoryDevice.ui
