# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import SOAPpy

from upnp import utils
from control_point import ControlPoint
from upnp.UPnPMessageHandler.device_description_object import DeviceDescriptionObject
from media_services.cds.didl.DIDLLite import DIDLElement

from twisted.internet.task import LoopingCall

import time

class ControlPointManager(object):

    timeWait = 1

    def __init__(self, device_list_update_listener):
        self.server_devices = {}
        self.renderer_devices = {}
        self.control_point = ControlPoint()
        self.device_list_update_listener = device_list_update_listener
        self.search_device_loop = LoopingCall(self.tick_search_device)

    def _getDeviceObject(self, location):
        return DeviceDescriptionObject(location).toObject()

    def doubleSearchDevice(self):
        self.searchDevices()
        self.searchDevices()

    def get_service_control_url(self, service_list, service_type):
        for service in service_list:
            if (service.get_service_type_version().startswith(service_type)):
                break
        return service.get_control_url()

    def searchDevices(self):
        self.control_point.discover_media_server_devices()
        self.control_point.discover_media_renderer_devices()

        for dev in self.control_point.ssdp_know:
            deviceObject = self._getDeviceObject(self.control_point.ssdp_know[dev]['LOCATION'])
            deviceType = deviceObject.get_device_type_version()
            if (deviceType == 'urn:schemas-upnp-org:device:MediaServer:1'):
                self.server_devices[deviceObject.get_udn()] = deviceObject
                device_services = deviceObject.get_services()
                control_url = self.get_service_control_url(device_services, 'urn:schemas-upnp-org:service:ContentDirectory')
                deviceObject.SOAPService = SOAPpy.SOAPProxy(control_url)
            else:
                if (deviceType == 'urn:schemas-upnp-org:device:MediaRenderer:1'):
                    self.renderer_devices[deviceObject.get_udn()] = deviceObject
                    device_services = deviceObject.get_services()
                    control_url = self.get_service_control_url(device_services, 'urn:schemas-upnp-org:service:AVTransport')
                    deviceObject.SOAPService = SOAPpy.SOAPProxy(control_url)

    def getSearchCapabilites(self, udn):
        device = self.server_devices[udn]

        print "Search Capabilities in " + device.get_friendly_name() + ": " + device.contentDirectorySOAP.GetSearchCapabilities()

    def browse(self, id, udn, type="BrowseDirectChildren"):
        device = self.server_devices[udn]
        print "Browsing " + device.get_friendly_name() #+ ": "
        #browseResponse = cds_soap_proxy.Browse('0', 'BrowseMetadata', '*', '0', '0', '')
        browseResponse = device.SOAPService.Browse(id, type, '*', '0', '20', '')

        elt = DIDLElement.fromString(browseResponse['Result'])
        #for item in elt.getItems():
        #    print item.id + ": " + item.title
        return elt.getItems()

    def get_media_url(self, server_udn, media_id):
        device = self.server_devices[server_udn]

        browseResponse = device.SOAPService.Browse(media_id, "BrowseMetadata", '*', '0', '20', '')

        elt = DIDLElement.fromString(browseResponse['Result'])

        # first item (since it is a media item), first resource and its url (resource.data = url)
        item = elt.getItems()[0]
        return item.res[0].data

    def play(self, server_udn, renderer_udn, media_id):
        media_url = self.get_media_url(server_udn, media_id)
        device = self.renderer_devices[renderer_udn]
        # Set Renderer URI
        device.SOAPService.SetAVTransportURI(0, media_url, '')
        device.SOAPService.Play()

    def stop(self, server_udn, renderer_udn):
        device = self.renderer_devices[renderer_udn]
        device.SOAPService.Stop()

    def tick_search_device(self):
        self.searchDevices()
        self.device_list_update_listener._on_device_list_update()

    def start_search_device_loop(self):
        if not self.search_device_loop.running:
            self.search_device_loop.start(self.timeWait)

    def stop_search_device_loop(self):
        if self.search_device_loop.running:
            self.search_device_loop.stop()
