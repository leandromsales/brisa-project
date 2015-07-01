import os

from brisa.threading import thread_manager
from brisa.upnp.device import RootDevice
from brisa.upnp.service import Service
from brisa.upnp.upnp_handler import DeviceHandler
from brisa.services.web_server import WebServer, StaticFile
from brisa.upnp.service import ServiceControl


class SwitchPower(ServiceControl):
    namespace = ('u', 'urn:schemas-upnp-org:service:SwitchPower:1')

    def __init__(self):
        ServiceControl.__init__(self)
        self.target = False
        self.status = False

    def soap_SetTarget(self, *args, **kwargs):
        self.target = kwargs['newTargetValue']
        print 'Light switched ', {'1': 'on', '0': 'off'}.get(self.target, None)

        # Status here is the target because our device is "perfect" and
        # no errors could occur (this is a simulation)
        self.status = self.target
        return {'SetTargetResponse': {}}

    def soap_GetTarget(self, *args, **kwargs):
        return {'GetTargetResponse': {'RetTargetValue': self.target}}

    def soap_GetStatus(self, *args, **kwargs):
        return {'GetStatusResponse': {'ResultStatus': self.status}}


class BinaryLight(object):

    def __init__(self):
        """ Constructor for the LightDevice class, which fills some basic
        information about the device.
        """
        self.cwd = os.getcwd()
        self.service_name = 'SwitchPower'
        self.server_name = 'Binary Light Device'
        self.listen_url = None
        self.root_device = None
        self.upnp_urn = 'urn:schemas-upnp-org:device:BinaryLight:1'

    def _add_root_device(self):
        """ Creates the root device object which will represent the device
        description.'
        """
        self.root_device = RootDevice(self.upnp_urn,
                                      self.server_name,
                                      self.listen_url)
        self.root_device.manufacturer = \
                        'Brisa Team. Embedded Laboratory and INdT Brazil'
        self.root_device.manufacturer_url = \
                        'https://garage.maemo.org/projects/brisa'
        self.root_device.model_name = 'Binary Light Device'
        self.root_device.model_description = \
                    'An UPnP Binary Light Device using BRisa UPnP Framework'
        self.root_device.model_number = '1.0'
        self.root_device.model_url = 'https://garage.maemo.org/projects/brisa/'

    def _add_services(self):
        # Creating the Service instance
        self.switch_power = Service(self.listen_url,
                                  'SwitchPower/control',
                                  'SwitchPower/event',
                                  'SwitchPower/presentation',
                                  'SwitchPower/scpd.xml')
        self.switch_power.service_id = 'SwitchPower'
        self.switch_power.service_type = \
                        'urn:schemas-upnp-org:service:SwitchPower:1'
        self.description_xml_filepath = self.cwd + '/SwitchPower-scpd.xml'
        self.switch_power.description_xml_filepath = \
            self.description_xml_filepath
        self.switch_power.control = SwitchPower()
        # Inserting a service into the root device
        self.root_device.services[self.switch_power.service_type] = \
                                                    self.switch_power

    def _add_webserver(self):
        # Creating the object webserver and setting It's listening url
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
        # Creating a handler for the Device
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
        self.webserver.start()
        # Start device (starts SSDP server internally and annouces device)
        self.device_handler.start_device()
        thread_manager.register_stop_function(self.stop)
        thread_manager.main_loop()

    def stop(self):
        self.device_handler.stop_device()


if __name__ == '__main__':
    device = BinaryLight()
    device.start()
