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

class DeviceMessage(object):

    def __init__(self, device):
        self.device = device
        self.elements = {}

    def set_body_value(self, name, value):
        self.body[name] = value

    def create_description(self):

        self.deviceElement = Element("device")

        self._create_device()
        self._create_iconList()
        self._create_serviceList()
        self._create_embeddedDevices()

        return self.deviceElement

    def _create_device(self):
        element = SubElement(self.deviceElement, "deviceType")
        element.text = self.device.get_device_type_version()

        element = SubElement(self.deviceElement, "friendlyName")
        element.text = self.device.get_friendly_name()

        element = SubElement(self.deviceElement, "manufacturer")
        element.text = self.device.get_manufacturer()

        element = SubElement(self.deviceElement, "manufacturerURL")
        element.text = self.device.get_manufacturer_url()

        element = SubElement(self.deviceElement, "modelDescription")
        element.text = self.device.get_model_description()

        element = SubElement(self.deviceElement, "modelName")
        element.text = self.device.get_model_name()

        element = SubElement(self.deviceElement, "modelNumber")
        element.text = self.device.get_model_number()

        element = SubElement(self.deviceElement, "modelURL")
        element.text = self.device.get_model_url()

        element = SubElement(self.deviceElement, "serialNumber")
        element.text = self.device.get_serial_number()

        element = SubElement(self.deviceElement, "UDN")
        element.text = self.device.get_udn()

        element = SubElement(self.deviceElement, "UPC")
        element.text = self.device.get_upc()

        element = SubElement(self.deviceElement, "presentationURL")
        element.text = self.device.get_presentation_url()

    def _create_iconList(self):
        #<device><iconList>
        device_icons = self.device.get_icons()
        if len(device_icons) > 0:
            icon_list_element = SubElement(self.deviceElement, "iconList")
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

    def _create_serviceList(self):
        #<device><serviceList>
        device_services = self.device.get_services()

        if len(device_services) > 0:

            service_list_element = SubElement(self.deviceElement,
                                                  "serviceList")
            for device_service in device_services:
                service_element = SubElement(service_list_element, "service")
                element = SubElement(service_element, "serviceType")
                element.text = device_service.get_service_type_version()

                element = SubElement(service_element, "serviceId")
                element.text = device_service.get_service_id()

                element = SubElement(service_element, "SCPDURL")
                element.text = device_service.get_scpd_url()

                element = SubElement(service_element, "controlURL")
                element.text = device_service.get_control_url()

                element = SubElement(service_element, "eventSubURL")
                element.text = device_service.get_event_sub_url()

    def _create_embeddedDevices(self):
        #<device><deviceList>
        if (self.device.is_rootDevice()):
            embedded_devices = self.device.get_devices()

            if len(embedded_devices) > 0:
                device_list_element = SubElement(self.deviceElement,
                                                     "deviceList")
                for embedded_device in embedded_devices:
                    embedded_device_description = DeviceMessage(
                                                            embedded_device)
                    device_list_element.append(embedded_device_description.\
                                                   create_description())
