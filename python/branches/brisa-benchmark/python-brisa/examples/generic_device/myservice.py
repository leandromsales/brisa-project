# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.upnp.service import ServiceControl


class MyService(ServiceControl):
    namespace = ('u', 'urn:schemas-upnp-org:service:MyService:1')

    def __init__(self):
        ServiceControl.__init__(self)

    def start(self):
        pass

    def soap_MyMethod(self, *args, **kwargs):
        # Pay attention to the case sensitive arguments used here
        # and in the xml file you create for the service
        self.TextOut = kwargs['TextIn']
        return{'MyMethodResponse': {'TextOut': self.TextOut}}
