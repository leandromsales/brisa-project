# Replace with your basedir
BASEDIR = /home/larissa/UFAL/Compelab/git/brisa-project/brisa-src

INCLUDEPATH += $$BASEDIR/Brisa/include

# Replace all paths after $$BASEDIR/ with your build paths
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
    INCLUDEPATH += $$BASEDIR/build-brisa
    DEPENDPATH += $$BASEDIR/build-brisa
    LIBS += -L$$BASEDIR/build-brisa -lbrisa
}
