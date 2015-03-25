#This file was auto-generated. Don't edit this.

# Replace with your basedir
BASEDIR = /home/bruno/Documentos/brisa/brisa-project/brisa-src

INCLUDEPATH += $$BASEDIR/Brisa/include

android {
    message("BRisa Android config.")
    QT += network xml
    INCLUDEPATH += $$BASEDIR/build-brisa-Android_for_armeabi_v7a_GCC_4_9_Qt_5_3_2-Debug
    DEPENDPATH += $$BASEDIR/build-brisa-Android_for_armeabi_v7a_GCC_4_9_Qt_5_3_2-Debug
    LIBS += -L$$BASEDIR/build-brisa-Android_for_armeabi_v7a_GCC_4_9_Qt_5_3_2-Debug -lbrisa
    LIBS += -L$$BASEDIR/arm/so.le-v7-g -lbrisa
}

# Replace all paths after $$BASEDIR/ with your build paths
unix {
    message("BRisa Unix config.")
    QT += network xml
    INCLUDEPATH += $$BASEDIR/build-brisa-Desktop_Qt_5_4_1_GCC_64bit-Debug
    DEPENDPATH += $$BASEDIR/build-brisa-Desktop_Qt_5_4_1_GCC_64bit-Debug
    LIBS += -L$$BASEDIR/build-brisa-Desktop_Qt_5_4_1_GCC_64bit-Debug -lbrisa
}
