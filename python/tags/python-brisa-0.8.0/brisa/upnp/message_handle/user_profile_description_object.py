# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import xml.etree.ElementTree as ET

from brisa.upnp.message_handle.device_object import DeviceObject
from brisa.utils.network import url_fetch
from brisa.upnp.upnp_defaults import UPnPDefaults


class UserProfileDescriptionObject(object):

    def __init__(self, fileName):
        self.fileName = fileName
        self.ns = UPnPDefaults.NAME_SPACE_XML_SCHEMA
        self.userProfile = None

    def to_object(self):
        self.tree = ET.parse(self.fileName)
        self.root = self.tree.getroot()
        self.userId = self.root.findtext('.//{%s}userId' % self.ns)
        self.fullName = self.root.findtext('.//{%s}fullName' % self.ns)
        self.userName = self.root.findtext('.//{%s}userName' % self.ns)
        self.email = self.root.findtext('.//{%s}email' % self.ns)

        self.userProfile = {"userId": self.userId, "fullName": self.fullName,
                            "userName": self.userName, "email": self.email}

        return self.userProfile
