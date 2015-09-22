#This file was auto-generated. Don't edit this.

BASEDIR = /home/larissa/UFAL/Compelab/git/brisa-project/platforms/qt/lib

INCLUDEPATH += $$BASEDIR/include
unix {
    message("BRisa Unix config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/../build-brisa
}
