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
    Controller/dataobject.h \
    Controller/filedownloader.h \
    Controller/folderCompressor.h \
    Controller/essencials/qqmlhelpers.h \
    Controller/essencials/qqmlobjectlistmodel.h

SOURCES += Controller/main.cpp \
    Controller/controlpointbcu.cpp \
    Controller/dataobject.cpp \
    Controller/filedownloader.cpp \
    Controller/folderCompressor.cpp \
    Controller/essencials/qqmlhelpers.cpp \
    Controller/essencials/qqmlobjectlistmodel.cpp
