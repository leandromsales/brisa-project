#This file was auto-generated. Don't edit this.

BASEDIR_BRUNO = /home/bruno/Documentos/Workspaces/Qt_Projects/brisa-project/platforms/qt/lib
BASEDIR_MAC = /Users/compemac2/brisa-project/platforms/qt/build-brisa-iphonesimulator_clang_Qt_5_5_1_for_iOS-Debug/

INCLUDEPATH += $$BASEDIR_BRUNO/include

linux:!android {
    message("BRisa Unix config.")
    QT += network xml webview
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR_BRUNO/../build-brisa
}
android {
    message("BRisa Android config.")
    QT += network xml
    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        ANDROID_EXTRA_LIBS = \
            $$PWD/../../build-brisa-Android/libbrisa.so
        LIBS += -L$$PWD/../../build-brisa-Android
        LIBS += -lbrisa
    }
}

