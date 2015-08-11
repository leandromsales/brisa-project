# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core.reactors import install_default_reactor
reactor = install_default_reactor()

import sys
import time
import thread

from brisa.upnp.control_point.control_point import ControlPoint
from brisa.core.threaded_call import run_async_function


class CommandLineControlPoint(ControlPoint):

    def __init__(self):
        ControlPoint.__init__(self)
        self.running = False
        self._initial_subscribes()
        self.devices_found = []
        self.commands = {'search': self._search,
                         'stop': self._stop,
                         'exit': self._exit,
                         'list': self._cmd_list_devices,
                         'subscribe': self._cmd_subscribe,
                         'unsubscribe': self._cmd_unsubscribe,
                         'help': self._help}

    def _initial_subscribes(self):
        self.subscribe('new_device_event', self.on_new_device)
        self.subscribe('removed_device_event', self.on_remove_device)
        self.subscribe('device_event', self.on_device_event)

    def on_new_device(self, dev):
        self.devices_found.append(dev)

    def on_remove_device(self, udn):
        device_found = None
        for dev in self.devices:
            if dev.udn == udn:
                device_found = dev
                break
        self.devices_found.remove(device_found)

    def on_device_event(self, sid, changed_vars):
        print
        print "device event"
        print "sid: " + sid
        print changed_vars

    def _cmd_list_devices(self):
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

    def _cmd_subscribe(self):
        service = self._select_service()
        service.event_subscribe(self.event_host,
                                self._event_subscribe_callback, None,
                                True, self._event_renew_callback)
        time.sleep(0.5)

    def _cmd_unsubscribe(self):
        service = self._select_service()
        service.event_unsubscribe(self.event_host,
            self._event_unsubscribe_callback, None)
        time.sleep(0.5)

    def _select_service(self):
        dev = int(raw_input('Insert the number of the device: '))
        device = self.devices_found[dev]
        self.list_services(device)
        serv = int(raw_input('Insert the number of the service: '))
        k, service = device.services.items()[serv]
        return service

    def list_services(self, dev):
        print 'device friendly_name:', dev.friendly_name
        n = 0
        for k, serv in dev.services.items():
            print '\tservice %d:' % n
            print '\t\tservice_id: ' + serv.id
            print '\t\tevent_sid: ' + serv.event_sid
            print
            n += 1

    def _event_subscribe_callback(self, cargo, subscription_id, timeout):
        print
        print "Event subscribe done!"
        print 'Subscription ID: ' + str(subscription_id[5:])
        print 'Timeout: ' + str(timeout)

    def _event_renew_callback(self, cargo, subscription_id, timeout):
        print
        print "Event renew done!"
        print 'Subscription ID: ' + str(subscription_id[5:])
        print 'Timeout: ' + str(timeout)

    def _event_unsubscribe_callback(self, cargo, old_subscription_id):
        print
        print "Event unsubscribe done!"
        print 'Old subscription ID: ' + str(old_subscription_id[5:])

    def _search(self):
        self.start_search(600, 'upnp:rootdevice')

    def _stop(self):
        self.stop_search()

    def _exit(self):
        self.running = False

    def _help(self):
        help = 'commands: '
        for k in self.commands.keys():
            help += k + ', '
        print help[:-2]

    def run(self):
        self.running = True
        self.start()
        reactor.add_after_stop_func(self.stop)
        thread.start_new_thread(self._handle_cmds, ())
        reactor.main()

    def _handle_cmds(self):
        try:
            while self.running:
                command = str(raw_input('>>> '))
                try:
                    self.commands[command]()
                except KeyError:
                    print 'invalid command, try help'
                command = ''
        except EOFError, k:
            pass
        except KeyboardInterrupt, k:
            pass
        reactor.main_quit()


def main():
    print "ControlPoint example with Event Notification\n"
    cmdline = CommandLineControlPoint()
    cmdline.run()

if __name__ == "__main__":
    main()
