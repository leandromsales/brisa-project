# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
#
# Copyright (C) 2006 Fluendo, S.A. (www.fluendo.com).
# Copyright 2006, Frank Scholz <coherence@beebits.net>
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Contains the MSearch class which can search for devices.
"""

from brisa.utils.network import parse_http_response
from brisa.utils.network_senders import UDPTransport
from brisa.utils.network_listeners import UDPListener
from brisa.utils.looping_call import LoopingCall

from brisa import log

DEFAULT_SEARCH_TIME = 600.0


class MSearch(object):
    """ Represents a MSearch. Contains some control functions for starting and
    stopping the search. While running, search will be repeated in regular
    intervals specified at construction or passed to the start() method.
    """

    def __init__(self, ssdp, start=True, interval=DEFAULT_SEARCH_TIME,
                 ssdp_addr='239.255.255.250', ssdp_port=1900):
        """ Constructor for the MSearch class.

        @param ssdp: ssdp server instance that will receive new device events
        and subscriptions
        @param start: if True starts the search when constructed
        @param interval: interval between searchs
        @param ssdp_addr: ssdp address for listening (UDP)
        @param ssdp_port: ssdp port for listening (UDP)

        @type ssdp: SSDPServer
        @type start: boolean
        @type interval: float
        @type ssdp_addr: string
        @type ssdp_port integer
        """
        # Sharing sockets between UDPTransport and UDPListener is CRITICAL
        # here. It must be done so that the answer to M-Search comes back to
        # this listener.
        self.udp_transport = UDPTransport()
        self.listen_udp = UDPListener(ssdp_addr, ssdp_port,
                                     data_callback=self.datagram_received,
                                     shared_socket=self.udp_transport.socket)
        self.ssdp = ssdp
        self.ssdp_addr = ssdp_addr
        self.ssdp_port = ssdp_port
        self.type_search = "ssdp:all"
        self.loopcall = LoopingCall(self.double_discover)
        if start:
            self.start(interval)

    def is_running(self):
        """ Returns True if the search is running (it's being repeated in the
        interval given).

        @rtype: boolean
        """
        return self.loopcall.running

    def start(self, interval=DEFAULT_SEARCH_TIME):
        """ Starts the search.

        @param interval: interval between searchs. Default is 600.0 seconds
        @type interval: float
        """
        log.debug('MSearch started')
        self.listen_udp.start()
        if not self.loopcall.running:
            log.debug('MSearch loopcall started')
            self.loopcall.start(interval, now=True)

    def stop(self):
        """ Stops the search.
        """
        self.listen_udp.stop()
        if self.loopcall.running:
            self.loopcall.stop()

    def datagram_received(self, data, (host, port)):
        """ Callback for the UDPListener when messages arrive.

        @param data: raw data received
        @param host: host where data came from
        @param port: port where data came from

        @type data: string
        @type host: string
        @type port: integer
        """
        cmd, headers = parse_http_response(data)
        if cmd[0] == 'HTTP/1.1' and cmd[1] == '200':
            if self.ssdp != None:
                if not self.ssdp.is_known_device(headers['usn']):
                    self.ssdp.register(headers['usn'], headers['st'],
                                              headers['location'],
                                              headers['server'],
                                              headers['cache-control'])

    def double_discover(self):
        """ Sends a MSearch imediatelly. Each call to this method will yield a
        MSearch message, that is, it won't repeat automatically.
        """
        self.discover(self.type_search)
        self.discover(self.type_search)

    def discover(self, type = "ssdp:all"):
        """ Mounts and sends the discover message (MSearch).

        @param type: search type
        @type type: string
        """
        req = ['M-SEARCH * HTTP/1.1',
                'HOST: %s:%d' % (self.ssdp_addr, self.ssdp_port),
                'MAN: "ssdp:discover"',
                'MX: 5',
                'ST: ' + type, '', '']
        req = '\r\n'.join(req)
        self.udp_transport.send_data(req, (self.ssdp_addr, self.ssdp_port))
