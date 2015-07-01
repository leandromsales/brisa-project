from brisa.core.reactors import install_default_reactor
reactor = install_default_reactor()
from brisa.core.threaded_call import run_async_function
from brisa.upnp.control_point.control_point import ControlPoint


service = 'urn:schemas-brisaupnp-com:service:CalculatorOperations:1'
type = 'urn:schemas-brisaupnp-com:device:Calculator:1'
c = ControlPoint()


def get_service(device):
    service_ref = device.services[service]
    print service_ref.device
    return service_ref


def _exit(c):
    """ Stops the _handle_cmds loop
    """
    global running_handle_cmds
    running_handle_cmds = False
    

def _help(c):
    """ Prints the available commands that are used in '_handle_cmds' method.
    """
    print 'Available commands: '
    for x in ['exit', 'help', 'search', 'set_calc <device num>',
    	      'sum <op1> <op2>', 'subtract <op1> <op2>',
	      'mutiplies <op1> <op2>', 'divide <op1> <op2> expression <expr>',
	      'stop', 'list']:
        print '\t%s' % x
    
    
def _search(c):
    """ Start searching for devices of type upnp:rootdevice and repeat
    search every 600 seconds (UPnP default)
    """
    # just for testing proposal
    # additionals = {'OWN_FIELD1': 'value1', 'OWN_FIELD2': 'value2'}
    # c.start_search(600, type, http_version='1.1', man='ssdp:discover', mx=1, additionals=additionals)
    c.start_search(600, type)


def _stop(c):
    """ Stop searching
    """
    c.stop_search()


def _list_devices(c):
    """ Lists the devices that are in network.
    """
    k = 0
    for d in c.get_devices().values():
        print 'Device no.:', k
        print 'UDN:', d.udn
        print 'Name:', d.friendly_name
        print 'Manufacturer:', d.manufacturer
        print 'Device type:', d.device_type
        print 'Services:', d.services.keys() # Only print services name
        print 'Embedded devices:', [dev.friendly_name for dev in \
             d.devices.values()] # Only print embedded devices names
        print
        k += 1


def _set_calc(c, command):
    """ Gets the binary device by the number that is passed as parameter
    """
    try:
        devices = c.get_devices().values()
        c.current_calc = devices[int(command)]
        if c.current_calc and \
                    c.current_calc.device_type != type:
            print 'Please choose a Calculator device.'
            c.current_calc = None
    except:
        print 'Calculator number not found. Please run list and '\
              'check again'
        c.current_calc = None


def _sum(c, op1, op2):
    try:
        service = get_service(c.current_calc)
	
	#call_headers = {"Session": {"SeqNum": "1", "UUID": "uuid_value"}}
	# go with header
        #status_response = service.Sum(Op1=op1, Op2=op2, call_headers=call_headers)
	
	# go without header
        status_response = service.Sum(Op1=op1, Op2=op2)
	
	# always go with header
	#service.call_headers = call_headers 
	#status_response = service.Sum(Op1=op1, Op2=op2)
	#status_response = service.Sum(Op1=op1, Op2=op2)
	#status_response = service.Sum(Op1=op1, Op2=op2)

        # Other ways:
	#
	# call_headers = {"Session": {"SeqNum": "1", "UUID": "uuid_value"}}
	# service.call_headers = {"Session": {"SeqNum": "1"}}
	# status_response = service.Sum(Op1=op1, Op2=op2, call_headers=call_headers)
	#
	# NOTE: the call_header parameter for the remote service Sum in the above
	#       example, will replace the general call_header set in the
	#       line before, but will not erase it.
	#       So that, once service.call_header is set, it will be used in
	#       all calls after, except if the call_headers parameter is passed as
	#       an argument of the service call, like above. To avoid send header
	#       for a specific call just do either:
	#
	#       service.call_header = {} # will empty header for all next calls
	#       or
	#       service.Sum(Op1=op1, Op=op1, call_header={}) # will empty header
	#                                                    # only for this
	#                                                    # specific call.

	print status_response['Result']
	# print what is comming in the soap header
	print status_response['__header__']
    except Exception, e:
        print 'Error... exception: ', str(e)


def _subtract(c, op1, op2):
    try:
        service = get_service(c.current_calc)
        status_response = service.Subtract(Op1=op1, Op2=op2)
	print status_response['Result']
	print status_response['__header__']
    except Exception, e:
        print 'Error... exception: ', str(e)


def _multiplies(c, op1, op2):
    try:
        service = get_service(c.current_calc)
        status_response = service.Multiplies(Op1=op1, Op2=op2)
	print status_response['Result']
	print status_response['__header__']
    except Exception, e:
        print 'Error... exception: ', str(e)


def _divide(c, op1, op2):
    try:
        service = get_service(c.current_calc)
        status_response = service.Divide(Op1=op1, Op2=op2)
	print status_response['Result']
	print status_response['__header__']
    except Exception, e:
        print 'Error... exception: ', str(e)


def _expression(c, expr):
    try:
        service = get_service(c.current_calc)
        status_response = service.Expression(Expr=expr)
	print status_response['Result']
    except Exception, e:
        print 'Error... exception: ', str(e)


def on_new_device(dev):
    """ Callback triggered when a new device is found.
    """
    print 'Got new device:', dev.udn
    print "Type 'list' to see the whole list"
    # note that the fields is always in lowcase
    print "additional headers field UP: %s" % dev.additional_headers['up']
    _set_calc(c, 0)


def on_removed_device(udn):
    """ Callback triggered when a device leaves the network.
    """
    print 'Device is gone:', udn


def subscribe_for_discover_event(c):
    """ Creates the control point and binds callbacks to device events.
    """
    c.subscribe('new_device_event', on_new_device)
    c.subscribe('removed_device_event', on_removed_device)


# Control the loop at _handle_cmds function
running_handle_cmds = True
commands = {'exit': _exit, 
            'search': _search,
            'help': _help,
            'stop': _stop,
            'set_calc': _set_calc,
            'list': _list_devices,
            'sum': _sum,
            'subtract': _subtract,
            'multiplies': _multiplies,
            'divide': _divide,
            'expression': _expression}


def _handle_cmds(c):
    while running_handle_cmds:
        try:
            input = raw_input('>>> ').strip()
            if len(input.split(" ")) > 0:
                try:
		    command_parameters = input.split(" ")
                    if len(command_parameters) == 3:
                        commands[command_parameters[0]](c, command_parameters[1], command_parameters[2])
                    else:
		    	if len(command_parameters) == 2:
                        	commands[command_parameters[0]](c, command_parameters[1])
			else:
	                        commands[command_parameters[0]](c)
                except KeyError, IndexError:
                    print 'Invalid command, try help'
                except TypeError:
                    print 'Wrong usage, try help to see'
        except KeyboardInterrupt, EOFError:
            c.stop()
            break

    # Stops the main loop
    reactor.main_quit()


def main():
    """ Main loop iteration receiving input commands.
    """
    subscribe_for_discover_event(c)
    c.start()
    run_async_function(_handle_cmds, (c, ))
    reactor.add_after_stop_func(c.stop)
    reactor.main()


if __name__ == '__main__':
    main()
