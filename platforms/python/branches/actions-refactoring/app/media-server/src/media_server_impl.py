# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>
#
# Implementation of an UPnP Media Server reference 1.0

from brisa.core.reactors import GLib2Reactor
reactor = GLib2Reactor()

import os
import dbus.mainloop.glib
import dbus.service

from brisa.core import config
from brisa.upnp.device import Device, Service
from brisa.upnp.services.cds import ContentDirectory
from brisa.upnp.services.connmgr import ConnectionManagerServer

from brisa_media_server.services.media_registrar_ms import MSMediaRegistrar


# DBus constants
DBUS_OBJECT_PATH = '/br/edu/ufcg/embedded/brisa/MediaServer'
DBUS_IF = 'br.edu.ufcg.embedded.brisa.MediaServer'


# Main Loop setup for DBus
dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)


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

    def __init__(self, server_name, listen_url=''):
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
        self.device = None
        self.cds = None

    def _create_device(self):
        if self.xbox_comp:
            # Hack to work with Windows Media Center
            model_name = "Windows Media Connect"
        else:
            model_name = 'BRisa Media Server version %s' % \
                         config.manager.brisa_version

        project_page = 'http://brisa.garage.maemo.org'
        serial_no = config.manager.brisa_version.replace('.', '').rjust(7, '0')
        self.device = Device('urn:schemas-upnp-org:device:MediaServer:1',
                             self.server_name,
                             force_listen_url=self.listen_url,
                             manufacturer='BRisa Team. Embedded '\
                                          'Laboratory and INdT Brazil',
                             manufacturer_url=project_page,
                             model_description='An Open Source UPnP Media '\
                                               'Server',
                             model_name=model_name,
                             model_number=config.manager.brisa_version,
                             model_url=project_page,
                             serial_number=serial_no)

    def _add_services(self):
        """ Creates services 'content directory' and 'connection manager', both
        compliant with the UPnP A/V 1.0 specification.
        """
        # Save reference for use on some DBus methods
        self.cds = ContentDirectory(self.plugins_folder,
                                    self.plugins_module_path)
        cm = ConnectionManagerServer()

        self.device.add_service(self.cds)
        self.device.add_service(cm)

        if self.xbox_comp:
            local_xml_path = os.path.join(os.path.dirname(__file__),
                                          'xml_descriptions')
            msreg = MSMediaRegistrar(local_xml_path)
            self.device.add_service(msreg)

    def start(self):
        """ Starts the Media Server device.
        """
        self._create_device()
        self._add_services()
        self.device.start()
        reactor.add_after_stop_func(self.device.stop)
        reactor.main()

    # DBus Interface

    @dbus.service.method(DBUS_IF)
    def halt(self):
        """ Halts the media server.
        """
        reactor.main_quit()

    @dbus.service.method(DBUS_IF)
    def rescan_folders(self):
        if not self.cds:
            return
        pm = self.cds.plugin_manager
        # HACK: should we have access to plugins ? If so, fix this hack
        if not 'media_library' in pm.plugins_instances:
            return
        pm.plugins_instances['media_library'].process_all_folders()

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
