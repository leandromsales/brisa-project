TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    assets/cpp/brisaapplication.cpp \
    assets/cpp/brisaapplicationmanager.cpp \
    assets/cpp/bcadevice.cpp \
    assets/cpp/functions.cpp \
    assets/cpp/serviceApp.cpp \
    assets/cpp/bcajson.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(brisa.pri)

HEADERS += \
    assets/cpp/brisaapplication.h \
    assets/cpp/brisaapplicationmanager.h \
    assets/cpp/bcadevice.h \
    assets/cpp/functions.h \
    assets/cpp/serviceApp.h \
    assets/cpp/bcajson.h
