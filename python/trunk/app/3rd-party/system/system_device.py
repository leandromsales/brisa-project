import os
from brisa.core.reactors import install_default_reactor
reactor = install_default_reactor()
from brisa.upnp.device import Device, Service
from system_info import get_battery_info, get_memory_info, get_cpu_info, get_temperature
from brisa.core.network import get_ip_address

class System(Device):

    def __init__(self):
        Device.__init__(self,
                        'urn:schemas-brisaupnp-com:device:System:1',
                        'My Simple System Device')
	self.manufacturer = "IC/UFAL" 

class SystemServices(Service):

    def __init__(self):
        Service.__init__(self,
                         'SystemServices',
                         'urn:schemas-brisaupnp-com:service:SystemServices:1',
                         '',
                         os.getcwd() + '/system_services.xml')
        self.LastResult = ""

    def GetBatteryStatus(self, *args, **kwargs):
        self.LastResult = get_battery_info() 
 	self.set_state_variable("LastResult", self.LastResult)
        return {'Level': self.LastResult}

    def GetTemperature(self, *args, **kwargs):
        self.LastResult = get_temperature()
	self.set_state_variable("LastResult", self.LastResult)
        return {'Temperature': self.LastResult}

    def GetMemoryUsage(self, *args, **kwargs):
        self.LastResult = get_memory_info()
	total = self.LastResult['total']
	free = self.LastResult['free']
	used = total - free
	self.set_state_variable("LastResult", self.LastResult)
        return {'Used': used,
		'Free': free}

    def GetCPUInfo(self, *args, **kwargs):
        self.LastResult = get_cpu_info() 
	self.set_state_variable("LastResult", self.LastResult)
        return {'Load': self.LastResult}

    def GetIP(self, *args, **kwargs):
        self.LastResult = get_ip_address(kwargs['Interface'])
	self.set_state_variable("LastResult", self.LastResult)
        return {'IP': self.LastResult}

if __name__ == '__main__':
    device = System()
    device += SystemServices()

    # Start device
    print "System device started."
    device.start()

    # Setup main loop
    reactor.add_after_stop_func(device.stop)
    reactor.main()

