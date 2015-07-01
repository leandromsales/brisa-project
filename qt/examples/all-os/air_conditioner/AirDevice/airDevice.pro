# -------------------------------------------------
# Project created by QtCreator 2010-05-27T22:03:02
# -------------------------------------------------
QT += network \
    xml
TARGET = AirDevice
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
TEMPLATE = app
SOURCES += main.cpp \
    airdevice.cpp \
    AirOperations.cpp
HEADERS += airdevice.h \
    airOperations.h
FORMS += airdevice.ui
RESOURCES += air.qrc
OTHER_FILES += airOperations.xml
