# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a simple daemon Application of UPnP Media Renderer

import sys
import gtk
import dbus.mainloop.glib
import dbus.service

from os.path import join

from brisa import log, config
from brisa.threading import ThreadManager
from brisa.xml_descriptions import xml_path as xml_dir_path

from brisa.upnp.upnp_handler import DeviceHandler
from brisa.upnp.device import RootDevice
from brisa.upnp.service import Service

from brisa.services.web_server import WebServer, StaticFile
from brisa.services.connmgr import ConnectionManagerRendererControl

from brisa_media_renderer.services.avtransport import AVTransportControl
from brisa_media_renderer.services.render_control import RenderingControl


DBUS_OBJECT_PATH = '/br/edu/ufcg/embedded/brisa/MediaRenderer'
DBUS_IF = 'br.edu.ufcg.embedded.brisa.MediaRenderer'

dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
gtk.gdk.threads_init()


class MediaRendererDevice(dbus.service.Object):
    upnp_urn = 'urn:schemas-upnp-org:device:MediaRenderer:1'

    def __init__(self, server_name, listen_url):
        bus_name = dbus.service.BusName(DBUS_IF, bus=dbus.SessionBus())
        dbus.service.Object.__init__(self, bus_name, DBUS_OBJECT_PATH)
        self.server_name = server_name
        self.server_name = server_name
        self.listen_url = listen_url

    @dbus.service.method(DBUS_IF)
    def halt(self):
        ThreadManager().stop_main_loop()

    @dbus.service.method(DBUS_IF)
    def get_renderer_info(self):
        version = 'MediaRenderer V 1.0'
        brisa_version = config.get_parameter('brisa', 'version')
        app_version = config.get_parameter('media_renderer', 'version')
        name = config.get_parameter('media_renderer', 'name')
        log_level = config.get_parameter('brisa', 'logging')
        log_output = config.get_parameter('brisa', 'logging_output')
        return (version, brisa_version, app_version, name, log_level,
                log_output)

    def _add_root_device(self):
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
        self.webserver = WebServer()
        self.webserver.listen_url = self.listen_url

    def _add_resources(self):
        add_resource = self.webserver.add_resource
        for service in self.root_device.services.values():
            service_resource = service.start_service()
            add_resource(service.friendly_name, service_resource)

        [add_resource(name, resource) for name, resource in
                                             self.root_device.webResourceList]

    def _add_device_handler(self):
        self.device_handler = DeviceHandler(self.server_name)
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
        ThreadManager().register_stop_function(gtk.main_quit)
        gtk.main()

    def stop(self):
        self.device_handler.stop_device()
        return True
