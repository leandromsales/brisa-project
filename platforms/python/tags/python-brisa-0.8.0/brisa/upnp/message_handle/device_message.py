# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Generates a description of a device.
"""

from xml.etree.ElementTree import Element, SubElement


class DeviceMessage(object):

    def __init__(self, device):
        self.device = device
        self.elements = {}

    def set_body_value(self, name, value):
        self.body[name] = value

    def create_description(self):

        self.device_element = Element("device")

        self._create_device()
        self._create_icon_list()
        self._create_service_list()
        self._create_embedded_devices()

        return self.device_element

    def _create_device(self):
        element = SubElement(self.device_element, 'dlna:X_DLNADOC')
        element.attrib['xmlns:dlna'] = 'urn:schemas-dlna-org:device-1-0'
        element.text = 'DMS-1.50'

        element = SubElement(self.device_element, 'dlna:X_DLNACAP')
        element.attrib['xmlns:dlna'] = 'urn:schemas-dlna-org:device-1-0'
        element.text = 'av-upload,image-upload,audio-upload'

        element = SubElement(self.device_element, "deviceType")
        element.text = self.device.device_type

        element = SubElement(self.device_element, "friendlyName")
        element.text = self.device.friendly_name

        element = SubElement(self.device_element, "manufacturer")
        element.text = self.device.manufacturer

        element = SubElement(self.device_element, "manufacturerURL")
        element.text = self.device.manufacturer_url

        element = SubElement(self.device_element, "modelDescription")
        element.text = self.device.model_description

        element = SubElement(self.device_element, "modelName")
        element.text = self.device.model_name

        element = SubElement(self.device_element, "modelNumber")
        element.text = self.device.model_number

        element = SubElement(self.device_element, "modelURL")
        element.text = self.device.model_url

        element = SubElement(self.device_element, "serialNumber")
        element.text = self.device.serial_number

        element = SubElement(self.device_element, "UDN")
        element.text = self.device.udn

        element = SubElement(self.device_element, "UPC")
        element.text = self.device.upc

        element = SubElement(self.device_element, "presentationURL")
        element.text = self.device.presentation_url

    def _create_icon_list(self):
        #<device><iconList>
        device_icons = self.device.icons
        if len(device_icons) > 0:
            icon_list_element = SubElement(self.device_element, "iconList")
            for device_icon in device_icons:
                icon_element = SubElement(icon_list_element, "icon")
                element = SubElement(icon_element, "mimetype")
                element.text = device_icon.get_mimetype()

                element = SubElement(icon_element, "width")
                element.text = device_icon.get_width()

                element = SubElement(icon_element, "height")
                element.text = device_icon.get_height()

                element = SubElement(icon_element, "depth")
                element.text = device_icon.get_depth()

                element = SubElement(icon_element, "url")
                element.text = device_icon.get_url()

#    def _create_service_list(self):
#        device_services = self.device.services
#
#        if len(device_services) > 0:
#
#            service_list_element = SubElement(self.device_element,
#                                              "serviceList")
#            for device_service in device_services:
#                service_element = SubElement(service_list_element, "service")
#                element = SubElement(service_element, "serviceType")
#                element.text = device_service.service_type
#
#                element = SubElement(service_element, "serviceId")
#                element.text = device_service.service_id
#
#                element = SubElement(service_element, "SCPDURL")
#                element.text = device_service.scpd_url
#
#                element = SubElement(service_element, "controlURL")
#                element.text = device_service.control_url
#
#                element = SubElement(service_element, "eventSubURL")
#                element.text = device_service.event_sub_url

    def _create_service_list(self):
        device_services = self.device.services

        if len(device_services) > 0:

            service_list_element = SubElement(self.device_element,
                                              "serviceList")
            for k, device_service in device_services.items():
                service_element = SubElement(service_list_element, "service")
                element = SubElement(service_element, "serviceType")
                element.text = device_service.service_type

                element = SubElement(service_element, "serviceId")
                element.text = device_service.service_id

                element = SubElement(service_element, "SCPDURL")
                element.text = device_service.scpd_url

                element = SubElement(service_element, "controlURL")
                element.text = device_service.control_url

                element = SubElement(service_element, "eventSubURL")
                element.text = device_service.event_sub_url

    def _create_embedded_devices(self):
        if self.device.is_root_device():
            embedded_devices = self.device.devices

            if len(embedded_devices) > 0:
                device_list_element = SubElement(self.device_element,
                                                 "deviceList")
                for embedded_device in embedded_devices:
                    embedded_device_description = DeviceMessage(
                                                            embedded_device)
                    device_list_element.append(embedded_device_description.
                                               create_description())
