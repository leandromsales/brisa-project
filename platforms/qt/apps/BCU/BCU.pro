TEMPLATE = app

QT += qml quick widgets xml

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    TO_DO.txt

HEADERS += \
    controlpointbcu.h \
    myclasstest.h \
    FolderCompressor.h \
    src/network/brisanetwork.h \
    src/shared/soap/qtsoap.h \
    src/shared/soap/soap.h \
    src/shared/soap/soapenvelope.h \
    src/shared/ssdp/ssdp.h \
    src/shared/ssdp/ssdpclient.h \
    src/shared/ssdp/ssdpprotocol.h \
    src/shared/ssdp/ssdpserver.h \
    src/shared/webserver/http/httpmessage.h \
    src/shared/webserver/http/httprequest.h \
    src/shared/webserver/http/httpresponse.h \
    src/shared/webserver/http/httpserver.h \
    src/shared/webserver/http/httpsession.h \
    src/shared/webserver/http/httpsessionmanager.h \
    src/shared/webserver/http/httpversion.h \
    src/shared/webserver/webfile.h \
    src/shared/webserver/webserver.h \
    src/shared/webserver/webserversession.h \
    src/shared/webserver/webservice.h \
    src/shared/webserver/webstaticcontent.h \
    src/upnp/controlpoint/av/didl/container.h \
    src/upnp/controlpoint/av/didl/didllite.h \
    src/upnp/controlpoint/av/didl/item.h \
    src/upnp/controlpoint/av/didl/res.h \
    src/upnp/controlpoint/av/controlpoint_media_globals.h \
    src/upnp/controlpoint/av/mediacontrolpoint.h \
    src/upnp/controlpoint/av/mediarenderercpdevice.h \
    src/upnp/controlpoint/av/mediaserverdevice.h \
    src/upnp/controlpoint/actionargument.h \
    src/upnp/controlpoint/actionsales.h \
    src/upnp/controlpoint/controlpoint.h \
    src/upnp/controlpoint/cpdevice.h \
    src/upnp/controlpoint/cpservice.h \
    src/upnp/controlpoint/customreply.h \
    src/upnp/controlpoint/devicesales.h \
    src/upnp/controlpoint/devicexmlhandlercp.h \
    src/upnp/controlpoint/eventproxy.h \
    src/upnp/controlpoint/iconsales.h \
    src/upnp/controlpoint/msearchclientcp.h \
    src/upnp/controlpoint/multicasteventreceiver.h \
    src/upnp/controlpoint/servicesales.h \
    src/upnp/controlpoint/statevariablesales.h \
    src/upnp/controlpoint/udplistener.h \
    src/upnp/device/av/mediarendererdevice.h \
    src/upnp/device/av/mediarendererdeviceservice.h \
    src/upnp/device/abstracteventmessage.h \
    src/upnp/device/actionxmlparser.h \
    src/upnp/device/controlwebservice.h \
    src/upnp/device/device.h \
    src/upnp/device/devicexmlhandler.h \
    src/upnp/device/eventcontroller.h \
    src/upnp/device/eventmessage.h \
    src/upnp/device/eventsubscription.h \
    src/upnp/device/multicasteventmessage.h \
    src/upnp/device/service.h \
    src/upnp/abstracteventsubscription.h \
    src/upnp/abstractservice.h \
    src/upnp/action.h \
    src/upnp/argument.h \
    src/upnp/brisaglobal.h \
    src/upnp/brisautils.h \
    src/upnp/icon.h \
    src/upnp/servicexmlhandler.h \
    src/upnp/statevariable.h \
    dataobject.h

SOURCES += main.cpp \
    controlpointbcu.cpp \
    myclasstest.cpp \
    FolderCompressor.cpp \
    src/network/brisanetwork.cpp \
    src/shared/soap/qtsoap.cpp \
    src/shared/soap/soap.cpp \
    src/shared/soap/soapenvelope.cpp \
    src/shared/ssdp/ssdp.cpp \
    src/shared/ssdp/ssdpclient.cpp \
    src/shared/ssdp/ssdpserver.cpp \
    src/shared/webserver/http/httpmessage.cpp \
    src/shared/webserver/http/httprequest.cpp \
    src/shared/webserver/http/httpresponse.cpp \
    src/shared/webserver/http/httpserver.cpp \
    src/shared/webserver/http/httpsession.cpp \
    src/shared/webserver/http/httpsessionmanager.cpp \
    src/shared/webserver/webfile.cpp \
    src/shared/webserver/webserver.cpp \
    src/shared/webserver/webserversession.cpp \
    src/shared/webserver/webservice.cpp \
    src/shared/webserver/webstaticcontent.cpp \
    src/upnp/controlpoint/av/didl/container.cpp \
    src/upnp/controlpoint/av/didl/didllite.cpp \
    src/upnp/controlpoint/av/didl/item.cpp \
    src/upnp/controlpoint/av/didl/res.cpp \
    src/upnp/controlpoint/av/mediacontrolpoint.cpp \
    src/upnp/controlpoint/av/mediarenderercpdevice.cpp \
    src/upnp/controlpoint/av/mediaserverdevice.cpp \
    src/upnp/controlpoint/actionargument.cpp \
    src/upnp/controlpoint/actionsales.cpp \
    src/upnp/controlpoint/controlpoint.cpp \
    src/upnp/controlpoint/cpdevice.cpp \
    src/upnp/controlpoint/cpservice.cpp \
    src/upnp/controlpoint/customreply.cpp \
    src/upnp/controlpoint/devicesales.cpp \
    src/upnp/controlpoint/devicexmlhandlercp.cpp \
    src/upnp/controlpoint/eventproxy.cpp \
    src/upnp/controlpoint/iconsales.cpp \
    src/upnp/controlpoint/msearchclientcp.cpp \
    src/upnp/controlpoint/multicasteventreceiver.cpp \
    src/upnp/controlpoint/servicesales.cpp \
    src/upnp/controlpoint/statevariablesales.cpp \
    src/upnp/controlpoint/udplistener.cpp \
    src/upnp/device/av/mediarendererdevice.cpp \
    src/upnp/device/av/mediarendererdeviceservice.cpp \
    src/upnp/device/abstracteventmessage.cpp \
    src/upnp/device/actionxmlparser.cpp \
    src/upnp/device/controlwebservice.cpp \
    src/upnp/device/device.cpp \
    src/upnp/device/devicexmlhandler.cpp \
    src/upnp/device/eventcontroller.cpp \
    src/upnp/device/eventmessage.cpp \
    src/upnp/device/eventsubscription.cpp \
    src/upnp/device/multicasteventmessage.cpp \
    src/upnp/device/service.cpp \
    src/upnp/abstracteventsubscription.cpp \
    src/upnp/abstractservice.cpp \
    src/upnp/action.cpp \
    src/upnp/argument.cpp \
    src/upnp/icon.cpp \
    src/upnp/servicexmlhandler.cpp \
    src/upnp/statevariable.cpp \
    dataobject.cpp
