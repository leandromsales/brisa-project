# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Provides a simple API and observer model for listening over TCP and UDP.
"""

import socket
from struct import pack

from brisa import log
from brisa.threading import thread_manager, FD_EVENT_TYPE_READ


class CannotListenError(Exception):
    """ Exception denoting an error when binding interfaces for listening.
    """

    def __init__(self, interface, port, addr='', reason=''):
        """ Constructor for the CannotListenError class

        @param interface: interface where the error occured
        @param port: port at the error ocurred when binding
        @param addr: address at the error ocurred when binding
        @param reason: reason why the error happened

        @type interface: string
        @type port: integer
        @type addr: string
        @type reason: string
        """
        if addr:
            self.message = "Couldn't listen on %s:%s: %d. " % (interface,
                                                             addr, port)
        else:
            self.message = "Couldn't listen on %s: %d. " % (interface,
                                                               port)
        if reason:
            self.message += reason


class SubscriptionError(Exception):
    """ Exception denoting an observer subscription error. Gets raised when the
    observer does not implement the INetworkObserver interface.
    """
    message = "Couldn't subscribe listener because it does not implement \
    data_received. Implement brisa.utils.network_listeners.INetworkObserver."


class INetworkObserver(object):
    """ Interface for network observers. Prototypes the data_received method.
    """

    def data_received(self, data, addr=None):
        """ Receives data when subscribed to some network listener.

        @param data: raw data
        @param addr: can receive a 2-tuple (host, port)

        @type data: string
        @type addr: None or tuple
        """
        raise Exception("Classes implementing INetworkObserver must implement \
                        data_received() method")


class NetworkListener(object):
    """ Network listener abstract class. Forwards data to multiple subscribed
    observers and can have a single callback to get data forwarded to.

    Methods that MUST be implemented by an inheriting class:
        - run()   : main loop that receives data. In order to forward data to
    observers and the data callback run() method MUST call
    self.forward_data(data, addr). Note that addr is optional.
        - close() : closes the connection
    """

    def __init__(self, observers=[], data_callback=None):
        """ Constructor for the NetworkListener class

        @param observers: initial subscribers for data forwarding
        @param data_callback: callback that gets data forwarded to

        @type observers: list
        @type data_callback: callable
        """
        self.listening = False
        self.data_callback = data_callback
        self.observers = observers

    def forward_data(self, data, addr=''):
        """ Forwards data to the subscribed observers and to the data callback.

        @param data: raw data to be forwarded
        @param addr: can be a 2-tuple (host, port)

        @type data: string
        @type addr: None or tuple
        """
        for listener in self.observers:
            if addr:
                thread_manager.run_async_function(listener.data_received,
                                                  (data, addr))
            else:
                thread_manager.run_async_function(listener.data_received,
                                                  (data, ()))

        if self.data_callback:
            if addr:
                thread_manager.run_async_function(self.data_callback,
                                                  (data, addr))
            else:
                thread_manager.run_async_function(self.data_callback,
                                                  (data, ()))

    def subscribe(self, observer):
        """ Subscribes an observer for data forwarding.

        @param observer: observer instance
        @type observer: INetworkObserver
        """
        if hasattr(observer, data_received) and observer not in self.observers:
            self.observers.append(observer)
        else:
            raise SubscriptionError()

    def is_listening(self):
        """ Returns whether this network listener is listening (already started
        with start()).
        """
        return self.listening

    def start(self):
        self.listening = True

    def stop(self):
        self.listening = False
        self.cleanup()

    def cleanup(self):
        """ Removes references to other classes, in order to make GC easier
        """
        self.data_callback = None
        self.observers = []


class TCPListener(NetworkListener):
    """ Listens TCP in a given port. Data received is forwarded to the
    observers subscribed.
    """
    BUFFER_SIZE = 1500

    def __init__(self, port, interface='', observers=[], data_callback=None,
                 shared_socket=None):
        """ Constructor for the TCPListener class

        @param port: port to listen on
        @param interface: interface to listen on
        @param observers: initial observers
        @param data_callback: callback to get data forwarded to
        @param shared_socket: socket to be reused by this network listener

        @type port: integer
        @type interface: string
        @type observers: list of INetworkObserver
        @type data_callback: callable
        @type shared_socket: socket.socket
        """
        NetworkListener.__init__(self, observers, data_callback)
        self.port = port
        self.interface = interface
        self.socket = None
        self._create_socket(shared_socket)

    def _create_socket(self, shared):
        """ Creates the socket if a shared socket has not been passed to the
        constructor.

        @param shared: socket to be reused
        @type shared: socket.socket
        """
        if shared:
            self.socket = shared
        else:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    def start(self):
        self.socket.bind((self.interface, self.port))
        thread_manager.add_fd(self.socket, self.receive_stream,
                               FD_EVENT_TYPE_READ)
        NetworkListener.start(self)

    def stop(self):
        NetworkListener.stop(self)
        self.socket.close()
        thread_manager.remove_fd(self.socket)

    def receive_stream(self, event_type):
        if self.is_listening():
            try:
                (data, addr) = self.socket.recvfrom(self.BUFFER_SIZE)
                self.forward_data(data, addr)
            except Exception, e:
                log.debug('REMOVEME exception at TCPListener %s' % e.message)


class UDPListener(NetworkListener):
    """ Listens UDP in a given address and port (and in the given interface, if
    provided).
    """
    BUFFER_SIZE = 1500

    def __init__(self, addr, port, interface='', observers=[],
                 data_callback=None, shared_socket=None):
        """ Constructor for the UDPListener class.

        @param addr: address to listen on
        @param port: port to listen on
        @param interface: interface to listen on
        @param observers: list of initial subscribers
        @param data_callback: callback to get data forwarded to
        @param shared_socket: socket to be reused by this network listener

        @type addr: string
        @type port: integer
        @type interface: string
        @type observers: list of INetworkObserver
        @type data_callback: callable
        @type shared_socket: socket.socket
        """
        NetworkListener.__init__(self, observers, data_callback)
        self.addr = addr
        self.port = port
        self.interface = interface
        self.socket = None
        self._create_socket(shared_socket)

    def start(self):
        thread_manager.add_fd(self.socket, self.receive_datagram,
                              type=FD_EVENT_TYPE_READ)
        NetworkListener.start(self)

    def stop(self):
        NetworkListener.stop(self)
        thread_manager.remove_fd(self.socket)
        self.socket.close()

    def _create_socket(self, shared):
        """ Creates the socket if a shared socket has not been passed to the
        constructor.

        @param shared: socket to be reused
        @type shared: socket.socket
        """
        if shared:
            self.socket = shared
        else:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM,
                                         socket.IPPROTO_UDP)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        try:
            self.socket.bind((self.interface, self.port))
            self.mreq = pack('4sl', socket.inet_aton(self.addr),
                             socket.INADDR_ANY)
            self.socket.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP,
                                    self.mreq)
        except:
            log.debug('UDPListener: CRITICAL ERROR - cannot create socket')
            raise CannotListenError(self.interface, self.port, self.addr,
                                    "Couldn't bind address")
        log.debug('UDPListener: socket created')

    def receive_datagram(self, event_type):
        """ Implements the UDPListener listening actions.
        """
        if not self.is_listening():
            return

        try:
            (data, addr) = self.socket.recvfrom(self.BUFFER_SIZE)
            self.forward_data(data, addr)
        except Exception, e:
            log.debug('Error when reading on UDP socket: %s', e)
