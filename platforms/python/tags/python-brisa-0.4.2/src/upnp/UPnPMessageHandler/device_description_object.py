# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import sys

from xml.etree import ElementTree

from device_object import DeviceObject
import upnp.utils

class DeviceDescriptionObject(object):

    def __init__(self, location):
        self.location = location

    def toObject(self):
        handle = upnp.utils.url_fetch(self.location)
        if not handle:
            return

        tree = ElementTree(file=handle).getroot()
        return (DeviceObject(self.location, tree)).create_object()