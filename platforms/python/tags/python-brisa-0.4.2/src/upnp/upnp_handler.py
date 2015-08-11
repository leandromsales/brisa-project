# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a simple API for use in our UPnP Framework
import random
import string
import sys
import time
import thread
from time import sleep
import service
from ssdp import SSDPServer
from UPnPMessageHandler import DeviceDescriptionMessage
from twisted.web import resource, static, server
from twisted.internet import reactor

class WebServer(resource.Resource):

    def __init__(self):
        resource.Resource.__init__(self)


class DeviceHandler(object):

    def __init__(self):
        self.mySSDP = None
        self.is_threaded = False

    def config_device(self, device, logOn, deviceName):
        #Configure the Webserver and the SSDP server of the UPnP Device
        ddesc = DeviceDescriptionMessage(device, device.location)
        ddesc.generateXML("/tmp/"+deviceName+"root-device.xml")

        self.mySSDP = SSDPServer(logOn, deviceName)

        root = WebServer()

        root.putChild('root-device.xml',
                      static.File("/tmp/" + deviceName + "root-device.xml"))

        for service in device.services:
            serviceResource = service.startService()
            root.putChild(service.get_friendly_name(), serviceResource)

        for resource in device.webResourceList:
            root.putChild(resource[0], resource[1])

        site = server.Site(root)
        reactor.listenTCP(device.listenPort, site)

        self.mySSDP.registerRootDevice(device)
        self.mySSDP.announceDevice(device)

    def _callStages(self, aDevice, logOn):
        self.config_device(aDevice, logOn)
        reactor.run(0)

    # init device (network stuff) with its own thread (not the main one)
    def startDevice(self, aDevice, logOn):
        thread.start_new_thread(self._callStages, (aDevice, logOn))
        self.is_threaded = True
        return

    def stopDevice(self):
        #need to send bye bye message before stop reactor cause reactor is not
        #dealing with signals
        if self.is_threaded:
            self.mySSDP.doStop()
        reactor.stop()


#TODO: Implement some methods to simplify the use of Control Point objects
class ControlPointHandler(object):

    def __init__(self):
        pass