# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Provides a simple API and observer model for listening over TCP and UDP.
"""

import socket
from time import time
from struct import pack

from brisa import log
from brisa.threading import ThreadObject, ThreadManager


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


class INetworkObserver:
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


class NetworkListener(ThreadObject):
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
        ThreadObject.__init__(self)
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
                ThreadManager().run_async_function(listener.data_received,
                                                   (data, addr))
            else:
                ThreadManager().run_async_function(listener.data_received,
                                                   (data, ()))

        if self.data_callback:
            if addr:
                ThreadManager().run_async_function(self.data_callback,
                                                   (data, addr))
            else:
                ThreadManager().run_async_function(self.data_callback,
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

    def prepare_to_stop(self):
        """ Overwrites ThreadObject's prepare_to_stop method for stopping this
        network listener.
        """
        if self.listening:
            self.listening = False
            if self.close():
                return True
            else:
                # Could not close because it is busy. ThreadManager will handle
                # this.
                return False
        # Returns True either way. If listening, tries to stop.
        # If not listening, its already stopped.
        return True

    def run(self):
        """ Subclasses must implement this method for performing the listening.
        """
        raise Exception("You must implement the run() method.")

    def close(self):
        """ Subclasses must implement this method for closing the connection.
        """
        raise Exception("You must implement the close() method for closing the\
        connection. For example, for a socket you should call socket.close().")


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
        self.busy = False

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

    def run(self):
        """ Implements the TCPListener listening actions.
        """
        try:
            self.socket.bind((self.interface, self.port))
            self.listening = True
        except:
            raise CannotListenError(self.interface, self.port,
                                    reason="Couldn't bind address.")

        log.debug('TCPListener started')

        while self.listening:
            try:
                self.busy = True
                (data, addr) = self.socket.recvfrom(self.BUFFER_SIZE)
                self.busy = False
                self.forward_data(data, addr)
            except KeyboardInterrupt:
                log.debug('TCPListener shutdown')
                self.stop()
            except Exception, e:
                log.debug('TCPListener internal error, please report \
                         this error')

    def close(self):
        """ Closes the TCP connection.

        @return: True if stopped successfully
        @rtype boolean
        """
        if not self.busy:
            log.debug('Closing TCPListener socket')
            self.socket.close()
            return True
        return False


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
        self.busy = False

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

    def run(self):
        """ Implements the UDPListener listening actions.
        """
        self.listening = True

        while self.listening:
            try:
                self.busy = True
                (data, addr) = self.socket.recvfrom(self.BUFFER_SIZE)
                self.busy = False
                self.forward_data(data, addr)
            except KeyboardInterrupt:
                log.debug('UDPListener: shutdown')
                self.stop()
            except Exception, e:
                # Exception that always happen is timeout for recvfrom.
                # This is not a problem and the loop continues.
                pass

    def close(self):
        """ Closes the UDP socket.

        @return: True if stopped successfully
        @rtype boolean
        """
        if not self.busy:
            log.debug('UDPListener: closing socket')
            self.socket.close()
            return True
        return False
