APP_NAME = niversal-control-point
DEVICE_TARGET = bb10
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

CONFIG += qt warn_on cascades10 mobility
QT += network core xml multimedia
MOBILITY += multimedia
LIBS += -lbbmultimedia -lbbplatform -lbbcascadesmaps -lQtLocationSubset -lGLESv1_CM -lbbsystem -lbbcascadespickers

QML_IMPORT_PATH = gui/$${DEVICE_TARGET}/qml/

gui.source = gui/$${DEVICE_TARGET}/qml
gui.target = gui

SOURCES += \
    viewcontroller.cpp

HEADERS += \
    viewcontroller.h
    
include(config.pri)

include(gui/gui.pri)

VPATH += $$INCLUDEPATH

RESOURCES += resources_$${DEVICE_TARGET}.qrc

device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
    }
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
    }
}
