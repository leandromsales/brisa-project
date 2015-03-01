#-------------------------------------------------
#
# Project created by QtCreator 2015-02-23T15:53:27
#
#-------------------------------------------------

QT       += core gui xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG   += console
CONFIG   -= app_bundle

TARGET = UpnpCalculator
TEMPLATE = app

SOURCES += main.cpp\
        upnpcalculator.cpp \
    functions.cpp \
    network/brisanetwork.cpp \
    network/networkmanager.cpp \
    shared/soap/qtsoap.cpp \
    shared/soap/soap.cpp \
    shared/soap/soapenvelope.cpp \
    shared/ssdp/ssdp.cpp \
    shared/ssdp/ssdpclient.cpp \
    shared/ssdp/ssdpserver.cpp \
    shared/ssdp/ssdpudpsocket.cpp \
    shared/webserver/http/httpmessage.cpp \
    shared/webserver/http/httprequest.cpp \
    shared/webserver/http/httpresponse.cpp \
    shared/webserver/http/httpserver.cpp \
    shared/webserver/http/httpsession.cpp \
    shared/webserver/http/httpsessionmanager.cpp \
    shared/webserver/webfile.cpp \
    shared/webserver/webserver.cpp \
    shared/webserver/webserversession.cpp \
    shared/webserver/webservice.cpp \
    shared/webserver/webstaticcontent.cpp \
    upnp/controlpoint/av/didl/container.cpp \
    upnp/controlpoint/av/didl/didllite.cpp \
    upnp/controlpoint/av/didl/item.cpp \
    upnp/controlpoint/av/didl/res.cpp \
    upnp/controlpoint/av/mediacontrolpoint.cpp \
    upnp/controlpoint/av/mediarenderercpdevice.cpp \
    upnp/controlpoint/av/mediaserverdevice.cpp \
    upnp/controlpoint/actionargument.cpp \
    upnp/controlpoint/actionsales.cpp \
    upnp/controlpoint/controlpoint.cpp \
    upnp/controlpoint/cpdevice.cpp \
    upnp/controlpoint/cpservice.cpp \
    upnp/controlpoint/customreply.cpp \
    upnp/controlpoint/devicesales.cpp \
    upnp/controlpoint/devicexmlhandlercp.cpp \
    upnp/controlpoint/eventproxy.cpp \
    upnp/controlpoint/iconsales.cpp \
    upnp/controlpoint/msearchclientcp.cpp \
    upnp/controlpoint/multicasteventreceiver.cpp \
    upnp/controlpoint/servicesales.cpp \
    upnp/controlpoint/statevariablesales.cpp \
    upnp/controlpoint/udplistener.cpp \
    upnp/device/av/mediarendererdevice.cpp \
    upnp/device/av/mediarendererdeviceservice.cpp \
    upnp/device/abstracteventmessage.cpp \
    upnp/device/actionxmlparser.cpp \
    upnp/device/controlwebservice.cpp \
    upnp/device/device.cpp \
    upnp/device/devicexmlhandler.cpp \
    upnp/device/eventcontroller.cpp \
    upnp/device/eventmessage.cpp \
    upnp/device/eventsubscription.cpp \
    upnp/device/multicasteventmessage.cpp \
    upnp/device/service.cpp \
    upnp/abstracteventsubscription.cpp \
    upnp/abstractservice.cpp \
    upnp/action.cpp \
    upnp/argument.cpp \
    upnp/icon.cpp \
    upnp/servicexmlhandler.cpp \
    upnp/statevariable.cpp \
    button.cpp

HEADERS  += upnpcalculator.h \
    ui_calculator.h \
    functions.h \
    network/brisanetwork.h \
    network/networkmanager.h \
    shared/soap/qtsoap.h \
    shared/soap/soap.h \
    shared/soap/soapenvelope.h \
    shared/ssdp/ssdp.h \
    shared/ssdp/ssdpclient.h \
    shared/ssdp/ssdpprotocol.h \
    shared/ssdp/ssdpserver.h \
    shared/ssdp/ssdpudpsocket.h \
    shared/webserver/http/httpmessage.h \
    shared/webserver/http/httprequest.h \
    shared/webserver/http/httpresponse.h \
    shared/webserver/http/httpserver.h \
    shared/webserver/http/httpsession.h \
    shared/webserver/http/httpsessionmanager.h \
    shared/webserver/http/httpversion.h \
    shared/webserver/webfile.h \
    shared/webserver/webserver.h \
    shared/webserver/webserversession.h \
    shared/webserver/webservice.h \
    shared/webserver/webstaticcontent.h \
    upnp/controlpoint/av/didl/container.h \
    upnp/controlpoint/av/didl/didllite.h \
    upnp/controlpoint/av/didl/item.h \
    upnp/controlpoint/av/didl/res.h \
    upnp/controlpoint/av/controlpoint_media_globals.h \
    upnp/controlpoint/av/mediacontrolpoint.h \
    upnp/controlpoint/av/mediarenderercpdevice.h \
    upnp/controlpoint/av/mediaserverdevice.h \
    upnp/controlpoint/actionargument.h \
    upnp/controlpoint/actionsales.h \
    upnp/controlpoint/controlpoint.h \
    upnp/controlpoint/cpdevice.h \
    upnp/controlpoint/cpservice.h \
    upnp/controlpoint/customreply.h \
    upnp/controlpoint/devicesales.h \
    upnp/controlpoint/devicexmlhandlercp.h \
    upnp/controlpoint/eventproxy.h \
    upnp/controlpoint/iconsales.h \
    upnp/controlpoint/msearchclientcp.h \
    upnp/controlpoint/multicasteventreceiver.h \
    upnp/controlpoint/servicesales.h \
    upnp/controlpoint/statevariablesales.h \
    upnp/controlpoint/udplistener.h \
    upnp/device/av/mediarendererdevice.h \
    upnp/device/av/mediarendererdeviceservice.h \
    upnp/device/abstracteventmessage.h \
    upnp/device/actionxmlparser.h \
    upnp/device/controlwebservice.h \
    upnp/device/device.h \
    upnp/device/devicexmlhandler.h \
    upnp/device/eventcontroller.h \
    upnp/device/eventmessage.h \
    upnp/device/eventsubscription.h \
    upnp/device/multicasteventmessage.h \
    upnp/device/service.h \
    upnp/abstracteventsubscription.h \
    upnp/abstractservice.h \
    upnp/action.h \
    upnp/argument.h \
    upnp/brisaglobal.h \
    upnp/brisautils.h \
    upnp/icon.h \
    upnp/servicexmlhandler.h \
    upnp/statevariable.h \
    button.h

FORMS    += upnpcalculator.ui

SUBDIRS += app.pro

#OTHER_FILES += Functions.xml\
#    BrisaSplashScreenBinaryLight.png

RESOURCES += \
    resource.qrc

