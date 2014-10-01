APP_NAME = universal-control-point
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

CONFIG += qt warn_on cascades10 mobility
QT += network core xml multimedia
MOBILITY += multimedia
LIBS += -lbbmultimedia -lbbplatform -lbbcascadesmaps -lQtLocationSubset -lGLESv1_CM -lbbsystem -lbbcascadespickers

QML_IMPORT_PATH = assets/qml/

include(build/build.pri)
include(brisa.pri)
include(config.pri)

VPATH += $$INCLUDEPATH