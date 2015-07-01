# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core.reactors import install_default_reactor
reactor = install_default_reactor()

import pprint

from brisa.upnp.control_point import ControlPoint
from brisa.core.threaded_call import run_async_function


devices = []


def on_new_device(dev):
    print 'Got new device:', dev

    if dev.udn not in [d.udn for d in devices]:
        devices.append(dev)


def on_removed_device(udn):
    print 'Device is gone:', udn

    for dev in devices:
        if dev.udn == udn:
            devices.remove(dev)


def create():
    c = ControlPoint()
    c.subscribe('new_device_event', on_new_device)
    c.subscribe('removed_device_event', on_removed_device)
    return c


def list_devices(devices):
    count = 0
    for d in devices:
        print 'Device number: ', count
        print 'UDN (id): ', d.udn
        print 'Name: ', d.friendly_name
        print 'Device type:', d.device_type
        print 'Services:', d.services.keys()
        print 'Embedded devices: ',\
            [dev.friendly_name for dev in d.devices.values()]
        print
        count += 1


def list_services(dev):
    count = 0
    for k, serv in dev.services.items():
        print 'Service number: ', count
        print 'Service id: ' + serv.id
        print
        count += 1


def main():
    c = create()
    c.start()
    reactor.add_after_stop_func(c.stop)
    run_async_function(run, (c, ))
    reactor.main()


def run(c):
    while True:
        try:
            input = raw_input('>>> ')
        except KeyboardInterrupt, EOFError:
            break

        if input == '':
            print
            continue

        elif input == 'list':
            list_devices(devices)

        elif input == 'exit':
            break

        elif input == 'search':
            c.start_search(600, 'upnp:rootdevice')

        elif input == 'stop':
            c.stop_search()

        elif input.find('MyMethod') == 0:
            device = devices[int(input.split(' ')[1])]
            k, service = device.services.items()[0]

            response = service.MyMethod(TextIn="In!!")
            print "Return:", response

        elif input == 'help':
            print 'Commands available: list, exit,',
            'search, stop, help, MyMethod'

    reactor.main_quit()


if __name__ == '__main__':
    main()
