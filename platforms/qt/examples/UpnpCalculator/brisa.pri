#This file was auto-generated. Don't edit this.

BASEDIR = /home/bruno/Documentos/Brisa/brisa-project/brisa-src/Brisa

INCLUDEPATH += $$BASEDIR/include
unix {
    message("BRisa Unix config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/../build-brisa-Desktop_Qt_5_4_0_GCC_64bit-Debug
}

device {
    message("BRisa Device config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/arm/so.le-v7-g
}
