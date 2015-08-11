TEMPLATE = app
TARGET = brisaconfiguration
CONFIG += QT BRISA

QMAKE_CLEAN += file.dat
BRISA += core
QT += core  gui

HEADERS += brisaconfgui.h
SOURCES += main.cpp  brisaconfgui.cpp

FORMS += brisaconfgui.ui
RESOURCES += 

include(../../deploy/qt/brisa.prf)