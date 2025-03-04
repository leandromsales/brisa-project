# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a simple daemon Application of UPnP Media Server
# It is based on the Twisted Framework

from brisa import config
from brisa import log
from brisa.threading import ThreadManager
from brisa.xml_descriptions import xml_path as xml_dir_path

from brisa.upnp.device import RootDevice
from brisa.upnp.service import Service
from brisa.upnp.upnp_handler import DeviceHandler

from brisa.services.up_profile.up_authentication import UPAuthentication
from brisa.services.up_profile.up_profile import UPProfile
from brisa.services.web_server import WebServer, StaticFile

from os.path import join


class UserProfileServerDevice(object):

    upnp_urn = 'urn:schemas-upnp-org:device:UPServer:1'

    def __init__(self, server_name, listen_url):
        self.server_name = server_name
        self.listen_url = listen_url

    def _add_root_device(self):
        self.root_device = RootDevice(self.upnp_urn,
                                      self.server_name,
                                      self.listen_url)
        self.root_device.manufacturer = \
                            "BRisa Team. Embedded Laboratory and INdT Brazil"
        self.root_device.manufacturer_url = \
                                    "https://garage.maemo.org/projects/brisa/"
        self.root_device.model_description = \
                                       "An Opensource UPnP User Profile Server"

        model_name = "BRisa User Profile Server version %s" % \
                                                config.conf.brisa_version
        self.root_device.model_number = config.conf.brisa_version
        self.root_device.model_url = "https://garage.maemo.org/projects/brisa/"
        self.root_device.serial_number = config.conf.brisa_version.\
                                                replace(".", "").rjust(7, "0")

    def _add_media_services(self):
        # Creating a UP Authentication Service
        up_auth = Service(self.listen_url,
                           "UPAuthentication/control",
                           "UPAuthentication/event",
                           "UPAuthentication/presentation",
                           "UPAuthentication/scpd.xml")
        up_auth.service_id = "UPAuthentication"
        up_auth.service_type = \
                            "urn:schemas-upnp-org:service:UPAuthentication:1"
        up_auth.description_xml_filepath = join(xml_dir_path,
                                                 'up-authentication-scpd.xml')
        up_auth.control = UPAuthentication()

        # Creating a UP Profile Service
        up_profile = Service(self.listen_url,
                          "UPProfile/control",
                          "UPProfile/event",
                          "UPProfile/presentation",
                          "UPProfile/scpd.xml")
        up_profile.service_id = "UPProfile"
        up_profile.service_type = \
                            "urn:schemas-upnp-org:service:UPProfile:1"
        up_profile.description_xml_filepath = join(xml_dir_path,
                                                'up-profile-scpd.xml')
        up_profile.control = UPProfile()

        self.root_device.services[up_auth.service_type] = up_auth
        self.root_device.services[up_profile.service_type] = up_profile

    def _add_webserver(self):
        self.webserver = WebServer()
        self.webserver.listen_url = self.listen_url

    def _add_resources(self):
        add_resource = self.webserver.add_resource
        for service in self.root_device.services.values():
            service_resource = service.start_service()
            add_resource(service.friendly_name, service_resource)

        [add_resource(name, resource)
                        for name, resource in self.root_device.webResourceList]

    def _add_device_handler(self):
        # Creating a handler for the Device
        self.device_handler = DeviceHandler(self.server_name)
        # Configuring the device
        (xml_name, xml_path) = self.device_handler.config_device(
                                                            self.root_device)

        self.webserver.add_static_file(xml_name, StaticFile(xml_path))

    def _load(self):
        self._add_root_device()
        self._add_webserver()
        self._add_media_services()
        self._add_resources()
        self._add_device_handler()

    def start(self):
        # Load resources
        self._load()

        # Start webserver
        self.webserver.start()

        # Start device (starts SSDP server internally and announces device)
        self.device_handler.start_device()
        ThreadManager().register_stop_function(self.stop)
        ThreadManager().main_loop()

    def stop(self):
        self.device_handler.stop_device()
