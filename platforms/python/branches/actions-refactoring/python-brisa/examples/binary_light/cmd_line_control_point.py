# Simple UPnP control point using BRisa framework
# Features:
# - Searching for devices
# - Listing devices
# - Handling events (new device located, removed device)

from brisa.core.reactors import install_default_reactor
reactor = install_default_reactor()

from brisa.core.network import parse_url
from brisa.core.threaded_call import run_async_function

from brisa.upnp.control_point.control_point import ControlPoint


# List for holding devices
devices = []
service = ('u', 'urn:schemas-upnp-org:service:SwitchPower:1')
binary_light_type = 'urn:schemas-upnp-org:device:BinaryLight:1'


def on_new_device(dev):
    """ Callback triggered when a new device is found.
    """
    print 'Got new device:', dev.udn

    if not dev:
        return

    devices.append(dev)


def on_removed_device(udn):
    """ Callback triggered when a device leaves the networ.
    """
    print 'Device is gone:', udn
    for dev in devices:
        # Find device whose udn is the one passed
        if dev.udn == udn:
            devices.remove(dev)


def get_switch_service(device):
    return device.services[service[1]]

def create_control_point():
    """ Creates the control point and binds callbacks to device events.
    """
    c = ControlPoint()
    c.subscribe('new_device_event', on_new_device)
    c.subscribe('removed_device_event', on_removed_device)
    return c


def main():
    """ Main loop iteration receiving input commands.
    """
    c = create_control_point()
    c.start()
    run_async_function(_handle_cmds, (c, ))
    reactor.add_after_stop_func(c.stop)
    reactor.main()


def _handle_cmds(c):
    while True:
        try:
            input = raw_input('>>> ')
        except KeyboardInterrupt, EOFError:
            c.stop()
            break

        # Handle command
        if input == '':
            continue
        elif input == 'exit':
            break
        elif input == 'help':
            print 'Available commands: '
            for x in ['exit', 'help', 'search', 'set_light <dev number>',
                      'get_status', 'get_target', 'turn <on/off>', 'stop',
                      'list']:
                print '\t%s' % x
        elif input == 'stop':
            # Stop searching
            c.stop_search()
        elif input == 'search':
            # Start searching for devices of type upnp:rootdevice and repeat
            # search every 600 seconds (UPnP default)
            c.start_search(600, 'upnp:rootdevice')
        elif input == 'list':
            # List devices found
            k = 0
            for d in devices:
                print 'Device no.:', k
                print 'UDN:', d.udn
                print 'Name:', d.friendly_name
                print 'Device type:', d.device_type
                print 'Services:', d.services.keys() # Only print services name
                print 'Embedded devices:', [dev.friendly_name for dev in \
                    d.devices.values()] # Only print embedded devices names
                print
                k += 1
        elif input.startswith('set_light'):
            try:
                c.current_server = devices[int(input.split(' ')[1])]
            except:
                print 'BinaryLight number not found. Please run list and '\
                      'check again'
                c.current_server = None
        elif input == 'get_status':
            try:
                service = get_switch_service(c.current_server)
                status_response = service.GetStatus()
                if status_response['ResultStatus'] == '1':
                    print 'Binary light status is on'
                else:
                    print 'Binary light status is off'
            except Exception, e:
                if not hasattr(c, 'current_server') or not c.current_server:
                    print 'BinaryLight device not set.Please use set_light <n>'
                else:
                    print 'Error in get_status():', e
        elif input.startswith('turn'):
            try:
                cmd = {'on': '1', 'off': '0'}.get(input.split(' ')[1], '')
                if not cmd:
                    print 'Wrong usage. Please try turn on or turn off.'
                    continue
                service = get_switch_service(c.current_server)
                service.SetTarget(newTargetValue=cmd)
                print 'Turning binary light', input.split(' ')[1]
            except Exception, e:
                if not hasattr(c, 'current_server') or not c.current_server:
                    print 'BinaryLight device not set.Please use set_light <n>'
                else:
                    print 'Error in set_status():', e
        elif input == 'get_target':
            try:
                service = get_switch_service(c.current_server)
                status_response = service.GetTarget()
                if status_response['RetTargetValue'] == '1':
                    print 'Binary light target is on'
                else:
                    print 'Binary light target is off'
            except Exception, e:
                if not hasattr(c, 'current_server') or not c.current_server:
                    print 'BinaryLight device not set.Please use set_light <n>'
                else:
                    print 'Error in get_target():', e

    # Stops the main loop
    reactor.main_quit()


if __name__ == '__main__':
    main()
