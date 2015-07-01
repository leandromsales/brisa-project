# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Device creation based on its description.
"""

from brisa.upnp.device import RootDevice, DeviceIcon
from brisa.upnp.service import Service
from brisa.upnp.upnp_defaults import UPnPDefaults


class DeviceObject(object):

    def __init__(self, location, tree):
        self.location = location
        self.tree = tree
        self.ns = UPnPDefaults.NAME_SPACE_XML_SCHEMA
        self.device = RootDevice("", "", self.location)

    def create_object(self):
        self._parse_device()
        self._parse_icons()
        self._parse_services()
        self._parse_embedded_devices()
        self.device.generate_soap_services()
        return self.device

    def _parse_device(self):
        self.device.device_type = self.tree.\
                                     findtext('.//{%s}deviceType' % self.ns)
        self.device.friendly_name = self.tree.\
                                  findtext('.//{%s}friendlyName' % self.ns)
        self.device.manufacturer = self.tree.\
                                     findtext('.//{%s}manufacturer' % self.ns)
        self.device.manufacturer_url = self.tree.\
                                 findtext('.//{%s}manufacturerURL' % self.ns)
        self.device.model_description = self.tree.\
                              findtext('.//{%s}modelDescription' % self.ns)
        self.device.model_name = self.tree.\
                                   findtext('.//{%s}modelName' % self.ns)
        self.device.model_number = self.tree.\
                                     findtext('.//{%s}modelNumber' % self.ns)
        self.device.model_url = self.tree.\
                                  findtext('.//{%s}modelURL' % self.ns)
        self.device.serial_number = self.tree.\
                                  findtext('.//{%s}serialNumber' % self.ns)
        self.device.udn = self.tree.findtext('.//{%s}UDN' % self.ns)
        self.device.upc = self.tree.findtext('.//{%s}UPC' % self.ns)
        self.device.presentation_url = self.tree.\
                                 findtext('.//{%s}presentationURL' % self.ns)

    def _parse_services(self):
        for xml_service_element in self.tree.\
                                    findall('.//{%s}service' % self.ns):
            service_type = xml_service_element.\
                                    findtext('{%s}serviceType' % self.ns)
            service_id = xml_service_element.\
                                    findtext('{%s}serviceId' % self.ns)
            control_url = xml_service_element.\
                                    findtext('{%s}controlURL' % self.ns)
            event_sub_url = xml_service_element.\
                                    findtext('{%s}eventSubURL' % self.ns)
            presentation_url = xml_service_element.\
                                    findtext('{%s}presentationURL' % self.ns)
            scpd_url = xml_service_element.\
                                    findtext('{%s}SCPDURL' % self.ns)
            service = Service(self.location, control_url, event_sub_url,
                              presentation_url, scpd_url)
            service.service_type = service_type
            service.service_id = service_id
            self.device.services[service_type] = service

    def _parse_icons(self):
        if self.tree.findtext('.//{%s}IconList' % self.ns) != None:
            for xml_icon_element in self.tree.findall('.//{%s}icon' % self.ns):
                mimetype = xml_icon_element.findtext('{%s}mimetype' % self.ns)
                width = xml_icon_element.findtext('{%s}width' % self.ns)
                height = xml_icon_element.findtext('{%s}height' % self.ns)
                depth = xml_icon_element.findtext('{%s}depth' % self.ns)
                url = xml_icon_element.findtext('{%s}url' % self.ns)

                icon = DeviceIcon(mimetype, width, height, depth, url)
                self.device.icons.append(icon)

    def _parse_embedded_devices(self):
        device_list = self.tree.find('.//{%s}deviceList' % self.ns)
        if device_list != None:
            embedded_device_tag = device_list.findall('.//{%s}device' %
                                                      self.ns)

            append = self.device.devices.append
            [append(DeviceObject(self.location,
                                  xml_device_element).create_object()) \
                                  for xml_device_element in \
                                                        embedded_device_tag]
