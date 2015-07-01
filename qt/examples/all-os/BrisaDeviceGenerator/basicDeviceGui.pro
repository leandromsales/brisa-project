TEMPLATE = app
TARGET = brisadevicegen
CONFIG += QT \
    BRISA
CONFIG += qt \
    qxt
QT += core \
    gui \
    xml \
    network
BRISA += upnp \
    utils \
    core
QXT += core \
    web
HEADERS += basicsdevicesgui.h \
    BinaryLightDevice/switchlightservice.h \
    BinaryLightDevice/getstatus.h \
    BinaryLightDevice/turnon.h \
    BinaryLightDevice/turnoff.h \
    BinaryLightDevice/binarylightdevice.h \
    EletronicDoorDevice/switchdoorservice.h \
    EletronicDoorDevice/open.h \
    EletronicDoorDevice/close.h \
    EletronicDoorDevice/getdoorstatus.h \
    EletronicDoorDevice/eletronicdoordevice.h \
    ConsoleGameDevice/reset.h \
    ConsoleGameDevice/setgame.h \
    ConsoleGameDevice/gameconsoleservice.h \
    ConsoleGameDevice/getgame.h \
    ConsoleGameDevice/consolegamedevice.h \
    DesktopComputerDevice/powerpcservice.h \
    DesktopComputerDevice/on.h \
    DesktopComputerDevice/off.h \
    DesktopComputerDevice/getpcstatus.h \
    DesktopComputerDevice/pcsystemtimeservice.h \
    DesktopComputerDevice/getsystemtime.h \
    DesktopComputerDevice/desktopcomputerdevice.h \
    NotebookDevice/getbatterycharge.h \
    NotebookDevice/getlevel.h \
    NotebookDevice/notebookdevice.h \
    NotebookDevice/powernotebookservice.h \
    NotebookDevice/setlevel.h \
    SmartPhoneDevice/call.h \
    SmartPhoneDevice/getcallstatus.h \
    SmartPhoneDevice/getlastnumbercalled.h \
    SmartPhoneDevice/hangup.h \
    SmartPhoneDevice/smartphonedevice.h \
    SmartPhoneDevice/smartphoneservice.h
SOURCES += main.cpp \
    basicsdevicesgui.cpp \
    BinaryLightDevice/switchlightservice.cpp \
    BinaryLightDevice/getstatus.cpp \
    BinaryLightDevice/turnon.cpp \
    BinaryLightDevice/turnoff.cpp \
    BinaryLightDevice/binarylightdevice.cpp \
    EletronicDoorDevice/switchdoorservice.cpp \
    EletronicDoorDevice/close.cpp \
    EletronicDoorDevice/open.cpp \
    EletronicDoorDevice/getdoorstatus.cpp \
    EletronicDoorDevice/eletronicdoordevice.cpp \
    ConsoleGameDevice/reset.cpp \
    ConsoleGameDevice/setgame.cpp \
    ConsoleGameDevice/gameconsoleservice.cpp \
    ConsoleGameDevice/getgame.cpp \
    ConsoleGameDevice/consolegamedevice.cpp \
    DesktopComputerDevice/powerpcservice.cpp \
    DesktopComputerDevice/on.cpp \
    DesktopComputerDevice/off.cpp \
    DesktopComputerDevice/getpcstatus.cpp \
    DesktopComputerDevice/pcsystemtimeservice.cpp \
    DesktopComputerDevice/getsystemtime.cpp \
    DesktopComputerDevice/desktopcomputerdevice.cpp \
    NotebookDevice/getbatterycharge.cpp \
    NotebookDevice/getlevel.cpp \
    NotebookDevice/notebookdevice.cpp \
    NotebookDevice/powernotebookservice.cpp \
    NotebookDevice/setlevel.cpp \
    SmartPhoneDevice/call.cpp \
    SmartPhoneDevice/getcallstatus.cpp \
    SmartPhoneDevice/getlastnumbercalled.cpp \
    SmartPhoneDevice/hangup.cpp \
    SmartPhoneDevice/smartphonedevice.cpp \
    SmartPhoneDevice/smartphoneservice.cpp
FORMS += basicsdevicesgui.ui
RESOURCES += 
include(../../deploy/qt/brisa.prf)
