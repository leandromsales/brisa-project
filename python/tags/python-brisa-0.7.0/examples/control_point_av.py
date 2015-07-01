# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import sys
from brisa.control_point.control_point_av import ControlPointAV
from brisa.main import ThreadManager


class CommandLineControlPointAV(ControlPointAV):

    def __init__(self):
        ControlPointAV.__init__(self)

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

    def cmd_set_server(self, id):
        self.current_server = self.devices_found[id]

    def cmd_set_render(self, id):
        self.current_renderer = self.devices_found[id]

    def cmd_browse(self, id):
        result = self.browse(id, 'BrowseDirectChildren', '*', 0, 10)['Result']
        for d in result:
            print "%s %s %s" % (d.id, d.title, d.upnp_class)

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
                elif c.startswith('browse'):
                    self.cmd_browse(c.split(' ')[1])
                elif c.startswith('set_server'):
                    self.cmd_set_server(int(c.split(' ')[1]))
                elif c.startswith('set_render'):
                    self.cmd_set_render(int(c.split(' ')[1]))
                elif c.startswith('play'):
                    self.play(c.split(' ')[1])
                elif c == 'exit':
                    exit = True
                elif c == 'help':
                    print 'commands: start_search, stop_search, list, ' \
                          'browse, set_server, set_render, play, exit, help'

                c = ''
        except KeyboardInterrupt, k:
            print 'quiting'

        ThreadManager().stop_all()
        return


def main():
    print "BRisa ControlPointAV example\n"
    cmdline = CommandLineControlPointAV()
    cmdline.run()
    sys.exit(0)

if __name__ == "__main__":
    main()
