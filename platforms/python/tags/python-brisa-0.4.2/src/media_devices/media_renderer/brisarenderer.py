# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a simple daemon Application of UPnP Media Renderer
# It is based on the Twisted Framework

from optparse import OptionParser
import random
import string
import sys
import socket
import platform

from twisted.internet import gtk2reactor
gtk2reactor.install()
from twisted.internet import reactor

from upnp import RootDevice, Service, DeviceHandler

from media_services.avtransport import AVTransportControl
from media_services.connmgr import ConnectionManagerRendererControl
from media_services.render_control import RenderingControl

from twisted.python import log
from twisted.web import resource
from brisa_utils import network

import brisa_config


def main():
    logOn = False
    listenPort = 0

    usage = "\nBrisa UPnP MediaRenderet version= 0.4\nusage: python %prog \
[options] arg"

    parser = OptionParser(usage)
#    parser.add_option("-d", "--directory", dest="directory", metavar=\
#"\"name:path,name1:path1, ...\"",
#help="set content directories to share and its names")
    parser.add_option("-v", "--verbose",
                      action="store_true", dest="verbose")
    parser.add_option("-q", "--quiet",
                      action="store_false", dest="verbose")
    parser.add_option("-i", "--address", dest="address",
                      help="set the UPnP MediaRenderer IP address")
    parser.add_option("-p", "--port", dest="port",
                      help="set the UPnP MediaRenderer Port")

    (options, args) = parser.parse_args()

    if len(args) != 0:
        parser.error("incorrect number of arguments")

#    if options.directory is None:
#        parser.error("need to specify a directory name:path")

    if options.address is None:
        if brisa_config.contains("connection", "listenif"):
            interface = brisa_config.get_parameter("connection", "listenif")
            listenAddr = network.get_ip_address(interface)
        elif platform.uname()[4].find('arm') is not -1:
                listenAddr = network.get_ip_address('wlan0')
        elif platform.system() == 'Linux':
            listenAddr = network.get_ip_address('eth0')
    else:
        listenAddr = options.address

    if options.port is None:
        listenPort = random.randint(10000, 65000)
    else:
        listenPort = options.port

    listenURL = "http://" + str(listenAddr) + ":" + str(listenPort)

    print "Brisa UPnP Media Renderer v. 0.4"
    print "now, starting the renderer at " + listenURL
    print "press Ctrl+C or kill the process to stop the renderer"

    if options.verbose:
        logOn = True

    uuid = ''.join(['uuid:'] + [random.choice(string.letters) \
for x in xrange(20)])

    serverName = socket.gethostname() + " BrisaMediarenderer"

    renderDevice = RootDevice("MediaRenderer:1", serverName, listenURL)

    renderDevice.set_manufacturer("Brisa Team");
    renderDevice.set_manufacturer_url \
        ("https://garage.maemo.org/projects/brisa/");
    renderDevice.set_model_description("A Media Renderer Implementation");
    renderDevice.set_model_number("model 01");
    renderDevice.set_serial_number("0000001");

    # Creating a ConnectionManager Service
    myService = Service(listenURL, "ConnectionManager/control", \
                                   "ConnectionManager/event", \
                                   "ConnectionManager/presentation", \
                                   "ConnectionManager/scpd.xml")
    myService.set_service_id("11")
    myService.set_service_type_version("ConnectionManager:1")

    # Inserting a control to deal with the actions
    myService.add_description('media_services/connmgr/renderconnmgrSCPD.xml')
    myService.insertControl(ConnectionManagerRendererControl())

    # Creating a AVTransport Service
    myService2 = Service(listenURL, "AVTransport/control", \
                                    "AVTransport/event", \
                                    "AVTransport/presentation", \
                                    "AVTransport/scpd.xml")
    myService2.set_service_id("22")
    myService2.set_service_type_version("AVTransport:1")

    # Inserting a control to deal with the actions
    myService2.\
add_description('media_services/avtransport/rendertransportSCPD.xml')
    avtransport = AVTransportControl()
    myService2.insertControl(avtransport)

    # Creating a AVTransport Service
    myService3 = Service(listenURL, "RenderControl/control",\
                                    "RenderControl/event", \
                                    "RenderControl/presentation", \
                                    "RenderControl/scpd.xml")
    myService3.set_service_id("33")
    myService3.set_service_type_version("RenderingControl:1")

    # Inserting a control to deal with the actions
    myService3.\
add_description('media_services/render_control/rendercontrolSCPD.xml')
    myService3.insertControl(RenderingControl(avtransport.get_player()))

    # Adding the services into the device
    renderDevice.add_service(myService)
    renderDevice.add_service(myService2)
    renderDevice.add_service(myService3)

    # Creating a handler for the Device
    deviceHandler = DeviceHandler()

    #or you can use the reactor from Twisted. You just need to
    #configuring the device and then start the reactor
    deviceHandler.config_device(renderDevice, logOn, serverName)
    reactor.run()