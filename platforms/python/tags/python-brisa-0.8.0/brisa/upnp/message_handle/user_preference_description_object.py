# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import xml.etree.ElementTree as ET

from brisa.upnp.message_handle.device_object import DeviceObject
from brisa.upnp.upnp_defaults import UPnPDefaults
from brisa.utils.network import url_fetch


class UserPreferenceDescriptionObject(object):

    def __init__(self, fileName):
        self.fileName = fileName
        self.ns = UPnPDefaults.NAME_SPACE_XML_SCHEMA
        self.userPreference = {}

    def to_object(self):
        self.tree = ET.parse(self.fileName)
        self.root = self.tree.getroot()
        userProfile = self.root.getchildren()[1]
        id = userProfile.attrib["id"]
        self.userPreference["id"]= id
        categoryList = userProfile.getchildren()[0]
        category = categoryList.getchildren()[0]
        categoryID = category.attrib["id"]
        categoryName = category.attrib["name"]
        self.userPreference[categoryName]= {}
        genreList = category.getchildren()[0]

        for genre in genreList.getchildren():
            genreID = genre.attrib["id"]
            genreTitle = genre.attrib["title"]
            artist = genre.findtext('.//{%s}artist' % self.ns)

            self.userPreference[categoryName].update({genreID: [genreTitle,
                                                                artist]})

        return self.userPreference
