# -------------------------------------------------
# Project created by QtCreator 2009-11-17T14:45:35
# -------------------------------------------------
TARGET = brisabinarylight
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
SOURCES += main.cpp \
    light.cpp \
    switchPower.cpp
HEADERS += light.h \
    switchPower.h

FORMS += light.ui


#include(../../../src/core/core.pri)
#include(../../../src/upnp/upnp.pri)
#include(../../../src/upnp/device/device.pri)
#include(../../../src/upnp/controlpoint/controlpoint.pri)
#include(../../../src/utils/utils.pri)

#include(deps/qtsoap-2.7_1-opensource/src/qtsoap.pri)


#SUBDIRS += ../../../src/core \
#   ../../../src/upnp \
#   ../../../src/utils


OTHER_FILES += SwitchPower-scpd.xml \
    android/AndroidManifest.xml \
    android/src/eu/licentia/necessitas/industrius/QtApplication.java \
    android/src/eu/licentia/necessitas/industrius/QtSurface.java \
    android/src/eu/licentia/necessitas/industrius/QtActivity.java \
    android/src/eu/licentia/necessitas/ministro/IMinistro.aidl \
    android/src/eu/licentia/necessitas/ministro/IMinistroCallback.aidl \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-hdpi/icon.png \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/drawable-mdpi/icon.png
