#This file was auto-generated. Don't edit this.

BASEDIR = /mnt/8a5bbc36-bd45-46d1-b620-3ba1f05a981b/bruno/Documentos/brisa/brisa-project/brisa-src/Brisa

INCLUDEPATH += $$BASEDIR/include
unix {
    message("BRisa Unix config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/../build-brisa-Desktop_Qt_5_4_1_GCC_64bit-Debug
}

device {
    message("BRisa Device config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/arm/so.le-v7-g
}
