#This file was auto-generated. Don't edit this.

BASEDIR = /mnt/8a5bbc36-bd45-46d1-b620-3ba1f05a981b/bruno/Documentos/brisa/brisa-project/platforms/qt/lib

INCLUDEPATH += $$BASEDIR/include
unix {
    message("BRisa Unix config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/../build-brisa
}
