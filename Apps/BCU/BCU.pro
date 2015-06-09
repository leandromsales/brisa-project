TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    TO_DO.txt

# Replace with your basedir
BASEDIR = /home/larissa/UFAL/Compelab/git/brisa-project/brisa-src

# Replace with your basedir
BASEDIR = /home/larissa/UFAL/Compelab/git/brisa-project/brisa-src

# Include BRisa
INCLUDEPATH += $$BASEDIR/Brisa/include

# Replace all paths after $$BASEDIR/ with your build paths
unix {
    message("BRisa Unix config.")
    QT += network xml
    INCLUDEPATH += $$BASEDIR/build-brisa
    DEPENDPATH += $$BASEDIR/build-brisa
    LIBS += -L$$BASEDIR/build-brisa -lbrisa
}

