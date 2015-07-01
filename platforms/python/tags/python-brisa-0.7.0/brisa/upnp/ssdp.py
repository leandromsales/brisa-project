# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
#
# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006 John-Mark Gurney <gurney_j@resnet.uroegon.edu>
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" SSDP Server implementation which listens for devices messages.
"""

import random
from brisa import log
from brisa.utils.network_listeners import UDPListener
from brisa.utils.network_senders import UDPTransport
from brisa.utils.python.looping_call import LoopingCall
from brisa.upnp.upnp_defaults import UPnPDefaults

SSDP_ADDR = UPnPDefaults.SSDP_ADDR
SSDP_PORT = UPnPDefaults.SSDP_PORT


class SSDPServer(object):
    """ Implementation of a SSDP server.

    The notify_received and search_received methods are called when the
    appropriate type of datagram is received by the server.
    """
    elements = {}
    known_device = {}
    own = {}
    _callbacks = {}

    def __init__(self, server_name, xml_description_filename, max_age=1800,
                receive_notify=True):
        """ Constructor for the SSDPServer class.

        @param server_name: server name
        @param xml_description_filename: XML description filename
        @param max_age: max age parameter, default 1800.
        @param receive_notify: if False, ignores notify messages

        @type server_name: string
        @type xml_description_filename:
        @type max_age: integer
        @type receive_notify: boolean
        """
        self.udp_listener = UDPListener(SSDP_ADDR, SSDP_PORT,
                                       data_callback=self.datagram_received)
        self.udp_transport = UDPTransport()
        self.server_name = server_name
        self.xml_description_filename = xml_description_filename
        self.max_age = max_age
        refresh_time = 0.8 * self.max_age
        self.renew_loop = LoopingCall(self._renew_notifications)
        self.renew_loop.start(refresh_time, now=True)
        self.started = False
        self.receive_notify = receive_notify

    def datagram_received(self, data, (host, port)):
        """ Handles a received multicast datagram.

        @param data: raw data
        @param host: datagram source host
        @param port: datagram source port

        @type data: string
        @type host: string
        @type port: integer
        """
        try:
            header, payload = data.split('\r\n\r\n')
        except ValueError, err:
            log.error('Error while receiving datagram packet')

        lines = header.split('\r\n')
        cmd = lines[0].split(' ')
        lines = map(lambda x: x.replace(': ', ':', 1), lines[1:])
        lines = filter(lambda x: len(x) > 0, lines)

        headers = [x.split(':', 1) for x in lines]
        headers = dict(map(lambda x: (x[0].lower(), x[1]), headers))

        if cmd[0] == 'M-SEARCH' and cmd[1] == '*':
            # SSDP discovery
            log.debug('SSDPServer: command is M-Search')
            self._discovery_request(headers, (host, port))
        elif cmd[0] == 'NOTIFY' and cmd[1] == '*':
            if not self.receive_notify:
                # Ignore notify
                return
            # SSDP presence
            self._notify_received(headers, (host, port))
        else:
            log.info('Unknown SSDP command %s', cmd)

    def _discovery_request(self, headers, (host, port)):
        """ Processes discovery requests and responds accordingly.

        @param headers: discovery headers
        @param host: discovery source host
        @param port: discovery source port

        @type headers: dictionary
        @type host: string
        @type port integer
        """
        rightKey = 0

        # Do we know about this service?
        if headers['st'] == 'ssdp:all':
            for i in self.own.keys():
                hcopy = dict(headers.iteritems())
                hcopy['st'] = self.own[i]['ST']
                self._discovery_request(hcopy, (host, port))
            return

        for key in self.own.keys():
            if self.own[key]['ST'].split()[0] == (headers['st']).split()[0]:
                rightKey = key
                break
        else:
            log.info('Discovery request ST %s not found', headers['st'])
            return

        if rightKey == 0:
            log.error('Unknown Error in Discovery request for %s',
                          headers['st'])
            return

        # Generate a response
        response = []
        response.append('HTTP/1.1 200 OK')
        append = response.append

        [append('%s: %s' % (k, v)) for k, v in self.own[rightKey].items()]
        response.extend(('', ''))
        delay = random.randint(0, int(headers['mx']))
        # Avoid using a timer with delay 0 :)
        if delay:
            self.udp_transport.send_delayed(delay, '\r\n'.join(response),
                                            (host, port))
        else:
            self.udp_transport.send_data('\r\n'.join(response), (host, port))
        log.debug('SSDPServer: discovery_request response sent to (%s, %d)' %\
                  (host, port))

    def _notify_received(self, headers, (host, port)):
        """ Processes a presence announcement.

        @param headers: notify headers
        @param host: notify source host
        @param port: notify source port

        @type headers: dictionary
        @type host: string
        @type port: integer
        """
        if headers['nts'] == 'ssdp:alive':
            if not self.is_known_device(headers['usn']):
                if 'cache-control' not in headers:
                    headers['cache-control'] = 'max-age=1800'
                self.register(headers['usn'],
                              headers['nt'],
                              headers['location'],
                              headers['server'],
                              headers['cache-control'])
        elif headers['nts'] == 'ssdp:byebye':
            if self.is_known_device(headers['usn']):
                self._unregister(headers['usn'])
        else:
            log.info('Unknown subtype %s for notification type %s',
                         headers['nts'], headers['nt'])

    def clear_device_list(self):
        """ Clears the device list.
        """
        self.known_device.clear()

    def register(self, usn, st, location, server, cache_control):
        """ Registers a service or device.

        @param usn: usn
        @param st: st
        @param location: location
        @param server: server
        @param cache_control: cache control

        @type usn: string
        @type location: string
        @type st: string
        @type server: string
        @type cache_control: string

        @note: these parameters are part of the UPnP Specification. Even though
        they're abstracted by the framework (devices and services messages
        already contain these parameters), if you want to understand it please
        refer to the UPnP specification. Links can be found at our developer
        documentation homepage.
        """

        self.known_device[usn] = {'USN': usn, 'LOCATION': location, 'ST': st,
                                      'EXT': '', 'SERVER': server,
                                      'CACHE-CONTROL': cache_control}

        if st == 'upnp:rootdevice':
            self._callback("new_device_event", st, self.known_device[usn])

    def _local_register(self, usn, st, location, server, cache_control):
        """ Registers locally a new service or device.
        """
        log.info('Registering locally %s (%s)', st, location)
        self.own[usn] = {'USN': usn, 'LOCATION': location, 'ST': st, 'EXT': '',
                         'SERVER': server, 'CACHE-CONTROL': cache_control}

        if st == 'upnp:rootdevice':
            self._callback("new_device_event", st, self.own[usn])

        self._do_notify(usn)

    def register_root_device(self, root_device):
        """ Registers the root device on the SSDP server.

        @param root_device: device to be registered
        @type root_device: RootDevice
        """
        if root_device.is_root_device():
            # Doesn't contain embedded devices
            device_id = root_device.udn
            device_type = root_device.device_type
            device_server = "BRisa Webserver UPnP/1.0 %s" % self.server_name
            device_location = "%s/%s" % (root_device.location,
                                         self.xml_description_filename)
            age = 'max-age=%d' % self.max_age
            self._local_register('%s::upnp:rootdevice' % device_id,
                               'upnp:rootdevice', device_location,
                               device_server, age)
            self._local_register(device_id, device_id, device_location,
                               device_server, age)
            self._local_register('%s%s%s' % (device_id, "::", device_type),
                                device_type, device_location,
                                device_server, age)

            local_register = self._local_register
            (local_register('%s%s%s' % (device_id, "::",
                                        service.service_type),
                                        service.service_type, device_location,
                                        device_server, age) for service in
                                                          root_device.services)
        else:
            # Contains embedded devices
            #TODO: implement-me
            pass

    def announce_device(self):
        """ Announces the device.
        """
        do_notify = self._do_notify
        (do_notify(usn) for usn in self.known_device)

    def _renew_notifications(self):
        """ Renew notifications (sends a notify
        """
        own_temp = self.own.copy()
        for usn in own_temp:
            log.debug('Renew notification for %s ', own_temp[usn]['USN'])
            self._do_notify(own_temp[usn]['USN'])

    def _unregister(self, usn):
        log.info("_unregistering %s", usn)

        try:
            self._callback("removed_device_event", self.known_device[usn])
            if usn in self.known_device:
                del self.known_device[usn]
        except:
            pass

    def is_known_device(self, usn):
        """ Returns if the device with the passed usn is already known.

        @param usn: device's usn
        @type usn: string

        @return: True if it is known
        @rtype: boolean
        """
        return (usn in self.known_device)

    def subscribe(self, name, callback):
        """ Subscribes a callback for an event.

        @param name: name of the event
        @param callback: callback

        @type name: string
        @type callback: callable
        """
        self._callbacks.setdefault(name, []).append(callback)

    def unsubscribe(self, name, callback):
        """ Unsubscribes a callback for an event.

        @param name: name of the event
        @param callback: callback

        @type name: string
        @type callback: callable
        """
        callbacks = self._callbacks.get(name, [])
        if callback in callbacks:
            callbacks.remove(callback)
        self._callbacks[name] = callbacks

    def _callback(self, name, *args):
        """ Performs callbacks for events.
        """
        for callback in self._callbacks.get(name, []):
            callback(*args)

    def _do_notify(self, st):
        """ Do a notification for the st specified.

        @param st: st
        @type st: string
        """
        log.info('Sending alive notification for %s', st)
        resp = ['NOTIFY * HTTP/1.1', 'HOST: %s:%d' % (SSDP_ADDR, SSDP_PORT),
                'NTS: ssdp:alive', ]
        stcpy = dict(self.own[st].iteritems())
        stcpy['NT'] = stcpy['ST']
        del stcpy['EXT']
        del stcpy['ST']
        resp.extend(map(lambda x: ': '.join(x), stcpy.iteritems()))
        resp.extend(('', ''))
        self.udp_transport.send_data('\r\n'.join(resp), (SSDP_ADDR, SSDP_PORT))
        self.udp_transport.send_data('\r\n'.join(resp), (SSDP_ADDR, SSDP_PORT))

    def _do_byebye(self, st):
        """ Do byebye notification for the st specified.

        @param st: st
        @type st: string
        """
        log.info('Sending byebye notification for %s', st)
        resp = ['NOTIFY * HTTP/1.1', 'HOST: %s:%d' % (SSDP_ADDR, SSDP_PORT),
                'NTS: ssdp:byebye', ]
        stcpy = dict(self.own[st].iteritems())
        stcpy['NT'] = stcpy['ST']
        del stcpy['ST']
        del stcpy['EXT']
        resp.extend(map(lambda x: ': '.join(x), stcpy.iteritems()))
        resp.extend(('', ''))
        log.debug(resp)
        self.udp_transport.send_data('\r\n'.join(resp), (SSDP_ADDR, SSDP_PORT))
        self.udp_transport.send_data('\r\n'.join(resp), (SSDP_ADDR, SSDP_PORT))

    def start(self):
        """ Starts listening.
        """
        self.udp_listener.start()
        self.started = True

    def stop(self):
        """ Stops the SSDPSerer, making sure we sent the byebye notifications.
        """
        own_temp = self.own.copy()
        for st in own_temp:
            self._do_byebye(st)
        log.info('Waiting for other threads to finish...')

        self.renew_loop.stop()
        self.udp_listener.stop()
