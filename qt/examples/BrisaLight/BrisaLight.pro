#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T13:06:27
#
#-------------------------------------------------

DEFINES += PROJECT_PATH=\"\\\"$$PWD\\\"\"
DEFINES += BUILD_PATH=\"\\\"$$OUT_PWD\\\"\"

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG   += console
CONFIG   -= app_bundle

TARGET    = BrisaLight

TEMPLATE  = app

SOURCES += main.cpp \
           light.cpp \
           switchPower.cpp

HEADERS  += light.h \
            switchPower.h \
            ui_light.h

FORMS    += light.ui

OTHER_FILES += SwitchPower-scpd.xml \
    defaults.pri \
    upnp/device/av/TVOperations.xml \
    BrisaSplashScreenBinaryLight.png \
    offlight1.png \
    onlight1.png \
    BrisaLight.pro.user.3.2-pre1 \
    BrisaLight.pro.user.f2e6976 \
    app_process \
    libBrisaLight.so \
    libc.so \
    assets/SwitchPower-scpd.xml \
    assets/BrisaSplashScreenBinaryLight.png \
    assets/offlight1.png \
    assets/onlight1.png

SUBDIRS += \
    app.pro \
    app.pro \
    app.pro \
    app.pro

RESOURCES += \
    assets.qrc

# you need to add the flowing lines to your .pro/.pri file(s)
deployment.files=assets/BrisaSplashScreenBinaryLight.png
deployment.files=assets/offlight1.png
deployment.files=assets/onlight1.png
deployment.files=assets/SwitchPower-scpd.xml

#define an android block
android {
    #all assets must go to "/assets" folder of your android package
    deployment.path=assets/
}
INSTALLS += deployment
