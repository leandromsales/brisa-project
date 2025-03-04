# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from os import path

from xml.etree import ElementTree
from xml.etree.ElementTree import Element, SubElement, dump

from brisa import config
from brisa.upnp.upnp_defaults import UPnPDefaults
from brisa_userprofile_server.services.up_profile.persistence.database import User


class UserProfileDescriptionMessage(object):

    def __init__(self, username, password):
        self.xml = ""
        id = 0
        name = ""
        email = ""

        user = User.select(User.q.username == username)[0]
        if user:
            if (user.password == password):
                id = str(user.id)
                name = str(user.firstName) + str(user.lastName)
                email = user.email
        self.userInfo = [id, name, username, email, password]

    def create_xml_elements(self):

        self.xml = Element("root", xmlns = "urn:schemas-upnp-org:" +
                           UPnPDefaults.SCHEMA_VERSION)

        specVersion_element = SubElement(self.xml, "specVersion")
        element = SubElement(specVersion_element, "major")
        element.text = UPnPDefaults.SCHEMA_VERSION_MAJOR
        element = SubElement(specVersion_element, "minor")
        element.text = UPnPDefaults.SCHEMA_VERSION_MINOR

        userInfo_element = SubElement(self.xml, "userInfo")
        element = SubElement(userInfo_element, "userId")
        element.text = self.userInfo[0]
        element = SubElement(userInfo_element, "fullName")
        element.text = self.userInfo[1]
        element = SubElement(userInfo_element, "userName")
        element.text = self.userInfo[2]
        element = SubElement(userInfo_element, "email")
        element.text = self.userInfo[3]
        element = SubElement(userInfo_element, "password")
        element.text = self.userInfo[4]

    def dump_xml(self):
        self.create_xml_elements()
        self.to_xml(path.join(config.manager.brisa_home, '"userInfo"'))
        xml = ElementTree.tostring(self.xml, encoding='utf-8')
        return xml

    def to_xml(self, location):
        ElementTree.ElementTree(self.xml).write(location)
