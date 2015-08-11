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
        self.server_name = 'My Generic Device 2'
        self.listen_url = None
        self.root_device = None
        self.upnp_urn = 'urn:schemas-upnp-org:device:MyDevice:1'
        
        self.running = False
        self.commands = {'set_var': self._cmd_set_var,
                         'exit': self._exit,
                         'help': self._help}        

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

    def run(self):
        self.running = True
        
        self._load()
        self.device_handler.start_device()
        thread_manager.main_loop_async()
        try:
            while self.running:
                command = str(raw_input('>>> '))
                try:
                    self.commands[command]()
                except KeyError:
                    print 'invalid command, try help'
                command = ''
        except KeyboardInterrupt, k:
            pass
        except EOFError, k:
            pass
        self.device_handler.stop_device()
        thread_manager.main_loop_quit()
        return

    def _cmd_set_var(self):
        service =  self._select_service()
        name = raw_input('Insert a name for the variable: ')
        value = raw_input('Insert a value for the variable: ')
        service.set_eventing_variable(name, value)

    def _select_service(self):
        self.list_services(self.root_device)
        serv = int(raw_input('Insert the number of the service: '))
        k, service = self.root_device.services.items()[serv]
        return service

    def list_services(self, dev):
        print 'device friendly_name:', dev.friendly_name
        n = 0
        for k, serv in dev.services.items():
            print '\tservice %d:' % n
            print '\t\tservice_id: ' + serv.service_id
            print '\t\tsubscription_id: ' + serv.subscription_id
            print '\t\tevent_sid: ' + serv.event_sid
            print
            n += 1

    def _exit(self):
        self.running = False

    def _help(self):
        help = 'commands: '
        for k in self.commands.keys():
            help += k + ', '
        print help[:-2]


if __name__ == '__main__':
    device = MyDevice()
    device.run()
