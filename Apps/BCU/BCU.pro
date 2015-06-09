TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    controlpointbcu.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    TO_DO.txt

HEADERS += \
    controlpointbcu.h
