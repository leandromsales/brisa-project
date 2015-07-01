# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Provides a base control point class that can be extended to any specialized
control point.
"""

import sys

from brisa.control_point.event import EventListenerServer
from brisa.upnp.message_handle.device_description_object import \
    DeviceDescriptionObject
from brisa.upnp.msearch import MSearch
from brisa.upnp.ssdp import SSDPServer
from brisa.threading.thread_manager import _ThreadManager, ThreadManager
import time


def get_device_object(location):
    """ Returns a description of an device given its location.

    @param location: device location, usually an URL.
    @type location: string

    @return: device description object for the location
    @rtype: DeviceDescriptionObject
    """
    return DeviceDescriptionObject(location).to_object()


def get_device_object_async(location, callback, cargo):
    """ Returns a description of an device given its location to the specified
    callback (asynchronously).

    @param location: device location, usually an URL
    @param callback: function to receive the device description object when
    ready. Must have the prototype like callback(cargo, device_object)
    @param cargo: cargo to be forwarded to the callback with the device object
    created

    @type location: string
    @type callback: callable
    """
    DeviceDescriptionObject(location).to_object_async(callback, cargo)


def get_service_control_url(service_dict, service_type):
    """ Returns the control URL for the service type in the given service
    dictionary.

    @param service_dict: dictionary of services
    @param service_type: service type to match

    @type service_dict: dictionary
    @type service_type: string
    """
    for k, service in service_dict.items():
        service = service_dict[k]
        if service.service_type.startswith(service_type):
            return service.control_url


class ControlPoint(object):
    """This class implements the basic UPnP Control Point functionalities.
    """
    encoding = 'UTF-8'

    def __init__(self, receive_notify=True):
        """ControlPoint class constructor.

        @param receive_notify: if False, ignores notify messages from devices.
        Default value is True and it can be set during runtime

        @type receive_notify: boolean
        """

        self._ssdp_server = SSDPServer("BRisa Control Point", None,
                                      receive_notify=receive_notify)
        self._ssdp_server.subscribe("new_device_event", self._new_device_event)
        self._ssdp_server.subscribe("removed_device_event",
                                   self._removed_device_event)
        self._ssdp_server.start()

        self._msearch = None
        self._callbacks = {}

        # EventListenerServer takes care of handling the web server
        self._event_listener = EventListenerServer(self)

        # Data for event subscribing
        # event_host is necessary for event subscribing, so it can tell the
        # IP:port of the event listener in subscription, without having to
        # refer to ControlPoint inside service.py (we presume that subscription
        # caller "knows" ControlPoint and Services, so it is in better position
        # of forwarding this info)
        self.event_host = self._event_listener.host()

    def stop(self):
        """ Stops and invalidates control point.
        """
        self.stop_search()
        self._ssdp_server.stop()
        self._event_listener.stop()
        # yield timeslice
        time.sleep(0.1)
        # HACK: kill outstanding actions
        ThreadManager().interrupt_all()
        self._event_listener = None
        self._ssdp_server = None
        self._msearch = None
        self._callbacks = {}
        self.event_host = None

    def _get_recv_notify(self):
        """ GET function for the receive_notify property. Use
        self.receive_notify instead.
        """
        return self._ssdp_server.receive_notify

    def _set_recv_notify(self, n):
        """ SET function for the receive_notify property. Use
        self.receive_notify instead.
        """
        self._ssdp_server.receive_notify = n

    receive_notify = property(_get_recv_notify,
                              _set_recv_notify,
                              doc='If False, the control point ignores NOTIFY\
                              messages from devices.')

    def subscribe(self, name, callback):
        """ Subscribes the callback for an event.

        @param name: event name
        @param callback: callback which will listen on the event

        @type name: string
        @type callback: callable
        """
        self._callbacks.setdefault(name, []).append(callback)

    def unsubscribe(self, name, callback):
        """ Unsubscribes the callback for an event.

        @param name: event name
        @param callback: callback which listens for the event

        @type name: string
        @type callback: callable
        """
        callbacks = self._callbacks.get(name, [])
        if callback in callbacks:
            callbacks.remove(callback)

    def callback(self, name, *args):
        """ Callback for any event. Forwards the event to the subscribed
        callbacks.

        @param name: event name
        @param args: arguments for the callbacks

        @type name: string
        @type args: tuple
        """
        for callback in self._callbacks.get(name, []):
            callback(*args)

    # Start of Device Search Methods

    def start_search(self, interval, type_search="ssdp:all", reset=False):
        """ Sends a multicast M-SEARCH message to discover UPnP devices.

        @param interval: interval to wait between sending search messages
        @param type_search: UPnP type search
        @param reset: clears the device list from any previous search

        @type interval: float
        @type type_search: string
        @type reset: boolean
        """

        if reset:
            self._ssdp_server.clear_device_list()

        if not self._msearch:
            self._msearch = MSearch(self._ssdp_server, start=False)
        self._msearch.type_search = type_search
        self._msearch.start(interval)

    def stop_search(self):
        """ Stops the device search.
        """
        if self._msearch is not None:
            self._msearch.stop()
            self._msearch = None

    def force_discovery(self, type_search="ssdp:all"):
        """ Forces a multicast MSearch bypassing the time interval. This method
        force msearch to send discovery message, bypassing the initial time
        interval passed to start_search function. Note this method doesn't
        cause any effect if the start_search call was never called.

        @param type_search: UPnP type search

        @type type_search: string
        """
        if self._msearch:
            self._msearch.type_search = type_search
            self._msearch.double_discover()

    def is_msearch_running(self):
        """ Returns whether MSEARCH is running or not.
        """
        if self._msearch:
            return self._msearch.is_running
        return False

    # End of Device Search Methods

    # Start of SSDP Callbacks implementation

    def _new_device_event(self, st, device_info):
        """ Receives a new device event.
        """
        # Callback assigned for new device event, processes asynchronously
        if 'LOCATION' not in device_info:
            return
        get_device_object_async(device_info['LOCATION'],
                                self._new_device_event_impl, device_info)

    def _new_device_event_impl(self, device_info, device_object):
        """ Real implementation of the new device event handler.
        """
        if not device_object and self._ssdp_server:
            # Device creation failed, tell SSDPSearch to forget it
            self._ssdp_server.discovered_device_failed(device_info)
            return

        # Real implementation of the new device event
        root_device = device_object.device

        # This is a general on new device event callback.
        # The device listener MUST implement this method
        # and check in device_object.device.device_type
        # the device type.
        self.callback("new_device_event", device_object.device)

    def _removed_device_event(self, device_info):
        """ Receives a removed device event.
        """
        udn = device_info['USN'].split('::')[0]
        self.callback("removed_device_event", udn)

    def _on_event(self, sid, changed_vars):
        """ Receives an event.
        """
        self.callback("device_event", sid, changed_vars)

    # End of SSPP Callbacks implementation
