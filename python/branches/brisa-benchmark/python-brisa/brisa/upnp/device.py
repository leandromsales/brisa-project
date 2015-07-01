# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
#
# Copyright 2006, Elisa team <contact@fluendo.com>
# Copyright 2007-2008, Brisa team <brisa-develop@garage.maemo.org>

""" Device related classes.
"""

import uuid

from brisa.upnp.soap import SOAPProxy


class Device(object):
    """ Represents an UPnP device.

    Consult http://upnp.org/standardizeddcps/basic.asp as a basic reference.
    """

    def __init__(self, device_type, friendly_name, location, udn=None,
                 parent=None):
        """ Constructor for the Device class.

        @param device_type: device type as described on the device reference
        @param friendly_name: a friendly name
        @param location: network location
        @param udn: uuid
        @param parent: parent device

        @type device_type: string
        @type friendly_name: string
        @type location: string
        @type udn: string
        @type parent: Device

        @note: basic device reference:
        http://upnp.org/standardizeddcps/basic.asp
        """
        self.parent = parent
        self.location = location
        self.friendly_name = friendly_name
        self.manufacturer = ""
        self.manufacturer_url = ""
        self.model_description = ""
        self.model_name = ""
        self.model_number = ""
        self.model_url = ""
        self.serial_number = ""
        self.upc = ""
        self.presentation_url = ""
        self.device_type = device_type

        self.services = {}
        self.icons = []
        self.webResourceList = []
        self.SOAPService = None

        if udn == None:
            self.udn = self.generate_udn()
        else:
            self.udn = udn

    def generate_udn(self):
        """ Generates and returns a random uuid.

        @return: udn (random uuid)
        @rtype: string
        """
        usn = 'uuid:%s' % uuid.uuid4()
        return usn

    def remove_service_with_usn(self, service_usn):
        """ Removes service from device that matches the usn given.

        @param service_usn: service usn
        @type service_usn: string
        """
        for k, service in self.services.items():
            if service.get_usn() == service_usn:
                del self.services[k]
                break

    def is_root_device(self):
        """ Returns True if this device is a root device.
        """
        return isinstance(self, RootDevice)

    def generate_soap_services(self):
        """ Generates soap services for services and devices contained this
        device.
        """
        # Set SOAPService to each device service
        self._generate_soap_services_for_device(self)

        # Set SOAPService to each embedded child device
        for child_device in self.devices:
            self._generate_soap_services_for_device(child_device)

    def _generate_soap_services_for_device(self, device):
        """ Generates soap services for a single device

        @param device: device to generate soap services from
        @type device: Device
        """
        for k, service in device.services.items():
            namespace = ('u', service.service_type)
            service.SOAPService = SOAPProxy("%s%s" % \
                                            (service.url_base,
                                             service.control_url),
                                            namespace)


class DeviceIcon(object):
    """ Represents an icon of a device.
    """

    def __init__(self, mimetype, width, height, depth, url):
        """ Constructor for the DeviceIcon class.

        @param mimetype: mimetype for the icon
        @param width: icon width
        @param height: icon height
        @param depth: icon depth
        @param url: icon url

        @type mimetype: string
        @type width: string
        @type height: string
        @type depth: string
        @type url: string
        """
        self.mimetype = mimetype
        self.width = width
        self.height = height
        self.depth = depth
        self.url = url

    def get_mimetype(self):
        """ Returns icon's mimetype.

        @rtype: string
        """
        return self.mimetype

    def get_width(self):
        """ Returns icon's width.

        @rtype: string
        """
        return  self.width

    def get_height(self):
        """ Returns icon's height.

        @rtype: string
        """
        return self.height

    def get_depth(self):
        """ Returns icon's depth.

        @rtype: string
        """
        return self.depth

    def get_url(self):
        """ Returns icon's url.

        @rtype: string
        """
        return self.url


class RootDevice(Device):
    """ Represents an UPnP Root Device (a device that can contain other
    devices).
    """

    def __init__(self, device_type, friendly_name, location, udn=None,
                 parent=None):
        """ Constructor for the RootDevice class.

        @param device_type: device type as described on the device reference
        @param friendly_name: a friendly name
        @param location: network location
        @param udn: uuid
        @param parent: parent device

        @type device_type: string
        @type friendly_name: string
        @type location: string
        @type udn: string
        @type parent: Device

        @note: basic device reference:
        http://upnp.org/standardizeddcps/basic.asp
        """
        Device.__init__(self, device_type, friendly_name, location, udn,
                        parent)
        self.devices = []
