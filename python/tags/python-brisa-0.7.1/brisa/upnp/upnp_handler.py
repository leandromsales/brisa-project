# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Autoconfiguration for device's XML file and SSDP server.
"""

from os import path, mkdir

from brisa.upnp.message_handle.device_description_message import\
DeviceDescriptionMessage
from brisa.upnp.ssdp import SSDPServer
from brisa.threading import ThreadManager
from brisa import log, config


class DeviceHandler(object):
    """ Configures the device by setting up a SSDP server and the XML
    description file.

    This class wraps the complexity of these crucial steps in a small set of
    methods. A basic use is to instantiate it, use config_device() passing your
    device to configure it and set up the SSDP server and then starting the
    device's SSDP server with start_device() whenever you want. You may stop it
    when leaving with the stop_device() method.
    """

    def __init__(self, device_name):
        """ Constructor for the DeviceHandler class.

        @param device_name: device name that this class will apply to.
        """
        self.device_name = device_name
        self.SSDP = None
        self.device = None

    def config_device(self, device):
        """ Configures the device server for requests, generates and returns
        the XML description for the device.

        @param device: device

        @type device: Device

        @return: 2-tuple (xml_filename, xml_filepath)
        @rtype: tuple of strings
        """
        self.device = device
        ddesc = DeviceDescriptionMessage(device, device.location)
        xml_file_name = "%s-root-device.xml" % self.device_name
        xml_file_name = xml_file_name.replace(" ", "")
        xml_file_path = path.join(config.conf.brisa_home, 'tmp_xml')
        if not path.exists(xml_file_path):
            mkdir(xml_file_path)
        xml_file_path = path.join(xml_file_path, xml_file_name)
        ddesc.generate_xml(xml_file_path)
        self.SSDP = SSDPServer(self.device_name, xml_file_name)
        self.SSDP.register_root_device(device)
        self.SSDP.start()
        return (xml_file_name, xml_file_path)

    def start_device(self):
        """ Starts the device's SSDP server.
        """
        self.SSDP.announce_device()

    def stop_device(self):
        """ Stops the device's SSDP server.
        """
        log.debug("DeviceHandler: stopping device")
        if self.SSDP:
            self.SSDP.stop()
