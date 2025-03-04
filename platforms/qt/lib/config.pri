# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        profile {
            CONFIG += \
                config_pri_source_group2
        } else {
            CONFIG += \
                config_pri_source_group1
        }

    }

    CONFIG(release, debug|release) {
        !profile {
            CONFIG += \
                config_pri_source_group2
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            CONFIG += \
                config_pri_source_group2
        }
    }
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/network/brisanetwork.cpp) \
        $$quote($$BASEDIR/src/network/networkmanager.cpp) \
        $$quote($$BASEDIR/src/shared/soap/qtsoap.cpp) \
        $$quote($$BASEDIR/src/shared/soap/soap.cpp) \
        $$quote($$BASEDIR/src/shared/soap/soapenvelope.cpp) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdp.cpp) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpclient.cpp) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpserver.cpp) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpudpsocket.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpmessage.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httprequest.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpresponse.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpserver.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpsession.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpsessionmanager.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webfile.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webserver.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webserversession.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webservice.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webstaticcontent.cpp) \
        $$quote($$BASEDIR/src/upnp/abstracteventsubscription.cpp) \
        $$quote($$BASEDIR/src/upnp/abstractservice.cpp) \
        $$quote($$BASEDIR/src/upnp/action.cpp) \
        $$quote($$BASEDIR/src/upnp/argument.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/actionargument.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/actionsales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/container.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/didllite.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/item.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/res.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediacontrolpoint.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediarenderercpdevice.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediaserverdevice.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/controlpoint.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/cpdevice.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/cpservice.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/devicesales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/devicexmlhandlercp.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/eventproxy.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/iconsales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/msearchclientcp.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/multicasteventreceiver.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/servicesales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/statevariablesales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/udplistener.cpp) \
        $$quote($$BASEDIR/src/upnp/device/abstracteventmessage.cpp) \
        $$quote($$BASEDIR/src/upnp/device/actionxmlparser.cpp) \
        $$quote($$BASEDIR/src/upnp/device/av/mediarendererdevice.cpp) \
        $$quote($$BASEDIR/src/upnp/device/av/mediarendererdeviceservice.cpp) \
        $$quote($$BASEDIR/src/upnp/device/controlwebservice.cpp) \
        $$quote($$BASEDIR/src/upnp/device/device.cpp) \
        $$quote($$BASEDIR/src/upnp/device/devicexmlhandler.cpp) \
        $$quote($$BASEDIR/src/upnp/device/eventcontroller.cpp) \
        $$quote($$BASEDIR/src/upnp/device/eventmessage.cpp) \
        $$quote($$BASEDIR/src/upnp/device/eventsubscription.cpp) \
        $$quote($$BASEDIR/src/upnp/device/multicasteventmessage.cpp) \
        $$quote($$BASEDIR/src/upnp/device/service.cpp) \
        $$quote($$BASEDIR/src/upnp/icon.cpp) \
        $$quote($$BASEDIR/src/upnp/servicexmlhandler.cpp) \
        $$quote($$BASEDIR/src/upnp/statevariable.cpp)

    HEADERS += \
        $$quote($$BASEDIR/include/upnp/controlpoint/actionsales.h) \
        $$quote($$BASEDIR/include/upnp/controlpoint/devicesales.h) \
        $$quote($$BASEDIR/include/upnp/controlpoint/iconsales.h) \
        $$quote($$BASEDIR/include/upnp/controlpoint/servicesales.h) \
        $$quote($$BASEDIR/include/upnp/controlpoint/statevariablesales.h) \
        $$quote($$BASEDIR/src/network/brisanetwork.h) \
        $$quote($$BASEDIR/src/network/networkmanager.h) \
        $$quote($$BASEDIR/src/shared/soap/qtsoap.h) \
        $$quote($$BASEDIR/src/shared/soap/soap.h) \
        $$quote($$BASEDIR/src/shared/soap/soapenvelope.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdp.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpclient.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpprotocol.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpserver.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpudpsocket.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpmessage.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httprequest.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpresponse.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpserver.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpsession.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpsessionmanager.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpversion.h) \
        $$quote($$BASEDIR/src/shared/webserver/webfile.h) \
        $$quote($$BASEDIR/src/shared/webserver/webserver.h) \
        $$quote($$BASEDIR/src/shared/webserver/webserversession.h) \
        $$quote($$BASEDIR/src/shared/webserver/webservice.h) \
        $$quote($$BASEDIR/src/shared/webserver/webstaticcontent.h) \
        $$quote($$BASEDIR/src/upnp/abstracteventsubscription.h) \
        $$quote($$BASEDIR/src/upnp/abstractservice.h) \
        $$quote($$BASEDIR/src/upnp/action.h) \
        $$quote($$BASEDIR/src/upnp/argument.h) \
        $$quote($$BASEDIR/src/upnp/brisaglobal.h) \
        $$quote($$BASEDIR/src/upnp/brisautils.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/actionargument.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/actionsales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/controlpoint_media_globals.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/container.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/didllite.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/item.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/res.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediacontrolpoint.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediarenderercpdevice.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediaserverdevice.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/controlpoint.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/cpdevice.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/cpservice.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/devicesales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/devicexmlhandlercp.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/eventproxy.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/iconsales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/msearchclientcp.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/multicasteventreceiver.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/servicesales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/statevariablesales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/udplistener.h) \
        $$quote($$BASEDIR/src/upnp/device/abstracteventmessage.h) \
        $$quote($$BASEDIR/src/upnp/device/actionxmlparser.h) \
        $$quote($$BASEDIR/src/upnp/device/av/mediarendererdevice.h) \
        $$quote($$BASEDIR/src/upnp/device/av/mediarendererdeviceservice.h) \
        $$quote($$BASEDIR/src/upnp/device/controlwebservice.h) \
        $$quote($$BASEDIR/src/upnp/device/device.h) \
        $$quote($$BASEDIR/src/upnp/device/devicexmlhandler.h) \
        $$quote($$BASEDIR/src/upnp/device/eventcontroller.h) \
        $$quote($$BASEDIR/src/upnp/device/eventmessage.h) \
        $$quote($$BASEDIR/src/upnp/device/eventsubscription.h) \
        $$quote($$BASEDIR/src/upnp/device/multicasteventmessage.h) \
        $$quote($$BASEDIR/src/upnp/device/service.h) \
        $$quote($$BASEDIR/src/upnp/icon.h) \
        $$quote($$BASEDIR/src/upnp/servicexmlhandler.h) \
        $$quote($$BASEDIR/src/upnp/statevariable.h)
}

config_pri_source_group2 {
    SOURCES += \
        $$quote($$BASEDIR/src/network/brisanetwork.cpp) \
        $$quote($$BASEDIR/src/network/networkmanager.cpp) \
        $$quote($$BASEDIR/src/shared/soap/qtsoap.cpp) \
        $$quote($$BASEDIR/src/shared/soap/soap.cpp) \
        $$quote($$BASEDIR/src/shared/soap/soapenvelope.cpp) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdp.cpp) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpclient.cpp) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpserver.cpp) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpudpsocket.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpmessage.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httprequest.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpresponse.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpserver.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpsession.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpsessionmanager.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webfile.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webserver.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webserversession.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webservice.cpp) \
        $$quote($$BASEDIR/src/shared/webserver/webstaticcontent.cpp) \
        $$quote($$BASEDIR/src/upnp/abstracteventsubscription.cpp) \
        $$quote($$BASEDIR/src/upnp/abstractservice.cpp) \
        $$quote($$BASEDIR/src/upnp/action.cpp) \
        $$quote($$BASEDIR/src/upnp/argument.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/actionargument.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/actionsales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/container.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/didllite.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/item.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/res.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediacontrolpoint.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediarenderercpdevice.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediaserverdevice.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/controlpoint.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/cpdevice.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/cpservice.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/devicesales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/devicexmlhandlercp.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/eventproxy.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/iconsales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/msearchclientcp.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/multicasteventreceiver.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/servicesales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/statevariablesales.cpp) \
        $$quote($$BASEDIR/src/upnp/controlpoint/udplistener.cpp) \
        $$quote($$BASEDIR/src/upnp/device/abstracteventmessage.cpp) \
        $$quote($$BASEDIR/src/upnp/device/actionxmlparser.cpp) \
        $$quote($$BASEDIR/src/upnp/device/av/mediarendererdevice.cpp) \
        $$quote($$BASEDIR/src/upnp/device/av/mediarendererdeviceservice.cpp) \
        $$quote($$BASEDIR/src/upnp/device/controlwebservice.cpp) \
        $$quote($$BASEDIR/src/upnp/device/device.cpp) \
        $$quote($$BASEDIR/src/upnp/device/devicexmlhandler.cpp) \
        $$quote($$BASEDIR/src/upnp/device/eventcontroller.cpp) \
        $$quote($$BASEDIR/src/upnp/device/eventmessage.cpp) \
        $$quote($$BASEDIR/src/upnp/device/eventsubscription.cpp) \
        $$quote($$BASEDIR/src/upnp/device/multicasteventmessage.cpp) \
        $$quote($$BASEDIR/src/upnp/device/service.cpp) \
        $$quote($$BASEDIR/src/upnp/icon.cpp) \
        $$quote($$BASEDIR/src/upnp/servicexmlhandler.cpp) \
        $$quote($$BASEDIR/src/upnp/statevariable.cpp)

    HEADERS += \
        $$quote($$BASEDIR/include/upnp/controlpoint/actionsales.h) \
        $$quote($$BASEDIR/include/upnp/controlpoint/devicesales.h) \
        $$quote($$BASEDIR/include/upnp/controlpoint/iconsales.h) \
        $$quote($$BASEDIR/include/upnp/controlpoint/servicesales.h) \
        $$quote($$BASEDIR/include/upnp/controlpoint/statevariablesales.h) \
        $$quote($$BASEDIR/src/network/brisanetwork.h) \
        $$quote($$BASEDIR/src/network/networkmanager.h) \
        $$quote($$BASEDIR/src/shared/soap/qtsoap.h) \
        $$quote($$BASEDIR/src/shared/soap/soap.h) \
        $$quote($$BASEDIR/src/shared/soap/soapenvelope.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdp.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpclient.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpprotocol.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpserver.h) \
        $$quote($$BASEDIR/src/shared/ssdp/ssdpudpsocket.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpmessage.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httprequest.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpresponse.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpserver.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpsession.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpsessionmanager.h) \
        $$quote($$BASEDIR/src/shared/webserver/http/httpversion.h) \
        $$quote($$BASEDIR/src/shared/webserver/webfile.h) \
        $$quote($$BASEDIR/src/shared/webserver/webserver.h) \
        $$quote($$BASEDIR/src/shared/webserver/webserversession.h) \
        $$quote($$BASEDIR/src/shared/webserver/webservice.h) \
        $$quote($$BASEDIR/src/shared/webserver/webstaticcontent.h) \
        $$quote($$BASEDIR/src/upnp/abstracteventsubscription.h) \
        $$quote($$BASEDIR/src/upnp/abstractservice.h) \
        $$quote($$BASEDIR/src/upnp/action.h) \
        $$quote($$BASEDIR/src/upnp/argument.h) \
        $$quote($$BASEDIR/src/upnp/brisaglobal.h) \
        $$quote($$BASEDIR/src/upnp/brisautils.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/actionargument.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/actionsales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/controlpoint_media_globals.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/container.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/didllite.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/item.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/didl/res.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediacontrolpoint.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediarenderercpdevice.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/av/mediaserverdevice.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/controlpoint.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/cpdevice.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/cpservice.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/devicesales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/devicexmlhandlercp.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/eventproxy.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/iconsales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/msearchclientcp.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/multicasteventreceiver.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/servicesales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/statevariablesales.h) \
        $$quote($$BASEDIR/src/upnp/controlpoint/udplistener.h) \
        $$quote($$BASEDIR/src/upnp/device/abstracteventmessage.h) \
        $$quote($$BASEDIR/src/upnp/device/actionxmlparser.h) \
        $$quote($$BASEDIR/src/upnp/device/av/mediarendererdevice.h) \
        $$quote($$BASEDIR/src/upnp/device/av/mediarendererdeviceservice.h) \
        $$quote($$BASEDIR/src/upnp/device/controlwebservice.h) \
        $$quote($$BASEDIR/src/upnp/device/device.h) \
        $$quote($$BASEDIR/src/upnp/device/devicexmlhandler.h) \
        $$quote($$BASEDIR/src/upnp/device/eventcontroller.h) \
        $$quote($$BASEDIR/src/upnp/device/eventmessage.h) \
        $$quote($$BASEDIR/src/upnp/device/eventsubscription.h) \
        $$quote($$BASEDIR/src/upnp/device/multicasteventmessage.h) \
        $$quote($$BASEDIR/src/upnp/device/service.h) \
        $$quote($$BASEDIR/src/upnp/icon.h) \
        $$quote($$BASEDIR/src/upnp/servicexmlhandler.h) \
        $$quote($$BASEDIR/src/upnp/statevariable.h)
}

INCLUDEPATH += $$quote($$BASEDIR/src/shared/webserver) \
    $$quote($$BASEDIR/src/upnp/device) \
    $$quote($$BASEDIR/src/network) \
    $$quote($$BASEDIR/src/upnp/controlpoint/av) \
    $$quote($$BASEDIR/src/upnp/controlpoint/av/didl) \
    $$quote($$BASEDIR/src/shared/soap) \
    $$quote($$BASEDIR/include/upnp/controlpoint) \
    $$quote($$BASEDIR/src/shared/ssdp) \
    $$quote($$BASEDIR/src/upnp/device/av) \
    $$quote($$BASEDIR/src/upnp/controlpoint) \
    $$quote($$BASEDIR/src/upnp) \
    $$quote($$BASEDIR/src/shared/webserver/http)

CONFIG += precompile_header

PRECOMPILED_HEADER = $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../include/*.c) \
        $$quote($$BASEDIR/../include/*.c++) \
        $$quote($$BASEDIR/../include/*.cc) \
        $$quote($$BASEDIR/../include/*.cpp) \
        $$quote($$BASEDIR/../include/*.cxx) \
        $$quote($$BASEDIR/../include/network/*.c) \
        $$quote($$BASEDIR/../include/network/*.c++) \
        $$quote($$BASEDIR/../include/network/*.cc) \
        $$quote($$BASEDIR/../include/network/*.cpp) \
        $$quote($$BASEDIR/../include/network/*.cxx) \
        $$quote($$BASEDIR/../include/shared/*.c) \
        $$quote($$BASEDIR/../include/shared/*.c++) \
        $$quote($$BASEDIR/../include/shared/*.cc) \
        $$quote($$BASEDIR/../include/shared/*.cpp) \
        $$quote($$BASEDIR/../include/shared/*.cxx) \
        $$quote($$BASEDIR/../include/shared/soap/*.c) \
        $$quote($$BASEDIR/../include/shared/soap/*.c++) \
        $$quote($$BASEDIR/../include/shared/soap/*.cc) \
        $$quote($$BASEDIR/../include/shared/soap/*.cpp) \
        $$quote($$BASEDIR/../include/shared/soap/*.cxx) \
        $$quote($$BASEDIR/../include/shared/ssdp/*.c) \
        $$quote($$BASEDIR/../include/shared/ssdp/*.c++) \
        $$quote($$BASEDIR/../include/shared/ssdp/*.cc) \
        $$quote($$BASEDIR/../include/shared/ssdp/*.cpp) \
        $$quote($$BASEDIR/../include/shared/ssdp/*.cxx) \
        $$quote($$BASEDIR/../include/shared/webserver/*.c) \
        $$quote($$BASEDIR/../include/shared/webserver/*.c++) \
        $$quote($$BASEDIR/../include/shared/webserver/*.cc) \
        $$quote($$BASEDIR/../include/shared/webserver/*.cpp) \
        $$quote($$BASEDIR/../include/shared/webserver/*.cxx) \
        $$quote($$BASEDIR/../include/shared/webserver/http/*.c) \
        $$quote($$BASEDIR/../include/shared/webserver/http/*.c++) \
        $$quote($$BASEDIR/../include/shared/webserver/http/*.cc) \
        $$quote($$BASEDIR/../include/shared/webserver/http/*.cpp) \
        $$quote($$BASEDIR/../include/shared/webserver/http/*.cxx) \
        $$quote($$BASEDIR/../include/upnp/*.c) \
        $$quote($$BASEDIR/../include/upnp/*.c++) \
        $$quote($$BASEDIR/../include/upnp/*.cc) \
        $$quote($$BASEDIR/../include/upnp/*.cpp) \
        $$quote($$BASEDIR/../include/upnp/*.cxx) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/*.c) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/*.c++) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/*.cc) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/*.cpp) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/*.cxx) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/*.c) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/*.c++) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/*.cc) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/*.cpp) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/*.cxx) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/didl/*.c) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/didl/*.c++) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/didl/*.cc) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/didl/*.cpp) \
        $$quote($$BASEDIR/../include/upnp/controlpoint/av/didl/*.cxx) \
        $$quote($$BASEDIR/../include/upnp/device/*.c) \
        $$quote($$BASEDIR/../include/upnp/device/*.c++) \
        $$quote($$BASEDIR/../include/upnp/device/*.cc) \
        $$quote($$BASEDIR/../include/upnp/device/*.cpp) \
        $$quote($$BASEDIR/../include/upnp/device/*.cxx) \
        $$quote($$BASEDIR/../include/upnp/device/av/*.c) \
        $$quote($$BASEDIR/../include/upnp/device/av/*.c++) \
        $$quote($$BASEDIR/../include/upnp/device/av/*.cc) \
        $$quote($$BASEDIR/../include/upnp/device/av/*.cpp) \
        $$quote($$BASEDIR/../include/upnp/device/av/*.cxx) \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../src/network/*.c) \
        $$quote($$BASEDIR/../src/network/*.c++) \
        $$quote($$BASEDIR/../src/network/*.cc) \
        $$quote($$BASEDIR/../src/network/*.cpp) \
        $$quote($$BASEDIR/../src/network/*.cxx) \
        $$quote($$BASEDIR/../src/shared/*.c) \
        $$quote($$BASEDIR/../src/shared/*.c++) \
        $$quote($$BASEDIR/../src/shared/*.cc) \
        $$quote($$BASEDIR/../src/shared/*.cpp) \
        $$quote($$BASEDIR/../src/shared/*.cxx) \
        $$quote($$BASEDIR/../src/shared/soap/*.c) \
        $$quote($$BASEDIR/../src/shared/soap/*.c++) \
        $$quote($$BASEDIR/../src/shared/soap/*.cc) \
        $$quote($$BASEDIR/../src/shared/soap/*.cpp) \
        $$quote($$BASEDIR/../src/shared/soap/*.cxx) \
        $$quote($$BASEDIR/../src/shared/ssdp/*.c) \
        $$quote($$BASEDIR/../src/shared/ssdp/*.c++) \
        $$quote($$BASEDIR/../src/shared/ssdp/*.cc) \
        $$quote($$BASEDIR/../src/shared/ssdp/*.cpp) \
        $$quote($$BASEDIR/../src/shared/ssdp/*.cxx) \
        $$quote($$BASEDIR/../src/shared/webserver/*.c) \
        $$quote($$BASEDIR/../src/shared/webserver/*.c++) \
        $$quote($$BASEDIR/../src/shared/webserver/*.cc) \
        $$quote($$BASEDIR/../src/shared/webserver/*.cpp) \
        $$quote($$BASEDIR/../src/shared/webserver/*.cxx) \
        $$quote($$BASEDIR/../src/shared/webserver/http/*.c) \
        $$quote($$BASEDIR/../src/shared/webserver/http/*.c++) \
        $$quote($$BASEDIR/../src/shared/webserver/http/*.cc) \
        $$quote($$BASEDIR/../src/shared/webserver/http/*.cpp) \
        $$quote($$BASEDIR/../src/shared/webserver/http/*.cxx) \
        $$quote($$BASEDIR/../src/upnp/*.c) \
        $$quote($$BASEDIR/../src/upnp/*.c++) \
        $$quote($$BASEDIR/../src/upnp/*.cc) \
        $$quote($$BASEDIR/../src/upnp/*.cpp) \
        $$quote($$BASEDIR/../src/upnp/*.cxx) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/*.c) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/*.c++) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/*.cc) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/*.cpp) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/*.cxx) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/*.c) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/*.c++) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/*.cc) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/*.cpp) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/*.cxx) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/didl/*.c) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/didl/*.c++) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/didl/*.cc) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/didl/*.cpp) \
        $$quote($$BASEDIR/../src/upnp/controlpoint/av/didl/*.cxx) \
        $$quote($$BASEDIR/../src/upnp/device/*.c) \
        $$quote($$BASEDIR/../src/upnp/device/*.c++) \
        $$quote($$BASEDIR/../src/upnp/device/*.cc) \
        $$quote($$BASEDIR/../src/upnp/device/*.cpp) \
        $$quote($$BASEDIR/../src/upnp/device/*.cxx) \
        $$quote($$BASEDIR/../src/upnp/device/av/*.c) \
        $$quote($$BASEDIR/../src/upnp/device/av/*.c++) \
        $$quote($$BASEDIR/../src/upnp/device/av/*.cc) \
        $$quote($$BASEDIR/../src/upnp/device/av/*.cpp) \
        $$quote($$BASEDIR/../src/upnp/device/av/*.cxx)

    HEADERS += \
        $$quote($$BASEDIR/../include/*.h) \
        $$quote($$BASEDIR/../include/*.h++) \
        $$quote($$BASEDIR/../include/*.hh) \
        $$quote($$BASEDIR/../include/*.hpp) \
        $$quote($$BASEDIR/../include/*.hxx) \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}.ts)
