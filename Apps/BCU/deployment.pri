# Replace with your basedir
BASEDIR = /home/larissa/UFAL/Compelab/git/brisa-project/brisa-src

# Replace with your basedir
BASEDIR = /home/larissa/UFAL/Compelab/git/brisa-project/brisa-src

# Include BRisa
INCLUDEPATH += $$BASEDIR/Brisa/include

# Replace all paths after $$BASEDIR/ with your build paths
unix {
    message("BRisa Unix config.")
    QT += network xml
    INCLUDEPATH += $$BASEDIR/build-brisa
    DEPENDPATH += $$BASEDIR/build-brisa
    LIBS += -L$$BASEDIR/build-brisa -lbrisa
}
