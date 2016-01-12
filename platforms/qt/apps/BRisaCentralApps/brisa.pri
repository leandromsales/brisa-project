#This file was auto-generated. Don't edit this.

BASEDIR_BRUNO = /home/bruno/Documentos/Workspaces/Qt_Projects/brisa-project/platforms/qt/lib
BASEDIR_LARISSA = /home/larissa/UFAL/Compelab/git/brisa-project/platforms/qt/lib
INCLUDEPATH += $$BASEDIR_BRUNO/include
unix {
    message("BRisa Unix config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR_BRUNO/../build-brisa
}
