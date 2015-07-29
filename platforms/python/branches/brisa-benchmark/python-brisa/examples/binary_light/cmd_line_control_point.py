# Simple UPnP control point using BRisa framework
# Features:
# - Searching for devices
# - Listing devices
# - Handling events (new device located, removed device)


# 
import brisa
brisa.__skip_service_xml__ = False


from brisa.control_point import ControlPoint
from brisa.control_point.control_point import get_service_control_url
from brisa.threading import thread_manager
from brisa.upnp.soap import SOAPProxy
from brisa.utils.network import parse_url



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

    url_info = parse_url(dev.location)

    if dev.udn not in [d.udn for d in devices]:
        print 'Device found! Type is', dev.device_type
        if dev.device_type == binary_light_type:
            # Only insert if not already present and binary lights!
            devices.append(dev)
            control_url = get_service_control_url(dev.services, service[1])
            control_url = '%s://%s%s' % (url_info[0], url_info[1], control_url)
            dev.SOAPService = SOAPProxy(control_url, service)
            print 'Soap service created', dev.SOAPService


def on_removed_device(udn):
    """ Callback triggered when a device leaves the networ.
    """
    print 'Device is gone:', udn
    for dev in devices:
        # Find device whose udn is the one passed
        if dev.udn == udn:
            devices.remove(dev)


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

    # Runs the main loop in a separate thread, since the main thread is busy
    # pooling commands
    thread_manager.main_loop_async()

    while True:
        try:
            input = raw_input('>>> ')
        except KeyboardInterrupt, EOFError:
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
                print 'Services:', d.services.keys() # Only print services names
                print 'Embedded devices:', [dev.friendly_name for dev in \
                    d.devices] # Only print embedded devices names
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
                ss = c.current_server.SOAPService
                ss.soapaction = '%s#GetStatus' % service[1]
                status_response = ss.GetStatus()
                if status_response['ResultStatus'] == '1':
                    print 'Binary light status is on'
                else:
                    print 'Binary light status is off'
            except Exception, e:
                if not hasattr(c, 'current_server') or not c.current_server:
                    print 'BinaryLight device not set. Please use set_light <n>'
                else:
                    print 'Error in get_status():', e
        elif input.startswith('turn'):
            try:
                cmd = {'on': '1', 'off':'0'}.get(input.split(' ')[1], '')
                if not cmd:
                    print 'Wrong usage. Please try turn on or turn off.'
                    continue
                ss = c.current_server.SOAPService
                ss.soapaction = '%s#SetTarget' % service[1]
                ss.SetTarget(newTargetValue=cmd)
                print 'Turning binary light', input.split(' ')[1]
            except Exception, e:
                if not hasattr(c, 'current_server') or not c.current_server:
                    print 'BinaryLight device not set. Please use set_light <n>'
                else:
                    print 'Error in set_status():', e
        elif input == 'get_target':
            try:
                ss = c.current_server.SOAPService
                ss.soapaction = '%s#GetTarget' % service[1]
                status_response = ss.GetTarget()
                if status_response['RetTargetValue'] == '1':
                    print 'Binary light target is on'
                else:
                    print 'Binary light target is off'
            except Exception, e:
                if not hasattr(c, 'current_server') or not c.current_server:
                    print 'BinaryLight device not set. Please use set_light <n>'
                else:
                    print 'Error in  get_target():', e


    # Stops the main loop
    thread_manager.main_loop_stop()


if __name__ == '__main__':
    main()
