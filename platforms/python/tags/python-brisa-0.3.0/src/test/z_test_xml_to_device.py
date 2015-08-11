# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php

# Copyright 2007, Brisa team

from upnp.UPnPMessageHandler import DeviceDescriptionObject

if __name__ == "__main__":
    deviceDescriptionObject = DeviceDescriptionObject("file:///C:/Documents%20and%20Settings/Leandro/workspace/Cartola/src/deviceDescription.xml")

    device = deviceDescriptionObject.toObject()
    print "Device: " + device.get_friendly_name()

    devices = device.get_devices()

    print "Embedded Devices: "
    for emb_dev in devices:
        print "  " + emb_dev.get_friendly_name()
