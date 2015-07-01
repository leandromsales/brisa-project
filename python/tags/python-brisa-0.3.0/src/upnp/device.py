# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php

# Copyright 2006, Elisa team <contact@fluendo.com>
# Copyright 2007, Brisa team <brisa-develop@garage.maemo.org>

import sys

if sys.hexversion >= 0x20500f0:
    from xml.etree import ElementTree
else:
    import elementtree.ElementTree

import urllib2
from service import Service
import utils
from upnp_defaults import UPnPDefaults

class Device(object):

    def __init__(self, device_type_version, friendlyName, udn, location, parent=None):
        self.parent              = parent
        self.location            = location
        self.udn                 = udn
        self.friendly_name       = friendlyName
        self.manufacturer        = ""
        self.manufacturer_url    = ""
        self.model_description   = ""
        self.model_name          = ""
        self.model_number        = ""
        self.model_url           = ""
        self.serial_number       = ""
        self.upc                 = ""
        self.presentation_url    = ""

        self.set_device_type_version(device_type_version)

        self.services = []
        self.icons    = []
        self.webResourceList = []

    def get_device_type_version(self):
        return self.device_type_version

    def set_device_type_version(self, device_type_version):
        self.device_type_version = UPnPDefaults.DEVICE_TYPE_VERSION_PREFIX + device_type_version

    def set_complete_device_type_version(self, device_type_version):
        self.device_type_version = device_type_version

    def get_location(self):
        return self.location

    def get_friendly_name(self):
        return self.friendly_name

    def set_friendly_name(self, friendly_name):
        self.friendly_name = friendly_name

    def get_manufacturer(self):
        return self.manufacturer

    def set_manufacturer(self, manufacturer):
        self.manufacturer = manufacturer

    def get_manufacturer_url(self):
        return self.manufacturer_url

    def set_manufacturer_url(self, manufacturer_url):
        self.manufacturer_url = manufacturer_url

    def get_model_description(self):
        return self.model_description

    def set_model_description(self, model_description):
        self.model_description = model_description

    def get_model_name(self):
        return self.model_name

    def set_model_name(self, model_name):
        self.model_name = model_name

    def get_model_number(self):
        return self.model_number

    def set_model_number(self, model_number):
        self.model_number = model_number

    def get_model_url(self):
        return self.model_url

    def set_model_url(self, model_url):
        self.model_url = model_url

    def get_serial_number(self):
        return self.serial_number

    def set_serial_number(self, serial_number):
        self.serial_number = serial_number

    def get_udn(self):
        return self.udn

    def set_udn(self, udn):
        self.udn = udn

    def get_upc(self):
        return self.upc

    def get_listenPort(self):
        return int(self.location.split(":")[2])

    def set_upc(self, upc):
        self.upc = upc

    def get_presentation_url(self):
        return self.presentation_url

    def set_presentation_url(self, presentation_url):
        self.presentation_url = presentation_url

    def get_attribute_by_name(self, attribute_name):
        return getattr(self, attribute_name)

    def add_icon(self, icon):
        self.icons.append(icon)

    def get_icons(self):
        return self.icons

    def get_services(self):
        return self.services

    def add_service(self, service):
        self.services.append(service)

    def add_webResource(self, aResource):

        self.webResourceList.append(aResource)

    def get_webResourceList(self):
        return self.webResourceList

    def remove_service_with_usn(self, service_usn):
        for service in self.services:
            if service.get_usn() == service_usn:
                self.services.remove(service)
                break

    def is_rootDevice(self):
        return isinstance(self, RootDevice)

class DeviceIcon(object):

    def __init__(self, mimetype, width, height, depth, url):
        self.mimetype  = mimetype
        self.width     = width
        self.height    = height
        self.depth     = depth
        self.url       = url

    def get_mimetype(self):
        return self.mimetype

    def set_mimetype(self, mimetype):
        self.mimetype = mimetype

    def get_width(self):
        return self.width

    def set_width(self, width):
        self.width = width

    def get_height(self):
        return self.height

    def set_height(self, height):
        self.height = height

    def get_depth(self):
        return self.depth

    def set_depth(self, depth):
        self.depth = depth

    def get_url(self):
        return self.url

    def set_url(self, url):
        self.url = url

class RootDevice(Device):

    def __init__(self, device_type_version, friendlyName, udn, location, parent=None):
        Device.__init__(self, device_type_version, friendlyName, udn, location, parent)
        self.devices = []

    def add_device(self, device):
        self.devices.append(device)

    def get_devices(self):
        return self.devices
