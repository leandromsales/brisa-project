# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>
#
# Implementation of a UPnP Media Renderer reference 1.0

import sys
import gtk
import dbus.mainloop.glib
import dbus.service

from os.path import join, dirname

from brisa import log, config
from brisa.threading import thread_manager

from brisa.upnp.upnp_handler import DeviceHandler
from brisa.upnp.device import RootDevice
from brisa.upnp.service import Service

from brisa.services.web_server import WebServer, StaticFile
from brisa.services.connmgr import ConnectionManagerRendererControl

from brisa_media_renderer.services.avtransport import AVTransportControl
from brisa_media_renderer.services.render_control import RenderingControl


# XML descriptions directory path
xml_dir_path = join(dirname(__file__), 'xml_descriptions')


# DBus constants
DBUS_OBJECT_PATH = '/br/edu/ufcg/embedded/brisa/MediaRenderer'
DBUS_IF = 'br.edu.ufcg.embedded.brisa.MediaRenderer'


# Main Loop setup for DBus, enable threads to go along with GTK
dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
gtk.gdk.threads_init()


class MediaRendererDevice(dbus.service.Object):
    """ Media Renderer reference 1.0 implementation. Use should be basic
    start/stop and accessing the DBus interface.

    The dbus interface contains the following methods:

    halt() - Halts the media renderer
    get_renderer_info() - Returns a 6-tuple containing information about the
                          device. The format is (device version, brisa
                          framework version, application version, renderer
                          name, logging level, logging output)

    This media renderer contains a .service dbus file. This means that
    bringing up the media renderer through its dbus interface can be simply
    achieved by using the object path and interface constants defined above
    to retrieve the dbus object.
    """
    upnp_urn = 'urn:schemas-upnp-org:device:MediaRenderer:1'

    def __init__(self, server_name, listen_url=''):
        """ Constructor for the media renderer device.

        @param server_name: friendly name for the device
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

    @dbus.service.method(DBUS_IF)
    def halt(self):
        """ Halts the media renderer.
        """
        thread_manager.main_loop_quit()

    @dbus.service.method(DBUS_IF)
    def get_renderer_info(self):
        """ Returns a 6-tuple containing information about the device. The
        format is (device version, brisa framework version, application
        version, renderer name, logging level, logging output).

        @rtype: tuple
        """
        version = 'MediaRenderer V 1.0'
        brisa_version = config.get_parameter('brisa', 'version')
        app_version = config.get_parameter('media_renderer', 'version')
        name = config.get_parameter('media_renderer', 'name')
        log_level = config.get_parameter('brisa', 'logging')
        log_output = config.get_parameter('brisa', 'logging_output')
        return (version, brisa_version, app_version, name, log_level,
                log_output)

    def _add_root_device(self):
        """ Creates the root device and saves the information about the
        device.
        """
        self.root_device = RootDevice(self.upnp_urn,
                                      self.server_name,
                                      self.listen_url)
        self.root_device.manufacturer = \
                            'BRisa Team. Embedded Laboratory and INdT Brazil'
        self.root_device.manufacturer_url = \
                                    'https://garage.maemo.org/projects/brisa'
        self.root_device.model_description = \
                                        'An Opensource UPnP A/V Media Renderer'
        self.root_device.model_name = 'Brisa Media Renderer version ' + \
                                    config.manager.brisa_version
        self.root_device.model_number = config.manager.brisa_version
        self.root_device.model_url = 'https://garage.maemo.org/projects/brisa/'
        self.root_device.serial_number = config.manager.brisa_version.\
                                                replace('.', '').rjust(7, '0')

    def _add_media_services(self):
        """ Creates the Service's objects and attachs them to the root device.
        """
        # Creating a ConnectionManager Service
        cm = Service(self.listen_url, 'ConnectionManager/control',
                     'ConnectionManager/event',
                     'ConnectionManager/presentation',
                     'ConnectionManager/scpd.xml')
        cm.service_id = 'ConnectionManager'
        cm.service_type = 'urn:schemas-upnp-org:service:ConnectionManager:1'
        cm.description_xml_filepath = join(xml_dir_path,
                                           'render-connmgr-scpd.xml')
        cm.control = ConnectionManagerRendererControl()
        # Creating a AVTransport Service
        avt = Service(self.listen_url, 'AVTransport/control',
                      'AVTransport/event',
                      'AVTransport/presentation',
                      'AVTransport/scpd.xml')
        avt.service_id = 'AVTransport'
        avt.service_type = 'urn:schemas-upnp-org:service:AVTransport:1'
        avt.description_xml_filepath = join(xml_dir_path,
                                            'render-transport-scpd.xml')
        avt.control = AVTransportControl()
        # Creating a RenderControl Service
        render = Service(self.listen_url, 'RenderControl/control',
                         'RenderControl/event',
                         'RenderControl/presentation',
                         'RenderControl/scpd.xml')
        render.service_id = 'RenderingControl'
        render.service_type = 'urn:schemas-upnp-org:service:RenderingControl:1'
        render.description_xml_filepath = join(xml_dir_path,
                                               'render-control-scpd.xml')
        render.control = RenderingControl(avt.control.get_player())
        self.root_device.services[cm.service_type] = cm
        self.root_device.services[avt.service_type] = avt
        self.root_device.services[render.service_type] = render

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

        [add_resource(name, resource) for name, resource in
                                             self.root_device.webResourceList]

    def _add_device_handler(self):
        """ Adds a device handler to the root device representing the media
        renderer and publishes the XML description on the web server.
        """
        self.device_handler = DeviceHandler(self.server_name)
        (xml_name, xml_path) = self.device_handler.config_device(
                                                            self.root_device)
        self.webserver.add_static_file(xml_name, StaticFile(xml_path))
        # Start device (starts SSDP server internally and announces device)
        self.device_handler.start_device()

    def start(self):
        """ Starts the device.
        """
        self._add_webserver()
        self._add_root_device()
        self._add_media_services()
        self._add_resources()
        self._add_device_handler()

        thread_manager.register_stop_function(self.stop)
        thread_manager.register_stop_function(gtk.main_quit)
        thread_manager.main_loop_async()
        gtk.main()

    def stop(self):
        """ Stops the device.
        """
        self.device_handler.stop_device()
