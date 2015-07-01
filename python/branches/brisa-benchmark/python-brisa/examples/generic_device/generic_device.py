# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from myservice import MyService
from brisa import log, config
from brisa.threading import thread_manager
from brisa.upnp.device import RootDevice
from brisa.upnp.service import Service
from brisa.upnp.event import EventManager
from brisa.upnp.upnp_handler import DeviceHandler
from brisa.services.web_server import WebServer, StaticFile
import os


class MyDevice(object):

    def __init__(self):
        """ Constructor for class MyDevice, which fill some basic information
        about the device.
        """
        self.cwd = os.getcwd()
        self.service_name = 'MyService'
        self.server_name = 'My Generic Device'
        self.listen_url = None
        self.root_device = None
        self.upnp_urn = 'urn:schemas-upnp-org:device:MyDevice:1'

    def _add_root_device(self):
        """ Creates the root device object which will represent the device
        description.
        """
        self.root_device = RootDevice(self.upnp_urn,
                                      self.server_name,
                                      self.listen_url)
        self.root_device.manufacturer = \
                        'Brisa Team. Embedded Laboratory and INdT Brazil'
        self.root_device.manufacturer_url = \
                        'https://garage.maemo.org/projects/brisa'
        self.root_device.model_name = 'Generic Device Example'
        self.root_device.model_description = \
                        'An Opensource UPnP generic Device'
        self.root_device.model_number = '1.0'
        self.root_device.model_url = 'https://garage.maemo.org/projects/brisa/'

    def _add_services(self):
        # Creating the example Service
        myservice = Service(self.listen_url,
                            'MyService/control',
                            'MyService/event',
                            'MyService/presentation',
                            'MyService/scpd.xml')
        myservice.service_id = 'MyService'
        myservice.service_type = \
                            'urn:schemas-upnp-org:service:MyService:1'
        myservice.description_xml_filepath = self.cwd + '/myservice-scpd.xml'
        myservice.control = MyService()
        myservice.event = EventManager(myservice)
        # Inserting a service into the root device
        self.root_device.services[myservice.service_type] = myservice

    def _add_webserver(self):
        # Creating the object webserver and setting it's listening url
        self.webserver = WebServer()
        self.webserver.start()
        self.listen_url = self.webserver.listen_url

    def _add_resources(self):
        # Adding resources into the webserver
        add_resource = self.webserver.add_resource
        for service in self.root_device.services.values():
            service_resource = service.start_service()
            add_resource(service.friendly_name, service_resource)
        [add_resource(name, resource)
                        for name, resource in self.root_device.webResourceList]

    def _add_device_handler(self):
        # Create a handler for the Device
        self.device_handler = DeviceHandler(self.server_name)
        # Configuring the device
        (xml_name, self.cwd) = self.device_handler.config_device(
                                                            self.root_device)

        self.webserver.add_static_file(xml_name, StaticFile(self.cwd))

    def _load(self):
        self._add_webserver()
        self._add_root_device()
        self._add_services()
        self._add_resources()
        self._add_device_handler()

    def start(self):
        # Load resources
        self._load()
        # Start webserver
        # Start device (starts SSDP server internally and annouces device)
        self.device_handler.start_device()
        thread_manager.register_stop_function(self.stop)
        thread_manager.main_loop()

    def stop(self):
        self.device_handler.stop_device()
        thread_manager.main_loop_quit()

if __name__ == '__main__':
    device = MyDevice()
    device.start()
