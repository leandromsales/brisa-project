TEMPLATE = app

QT += qml quick widgets

qtHaveModule(webengine) {
        QT += webengine
        DEFINES += QT_WEBVIEW_WEBENGINE_BACKEND
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
include(brisa.pri)

SOURCES +=  main.cpp \
            assets/cpp/bcadevice.cpp \
            assets/cpp/bcajson.cpp \
            assets/cpp/brisaapplication.cpp \
            assets/cpp/brisaapplicationmanager.cpp \
            assets/cpp/FolderCompressor.cpp \
            assets/cpp/functions.cpp \
            assets/cpp/serviceApp.cpp \
            assets/cpp/essencials/qqmlobjectlistmodel.cpp

HEADERS +=  assets/cpp/bcadevice.h \
            assets/cpp/bcajson.h \
            assets/cpp/brisaapplication.h \
            assets/cpp/brisaapplicationmanager.h \
            assets/cpp/FolderCompressor.h \
            assets/cpp/functions.h \
            assets/cpp/serviceApp.h \
            assets/cpp/essencials/qqmlobjectlistmodel.h

RESOURCES += qml.qrc \
            assets/qml/CreateApp/createApp.qrc \
            assets/qml/Components/essencials/components.qrc \
            assets/qml/Components/brisaComponents.qrc \
            assets/qml/Screen/screens.qrc \
    src.qrc
