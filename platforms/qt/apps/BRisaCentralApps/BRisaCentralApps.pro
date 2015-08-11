TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    brisaapplication.cpp \
    brisaapplicationmanager.cpp \
    bcadevice.cpp \
    functions.cpp \
    serviceApp.cpp \
    bcajson.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(brisa.pri)

HEADERS += \
    brisaapplication.h \
    brisaapplicationmanager.h \
    bcadevice.h \
    functions.h \
    serviceApp.h \
    bcajson.h
