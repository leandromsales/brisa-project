# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a simple daemon Application of UPnP Media Server

from sys import exit
from os.path import join

from brisa import log, config
from brisa.threading import ThreadManager
from brisa.xml_descriptions import xml_path as xml_dir_path

from brisa.upnp.upnp_handler import DeviceHandler
from brisa.upnp.device import RootDevice
from brisa.upnp.service import Service

from brisa.services.cds import ContentDirectory
from brisa.services.connmgr import ConnectionManagerControl
from brisa.services.web_server import WebServer
from brisa.services.web_server.static import StaticFile
from brisa.services.media_registrar_ms import MS_MediaRegistrar


class MediaServerDevice(object):
    """ Media Server V1.0 device implementation.
    """

    xbox_comp = config.get_parameter_bool("brisa", "xbox_compatible")

    def __init__(self, server_name, listen_url):
        """ Constructor for the MediaServerDeviceClass.

        @param server_name: name of the media server that will appear on
        control points
        @param listen_url: url to publish resources and listen for requests

        @type server_name: string
        @type listen_url: string
        """
        self.server_name = server_name
        self.listen_url = listen_url
        self.root_device = None
        self.cds = None
        self.cm = None

    def _add_webserver(self):
        """ Creates the web server and sets the listening URL.
        """
        self.webserver = WebServer()
        self.webserver.listen_url = self.listen_url

    def _add_resources(self):
        """ Adds resources for services on the web server (publishing).
        """
        add_resource = self.webserver.add_resource
        for service in self.root_device.services.values():
            service_resource = service.start_service()
            add_resource(service.friendly_name, service_resource)

        [add_resource(name, resource)
                        for name, resource in self.root_device.webResourceList]

    def _add_device_handler(self):
        """ Adds a device handler to the root device representing the media
        server and publishes the XML description on the web server.
        """
        # Creating a handler for the Device
        self.device_handler = DeviceHandler(self.server_name)
        # Configuring the device
        (xml_name, xml_path) = self.device_handler.config_device(
                                                            self.root_device)
        self.webserver.add_static_file(xml_name, StaticFile(xml_path))

    def _load(self):
        """ Loads media server resources and services.
        """
        self._add_root_device()
        self._add_webserver()
        self._add_media_services()
        self._add_resources()
        self._add_device_handler()

    def start(self):
        """ Starts the Media Server device by loading and starting the
        resources and services.
        """
        # Load resources
        self._load()

        # Start webserver
        self.webserver.start()

        # Start plugin control
        self.cds.control.start()

        # Announce device
        self.device_handler.start_device()

        # Register stop function and block main loop
        ThreadManager().register_stop_function(self.stop)
        ThreadManager().main_loop()

        # TODO remove this forced exit (all threads must end)
        exit(0)

    def stop(self):
        """ Stops the device.
        """
        self.device_handler.stop_device()
        return True

    def _add_media_services(self):
        """ Adds the services to the root device.
        """
        # Create services
        self._create_server_services()

        # Adding the services into the device
        self.root_device.services[self.cds.service_type] = self.cds
        self.root_device.services[self.cm.service_type] = self.cm
        if self.xbox_comp:
            self.root_device.services[self.msreg.service_type] = self.msreg

    def _create_server_services(self):
        """ Creates services 'content directory' and 'connection manager', both
        compliant with the UPnP A/V 1.0 specification.
        """
        self.cds = Service(self.listen_url,
                           "ContentDirectory/control",
                           "ContentDirectory/event",
                           "ContentDirectory/presentation",
                           "ContentDirectory/scpd.xml")
        self.cds.service_id = "ContentDirectory"
        self.cds.service_type = \
                            "urn:schemas-upnp-org:service:ContentDirectory:1"
        self.cds.description_xml_filepath = join(xml_dir_path,
                                                 'content-directory-scpd.xml')

        self.cds.control = ContentDirectory()

        # Creating a ConnectionManager Service
        self.cm = Service(self.listen_url,
                          "ConnectionManager/control",
                          "ConnectionManager/event",
                          "ConnectionManager/presentation",
                          "ConnectionManager/scpd.xml")
        self.cm.service_id = "ConnectionManager"
        self.cm.service_type = \
                            "urn:schemas-upnp-org:service:ConnectionManager:1"
        self.cm.description_xml_filepath = join(xml_dir_path,
                                                'connection-manager-scpd.xml')
        self.cm.control = ConnectionManagerControl()

        if self.xbox_comp:
            self.msreg = Service(self.listen_url,
                                 "MSMediaReceiverRegistrar/control",
                                 "MSMediaReceiverRegistrar/event",
                                 "MSMediaReceiverRegistrar/presentation",
                                 "MSMediaReceiverRegistrar/scpd.xml")
            self.msreg.service_id = "X_MS_MediaReceiverRegistrar"
            self.msreg.service_type = \
                    "urn:microsoft.com:service:X_MS_MediaReceiverRegistrar:1"
            self.msreg.description_xml_filepath = join(xml_dir_path,
                                            "media-receiver-registrar-ms.xml")
            self.msreg.control = MS_MediaRegistrar()

    def _add_root_device(self):
        """ Creates the root device object which will represent the device
        description.
        """
        friendly_name = self.server_name
        self.root_device = RootDevice(
                                   "urn:schemas-upnp-org:device:MediaServer:1",
               "", self.listen_url)
        self.root_device.manufacturer = \
                            "BRisa Team. Embedded Laboratory and INdT Brazil"
        self.root_device.manufacturer_url = \
                                    "https://garage.maemo.org/projects/brisa/"
        self.root_device.model_description = \
                                        "An Opensource UPnP A/V Media Server"

        model_name = "BRisa Media Server version %s" % \
                                                config.conf.brisa_version
        if self.xbox_comp:
            model_name = "Windows Media Connect"

        self.root_device.friendly_name = friendly_name
        self.root_device.model_name = model_name
        self.root_device.model_number = config.conf.brisa_version
        self.root_device.model_url = "https://garage.maemo.org/projects/brisa/"
        self.root_device.serial_number = config.conf.brisa_version.\
                                                replace(".", "").rjust(7, "0")
