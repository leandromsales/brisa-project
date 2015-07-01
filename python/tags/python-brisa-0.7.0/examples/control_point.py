# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import sys
from brisa.control_point.control_point import ControlPoint
from brisa.main import ThreadManager


class CommandLineControlPoint(ControlPoint):

    def __init__(self):
        ControlPoint.__init__(self)

        self.subscribe('new_device_event', self.on_new_device)
        self.subscribe('remove_device_event', self.on_remove_device)

        self.devices_found = []

    def on_new_device(self, dev):
        self.devices_found.append(dev)

    def on_remove_device(self, udn):
        for dev in self.devices:
            if dev.udn == udn:
                self.devices_found.remove(dev)
                break

    def cmd_list_devices(self):
        n = 0
        for dev in self.devices_found:
            print 'device %d:' % n
            print '\tudn:', dev.udn
            print '\tfriendly_name:', dev.friendly_name
            print '\tservices:', dev.services
            print '\ttype:', dev.device_type
            if dev.devices:
                print '\tchild devices:'
                for child_dev in dev.devices:
                    print '\t\tudn:', child_dev.udn
                    print '\t\tfriendly_name:', child_dev.friendly_name
                    print '\t\tservices:', dev.services
                    print '\t\ttype:', child_dev.device_type
            print
            n += 1

    def run(self):
        exit = False
        try:
            while not exit:
                c = str(raw_input('>>> '))

                if c == 'start_search':
                    self.start_search(600, "upnp:rootdevice")
                    print 'search started!'
                elif c == 'stop_search':
                    self.stop_search()
                    print 'search stopped!'
                elif c == 'list':
                    self.cmd_list_devices()
                elif c == 'exit':
                    exit = True
                elif c == 'help':
                    print 'commands: start_search, stop_search, list, ' \
                          'exit, help'

                c = ''
        except KeyboardInterrupt, k:
            print 'quiting'

        ThreadManager().stop_all()
        return


def main():
    print "BRisa ControlPoint example\n"
    cmdline = CommandLineControlPoint()
    cmdline.run()
    sys.exit(0)

if __name__ == "__main__":
    main()
