#This file was auto-generated. Don't edit this.

BASEDIR = /home/bruno/Documentos/Workspaces/Qt_Projects/brisa-project/platforms/qt/lib

INCLUDEPATH += $$BASEDIR/include
unix {
    message("BRisa Unix config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/../build-brisa
}
