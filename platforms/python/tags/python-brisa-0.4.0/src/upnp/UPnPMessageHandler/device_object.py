# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import sys

if sys.hexversion >= 0x20500f0:
    from xml.etree import ElementTree
    from xml.etree.ElementTree import Element, SubElement, dump
else:
    import elementtree.ElementTree
    from elementtree.ElementTree import Element, SubElement, dump

from upnp import RootDevice
from upnp import Service
from upnp.upnp_defaults import UPnPDefaults

class DeviceObject(object):

    def __init__(self, location, tree):
        self.location = location
        self.tree = tree
        self.ns = UPnPDefaults.NAME_SPACE_XML_SCHEMA
        self.device = RootDevice("", "", "", "")
        self.SOAPService = None

    def create_object(self):
        self._parse_device()
        self._parse_icons()
        self._parse_services()
        self._parse_embedded_devices()
        return self.device

    def _parse_device(self):
        self.device.set_complete_device_type_version(self.tree.
                                     findtext('.//{%s}deviceType' % self.ns))
        self.device.set_friendly_name(self.tree.
                                  findtext('.//{%s}friendlyName' % self.ns))
        self.device.set_manufacturer(self.tree.
                                     findtext('.//{%s}manufacturer' % self.ns))
        self.device.set_manufacturer_url(self.tree.
                                 findtext('.//{%s}manufacturerURL' % self.ns))
        self.device.set_model_description(self.tree.
                              findtext('.//{%s}modelDescription' % self.ns))
        self.device.set_model_name(self.tree.
                                   findtext('.//{%s}modelName' % self.ns))
        self.device.set_model_number(self.tree.
                                     findtext('.//{%s}modelNumber' % self.ns))
        self.device.set_model_url(self.tree.
                                  findtext('.//{%s}modelURL' % self.ns))
        self.device.set_serial_number(self.tree.
                                  findtext('.//{%s}serialNumber' % self.ns))
        self.device.set_udn(self.tree.
                            findtext('.//{%s}UDN' % self.ns))
        self.device.set_upc(self.tree.
                            findtext('.//{%s}UPC' % self.ns))
        self.device.set_presentation_url(self.tree.
                                 findtext('.//{%s}presentationURL' % self.ns))

    def _parse_services(self):
        for xml_service_element in self.tree.\
                                    findall('.//{%s}service' % self.ns):
            service_type = xml_service_element.\
                                    findtext('{%s}serviceType' % self.ns)
            service_id = xml_service_element.\
                                    findtext('{%s}serviceId' % self.ns)
            control_url = xml_service_element.\
                                    findtext('{%s}controlURL' % self.ns)
            eventSub_url = xml_service_element.\
                                    findtext('{%s}eventSubURL' % self.ns)
            presentation_url = xml_service_element.\
                                    findtext('{%s}presentationURL' % self.ns)
            scpd_url = xml_service_element.\
                                    findtext('{%s}SCPDURL' % self.ns)
            service = Service(self.location, control_url,
                 eventSub_url, presentation_url, scpd_url)
            service.set_complete_service_type_version(service_type)
            service.set_complete_service_id(service_id)
            self.device.add_service(service)

    def _parse_icons(self):
        if self.tree.findtext('.//{%s}IconList' % self.ns) != None:
            for xml_icon_element in self.tree.findall('.//{%s}icon' % self.ns):
                mimetype = xml_icon_element.findtext('{%s}mimetype' % self.ns)
                width = xml_icon_element.findtext('{%s}width' % self.ns)
                height = xml_icon_element.findtext('{%s}height' % self.ns)
                depth = xml_icon_element.findtext('{%s}depth' % self.ns)
                url = xml_icon_element.findtext('{%s}url' % self.ns)

            icon = DeviceIcon(mimetype, width, height, depth, url)
            self.device.add_service(icon)

    def _parse_embedded_devices(self):
        deviceList = self.tree.find('.//{%s}deviceList' % self.ns)
        if deviceList != None:
            devices = deviceList.findall('.//{%s}device' % self.ns)
            for xml_device_element in devices:
                device_object = DeviceObject(self.location, xml_device_element)
                self.device.add_device(device_object.create_object())
