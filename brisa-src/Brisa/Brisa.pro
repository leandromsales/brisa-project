#APP_NAME = MyLibrary
TEMPLATE = lib

INCLUDEPATH += "include"

CONFIG += qt warn_on cascades_library
CONFIG += hardening

QT += network core xml multimedia

include(config.pri)

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
