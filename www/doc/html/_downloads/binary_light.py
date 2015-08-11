# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core.reactors import install_default_reactor
reactor = install_default_reactor()

import os

from brisa.upnp.device import Device, Service


class SwitchPower(Service):

    def __init__(self):
        Service.__init__(self, 'SwitchPower',
                         'urn:schemas-upnp-org:service:SwitchPower:1',
                         '',
                         os.getcwd() + '/SwitchPower-scpd.xml')
        self.target = False
        self.status = False

    def soap_SetTarget(self, *args, **kwargs):
        self.target = kwargs['newTargetValue']
        print 'Light switched ', {'1': 'on', '0': 'off'}.get(self.target, None)
        # Status here is the target because our device is "perfect" and
        # no errors could occur (this is a simulation)
        self.status = self.target
        return {}

    def soap_GetTarget(self, *args, **kwargs):
        return {'RetTargetValue': self.target}

    def soap_GetStatus(self, *args, **kwargs):
        return {'ResultStatus': self.status}



class BinaryLight(object):

    def __init__(self):
        """ Constructor for the LightDevice class, which fills some basic
        information about the device.
        """
        self.server_name = 'Binary Light Device'
        self.device = None

    def _create_device(self):
        """ Creates the root device object which will represent the device
        description.'
        """
        project_page = 'https://garage.maemo.org/projects/brisa'
        self.device = Device('urn:schemas-upnp-org:device:BinaryLight:1',
                             self.server_name,
                             manufacturer='Brisa Team. Embedded Laboratory '\
                                          'and INdT Brazil',
                             manufacturer_url=project_page,
                             model_name='Binary Light Device',
                             model_description='A UPnP Binary Light Device',
                             model_number='1.0',
                             model_url=project_page)

    def _add_services(self):
        switch = SwitchPower()
        self.device.add_service(switch)

    def start(self):
        self._create_device()
        self._add_services()
        self.device.start()
        reactor.add_after_stop_func(self.device.stop)
        reactor.main()


if __name__ == '__main__':
    device = BinaryLight()
    device.start()
