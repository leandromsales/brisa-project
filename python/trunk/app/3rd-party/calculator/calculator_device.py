import os
from brisa.core.reactors import install_default_reactor
reactor = install_default_reactor()
from brisa.upnp.device import Device, Service

class Calculator(Device):

    def __init__(self):
        Device.__init__(self,
                        'urn:schemas-brisaupnp-com:device:Calculator:1',
                        'My Simple UPnP Calculator',
			additional_ssdp_headers={"UP": "Yes"})
	self.manufacturer = "IC/UFAL" 

service = "urn:schemas-brisaupnp-com:service:CalculatorOperations:1"
xmlfile = os.getcwd() + '/calculator_operations.xml'

class CalculatorOperations(Service):

    def __init__(self):
        Service.__init__(self, 'CalculatorOperations', service, '', xmlfile)
        self.LastResult = 0
	self.call_headers = {'General': {'SeqNum': '2', 'UUID': 'uuid_value'}}

    def Sum(self, *args, **kwargs):
    	print self.device
        operador1 = float(kwargs['Op1'])
        operador2 = float(kwargs['Op2'])
        self.LastResult = operador1 + operador2
	# reading header comming from the control point
	#print kwargs['__header__']['Session']['SeqNum']['__value__']
        return {'Result': self.LastResult,
               # replacing the default header defined in the __init__
	        '__header__': {'Session': {'SeqNum': '2'}}}

    def Subtract(self, *args, **kwargs):
        operador1 = float(kwargs['Op1'])
        operador2 = float(kwargs['Op2'])
        self.LastResult = operador1 - operador2
	# avoiding sending header defined in the __init__()
        return {'Result': self.LastResult,
	        '__header__': {}}

    def Multiplies(self, *args, **kwargs):
        operador1 = float(kwargs['Op1'])
        operador2 = float(kwargs['Op2'])
        self.LastResult = operador1 * operador2
	# this will use the header defined in the __init__()
        return {'Result': self.LastResult}

    def Divide(self, *args, **kwargs):
        operador1 = float(kwargs['Op1'])
        operador2 = float(kwargs['Op2'])
        self.LastResult = operador1 / operador2
        return {'Result': self.LastResult}

    def Expression(self, *args, **kwargs):
        self.LastResult = eval(kwargs['Expr']) # ATTENTION: security issues
					       # using eval function.
        return {'Result': self.LastResult}

if __name__ == '__main__':
    device = Calculator()
    device += CalculatorOperations() 

    # Start device
    print "Calculator started."
    device.start()

    # Setup main loop
    reactor.add_after_stop_func(device.stop)
    reactor.main()

