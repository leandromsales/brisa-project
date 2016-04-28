TEMPLATE = app

QT += qml quick widgets xml
qtHaveModule(webengine) {
        QT += webengine
        DEFINES += QT_WEBVIEW_WEBENGINE_BACKEND
}

RESOURCES += View/qml.qrc \
    Assets/pics/pics.qrc
include(brisa.pri)

DISTFILES += Assets/TO_DO.txt \

HEADERS += \
    Controller/controlpointbcu.h \
    Controller/FolderCompressor.h \
    Controller/dataobject.h \
    Controller/filedownloader.h

SOURCES += Controller/main.cpp \
    Controller/controlpointbcu.cpp \
    Controller/FolderCompressor.cpp \
    Controller/dataobject.cpp \
    Controller/filedownloader.cpp
