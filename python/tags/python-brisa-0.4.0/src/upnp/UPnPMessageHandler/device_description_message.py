# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import sys

if sys.hexversion >= 0x20500f0:
    from xml.etree import ElementTree
    from xml.etree.ElementTree import Element, SubElement, dump
else:
    from elementtree import ElementTree
    from elementtree.ElementTree import Element, SubElement, dump

from device_message import DeviceMessage
from upnp.upnp_defaults import UPnPDefaults

class DeviceDescriptionMessage(object):

    def __init__(self, root_device, url_base=None):
        self.xml = ""
        self.root_device = root_device
        self.url_base = url_base

    def get_device(self):
        return device

    def create_xml_elements(self):
        #<root>
        self.xml = Element("root", xmlns = "urn:schemas-upnp-org:" + \
                               UPnPDefaults.SCHEMA_VERSION)
        #<specVersion>
        specVersion_element = SubElement(self.xml, "specVersion")
        element = SubElement(specVersion_element, "major")
        element.text = UPnPDefaults.SCHEMA_VERSION_MAJOR
        element = SubElement(specVersion_element, "minor")
        element.text = UPnPDefaults.SCHEMA_VERSION_MINOR

        #<urlBase>
        if self.url_base != None:
            element = SubElement(self.xml, "URLBase")
            element.text = self.url_base

        #<device>
        device_message = DeviceMessage(self.root_device)
        self.xml.append(device_message.create_description())

    def dumpXML(self):
        self.create_xml_elements()
        return dump(self.xml["root"])

    # To maintain compatibility
    def generateXML(self, fileName):
        self.toXML(fileName)

    def toXML(self, fileName):
        self.create_xml_elements()
        ElementTree.ElementTree(self.xml).write(fileName)
