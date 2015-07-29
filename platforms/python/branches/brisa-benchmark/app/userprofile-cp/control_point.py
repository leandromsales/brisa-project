import sys
from control_point_up import ControlPointUP
from brisa.threading import thread_manager


class CommandLineControlPointUP(ControlPointUP):

    def __init__(self):
        ControlPointUP.__init__(self)
        self.current_server = None
        self.current_server_name = None
        self.running = True
        self._initial_subscribes()
        self.devices_found = []
        self.commands = {'start': self._search,
                         'stop': self._stop,
                         'list': self._cmd_list_devices,
                         'exit': self._exit,
                         'help': self._help,
                         'authenticate': self._cmd_authenticate,
                         'renew_session': self.renewsession,
                         'logout': self.logout,
                         'get_info': self._cmd_get_info}

    def _initial_subscribes(self):
        self.subscribe('new_device_event', self.on_new_device)
        self.subscribe('remove_device_event', self.on_remove_device)

    def on_new_device(self, dev):
        self.devices_found.append(dev)

    def on_remove_device(self, udn):
        for dev in self.devices:
            if dev.udn == udn:
                self.devices_found.remove(dev)
                break

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

    def _cmd_set_server(self, id):
        self.current_server = self.devices_found[id]
        self.current_server_name = self.devices_found[id]

    def _search(self):
        self.start_search(600, 'upnp:rootdevice')
        print 'search started'

    def _stop(self):
        self.stop_search
        print 'search stopped'

    def _cmd_authenticate(self, login, password):
        authentication = self.Auth(login, password)
        print 'Authentication has succeeded : urn = %s' % authentication

    def _help(self):
        print 'commands: start, stop, list, ' \
              'logout, authenticate, renew_session, set_server, get_info,'\
              'exit, help'

    def _cmd_get_info(self, _uuid):
        info = self.GetUserInfo(_uuid)

    def _exit(self):
        self.running = False

    def run(self):
        try:
            while self.running:
                command = str(raw_input('>>> '))
                try:
                    self.commands[command]()
                except KeyError:
                    if command.startswith('browse'):
                        self._cmd_browse(command.split(' ')[1])
                    elif command.startswith('authenticate'):
                        self._cmd_authenticate(command.split(' ')[1],
                                               command.split(' ')[2])
                    elif command.startswith('set_server'):
                        self._cmd_set_server(int(command.split(' ')[1]))
                    elif command.startswith('get_info'):
                        self._cmd_get_info(command.split(' ')[1])
                    else:
                        print 'Invalid command, try help'
                command = ''
        except KeyboardInterrupt, k:
            print 'quiting'
        thread_manager().main_loop_quit()
        return


def main():
    print "BRisa ControlPointAV example\n"
    cmdline = CommandLineControlPointUP()
    cmdline.run()

if __name__ == "__main__":
    main()
