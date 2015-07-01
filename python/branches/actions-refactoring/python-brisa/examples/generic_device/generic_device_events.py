# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core.reactors import install_default_reactor
reactor = install_default_reactor()

import os

from brisa.core import log, config, webserver
from brisa.core.threaded_call import run_async_function
from brisa.upnp.device import Device
from brisa.upnp.event import EventManager
from brisa.upnp.upnp_handler import DeviceHandler

from myservice import MyService


class MyDevice(object):

    def __init__(self):
        """ Constructor for class MyDevice, which fill some basic information
        about the device.
        """
        self.cwd = os.getcwd()
        self.service_name = 'MyService'
        self.server_name = 'My Generic Device 2'
        self.root_device = None
        self.upnp_urn = 'urn:schemas-upnp-org:device:MyDevice:1'
        self.running = False
        self.commands = {'set_var': self._cmd_set_var,
                         'exit': self._exit,
                         'help': self._help}

    def _add_root_device(self):
        """ Creates the root device which will represent the device
        description.
        """

        project_page = 'http://brisa.garage.maemo.org'
        serial_no = config.manager.brisa_version.replace('.', '').rjust(7, '0')
        self.root_device = Device(self.upnp_urn,
                             self.server_name,
                             manufacturer='BRisa Team. Embedded '\
                                          'Laboratory and INdT Brazil',
                             manufacturer_url=project_page,
                             model_description='An Open Source UPnP generic '\
                                               'Device',
                             model_name='Generic Device Example',
                             model_number=config.manager.brisa_version,
                             model_url=project_page,
                             serial_number=serial_no)

    def _add_services(self):
        # Creating the example Service
        myservice = MyService(self.cwd)
        myservice.eventSub_controller = EventManager(myservice)

        # Inserting a service into the root device
        self.root_device.add_service(myservice)

    def _load(self):
        self._add_root_device()
        self._add_services()

    def run(self):
        self.running = True

        self._load()
        self.root_device.start()
        run_async_function(reactor.main)
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
        reactor.main_quit()
        self.root_device.stop()
        return

    def _cmd_set_var(self):
        service = self._select_service()
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
            print '\t\tservice_id: ' + serv.id
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
