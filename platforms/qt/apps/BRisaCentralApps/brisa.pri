#This file was auto-generated. Don't edit this.

BASEDIR_BRUNO = /home/bruno/Documentos/Workspaces/Qt_Projects/brisa-project/platforms/qt/lib
BASEDIR_LARISSA = /home/larissa/UFAL/Compelab/git/brisa-project/platforms/qt/lib
BASEDIR_MAC = /Users/compemac2/brisa-project/platforms/qt/build-brisa-iphonesimulator_clang_Qt_5_5_1_for_iOS-Debug/

INCLUDEPATH += $$BASEDIR_BRUNO/include
unix {
    message("BRisa Unix config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR_BRUNO/../build-brisa
}
