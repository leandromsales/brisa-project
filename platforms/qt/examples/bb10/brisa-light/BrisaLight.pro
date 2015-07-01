APP_NAME = BrisaLight

CONFIG += qt warn_on cascades10

QT += network xml
LIBS += -lbbsystem -lbbdevice -lcamapi

include(brisa.pri)
include(build/build.pri)
include(config.pri)
