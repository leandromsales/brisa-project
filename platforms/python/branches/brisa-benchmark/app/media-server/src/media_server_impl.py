# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>
#
# Implementation of an UPnP Media Server reference 1.0

import os
import gtk
import dbus.mainloop.glib
import dbus.service

from brisa import log
from brisa import config
from brisa.threading import thread_manager
from brisa.xml_descriptions import xml_path as xml_dir_path

from brisa.upnp.upnp_handler import DeviceHandler
from brisa.upnp.device import RootDevice
from brisa.upnp.service import Service

from brisa.services.cds import ContentDirectory
from brisa.services.connmgr import ConnectionManagerControl
from brisa.services.web_server import WebServer
from brisa.services.web_server.static import StaticFile
from brisa_media_server.services.media_registrar_ms import MS_MediaRegistrar


# XML descriptions path
local_xml_path = os.path.join(os.path.dirname(__file__), 'xml_descriptions')


# DBus constants
DBUS_OBJECT_PATH = '/br/edu/ufcg/embedded/brisa/MediaServer'
DBUS_IF = 'br.edu.ufcg.embedded.brisa.MediaServer'


# Main Loop setup for DBus, enable threads to go along with GTK
dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
gtk.gdk.threads_init()


class MediaServerDevice(dbus.service.Object):
    """ Media Server reference 1.0 device implementation. Use should be basic
    start/stop and accessing the DBus interface.

    The dbus interface contains the following methods:

    halt() - Halts the media server
    get_server_info() - Returns a 7-tuple containing information about the
                        device. The format is (device version, brisa
                        framework version, application version, server name,
                        xbox compatibility, logging level, logging output)
    rescan_folders() - Forces the media server to rescan media library folders
    reload_config() - Forces the media server to load configurations made with
                      the configuration tool while running.

    This media server contains a .service dbus file. This means that
    bringing up the media server through its dbus interface can be simply
    achieved by using the object path and interface constants defined above
    to retrieve the dbus object.
    """

    xbox_comp = config.get_parameter_bool('media_server', 'xbox_compatible')
    plugins_folder = config.get_parameter('media_server', 'home') + '/plugins'
    plugins_module_path = 'brisa_media_server.plugins'

    def __init__(self, server_name, listen_url):
        """ Constructor for the media server device.

        @param server_name: friendly name for the server
        @param listen_url: a specific URL to be used for publishing resources
                           and listening for requests. If not specified, the
                           url will be http://IP:random_port

        @type server_name: string
        @type listen_url: string
        """
        bus_name = dbus.service.BusName(DBUS_IF, bus=dbus.SessionBus())
        dbus.service.Object.__init__(self, bus_name, DBUS_OBJECT_PATH)
        self.server_name = server_name
        self.listen_url = listen_url
        self.root_device = None
        self.cds = None
        self.cm = None

    @dbus.service.method(DBUS_IF)
    def halt(self):
        """ Halts the media server.
        """
        thread_manager.main_loop_quit()

    @dbus.service.method(DBUS_IF)
    def rescan_folders(self):
        if not self.cds:
            return
        # HACK: should we have access to plugins ? If so, fix this hack
        if not 'media_library' in \
            self.cds.control.plugin_manager.plugins_instances:
            return

        self.cds.control.plugin_manager.plugins_instances['media_library'].\
            process_all_folders()

    @dbus.service.method(DBUS_IF)
    def reload_config(self):
        config.manager.update()

    @dbus.service.method(DBUS_IF)
    def get_server_info(self):
        """ Returns a 7-tuple containing information about the device. The
        format is (device version, brisa framework version, application
        version, server name, xbox compatibility, logging level, logging
        output).

        @rtype: tuple
        """
        version = 'MediaServer V 1.0'
        brisa_version = config.get_parameter('brisa', 'version')
        app_version = config.get_parameter('media_server', 'version')
        server_name = config.get_parameter('media_server', 'name')
        xbox_compat = config.get_parameter('media_server', 'xbox_compatible')
        log_level = config.get_parameter('brisa', 'logging')
        log_output = config.get_parameter('brisa', 'logging_output')
        return (version, brisa_version, app_version, server_name, xbox_compat,
                log_level, log_output)

    def _add_webserver(self):
        """ Adds the webserver and sets up the URL.
        """
        self.webserver = WebServer()

        if self.listen_url:
            # URL specified under construction, use it
            self.webserver.listen_url = self.listen_url
            self.webserver.start()
        else:
            # Start it so that it generates the random port URL, save it for
            # later internal uses
            self.webserver.start()
            self.listen_url = self.webserver.listen_url

    def _add_resources(self):
        """ Adds resources for services to the webserver.
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
        self.device_handler.start_device()

    def start(self):
        """ Starts the Media Server device by loading and starting the
        resources and services.
        """
        self._add_webserver()
        self._add_root_device()
        self._add_media_services()
        self._add_resources()
        self._add_device_handler()

        # Start plugin control
        self.cds.control.start()

        # Register stop function and block main loop
        thread_manager.register_stop_function(self.stop)
        thread_manager.register_stop_function(gtk.main_quit)
        thread_manager.main_loop_async()
        gtk.main()

    def stop(self):
        """ Stops the device.
        """
        self.device_handler.stop_device()
        self.cds.control.stop()

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
        self.cds.description_xml_filepath = os.path.join(xml_dir_path,
                                                 'content-directory-scpd.xml')

        self.cds.control = ContentDirectory(self.plugins_folder,
                                            self.plugins_module_path)

        # Creating a ConnectionManager Service
        self.cm = Service(self.listen_url,
                          "ConnectionManager/control",
                          "ConnectionManager/event",
                          "ConnectionManager/presentation",
                          "ConnectionManager/scpd.xml")
        self.cm.service_id = "ConnectionManager"
        self.cm.service_type = \
                            "urn:schemas-upnp-org:service:ConnectionManager:1"
        self.cm.description_xml_filepath = os.path.join(xml_dir_path,
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
            self.msreg.description_xml_filepath = os.path.join(local_xml_path,
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
                                                config.manager.brisa_version
        if self.xbox_comp:
            model_name = "Windows Media Connect"

        self.root_device.friendly_name = friendly_name
        self.root_device.model_name = model_name
        self.root_device.model_number = config.manager.brisa_version
        self.root_device.model_url = "https://garage.maemo.org/projects/brisa/"
        self.root_device.serial_number = config.manager.brisa_version.\
                                                replace(".", "").rjust(7, "0")
