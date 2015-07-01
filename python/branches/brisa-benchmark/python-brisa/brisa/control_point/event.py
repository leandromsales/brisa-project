# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" UPnP event support.
"""

from xml.etree import ElementTree

from brisa import log
from brisa.threading import thread_manager
from brisa.services.web_server import WebServer
from brisa.services.web_server import CustomResource
from brisa.utils.network import get_active_ifaces
from brisa.utils.network import get_ip_address


class EventListener(CustomResource):
    """ EventListener resource available at the control point web server,
    listening for events.
    """
    isLeaf = True

    def __init__(self, observer):
        """ Constructor for the EventListener class

        @param observer: observer class with a _on_event() method.
        """
        CustomResource.__init__(self)
        self.observer = observer

    def cleanup(self):
        """ Removes reference to observer to make GC easier """
        self.observer = None

    def render(self, uri, request, response):
        """ Event renderer method. As events come only on NOTIFY messages, this
        method ignores any other type of message (GET, POST, ...).

        @param uri: URI of the request
        @param request: request object (Cherrypy)
        @param response: response object (Cherrypy)

        @type uri: string

        @note: see Cherrypy documentation for further info about request and
        response attributes and methods.
        """
        log.debug('Received render (%s)' % str((uri, request, response)))

        if request.method == 'NOTIFY':
            log.debug('Ok, got notify!')
            self.render_NOTIFY(request, response)
        else:
            log.debug('Did not get notify, got %s' % request.method)

        log.debug('Returning from render')

    def render_NOTIFY(self, request, response):
        """ Renders the notify message for an event.

        @param request: request object (Cherrypy)
        @param response: response object (Cherrypy)

        @note: see Cherrypy documentation for further info about request and
        response attributes and methods.
        """
        data_length = int(request.headers.get('Content-Length') or 0)
        data = request.rfile.read(data_length)
        # extraneous characters after the end of XML will choke ElementTree
        data = data[data.find("<"):data.rfind(">")+1]

        thread_manager.run_async_function(self.forward_notification,
                                           (request.headers, data),
                                           0.0001)
        return ""

    def forward_notification(self, received_headers, data):
        """ Forwards notifications to the observer registered.

        @param received_headers: headers received on the event notify
        @param data: XML data for the event

        @type received_headers: dictionary
        @type data: string
        """
        headers = {}
        changed_vars = {}
        for k, v in received_headers.items():
            headers[k.lower()] = v.lower()

        try:
            tree = ElementTree.XML(data)
        except:
            log.debug('Event XML invalid: %s', data)
            tree = None

        if tree:
            for prop1 in tree.findall('{%s}property' %
                                      'urn:schemas-upnp-org:event-1-0'):
                # prop1 = <e:property> <Ble> cont </Ble> </e:property>
                for prop2 in prop1:
                    # <Ble>cont</Ble>
                    changed_vars[prop2.tag] = prop2.text

        log.debug('Event changed vars: %s', changed_vars)

        if self.observer and 'sid' in headers:
            self.observer._on_event(headers['sid'], changed_vars)

            for dev in self.observer._known_devices:
                service = self._find_service(dev, headers['sid'])
                if service != None:
                    service._on_event(changed_vars)
                    return

                for child_dev in dev.devices:
                    service = self._find_service(child_dev, headers['sid'])
                    if service != None:
                        service._on_event(changed_vars)
                        return

    def _find_service(self, device, subscription_id):
        """ Method to find services on the given device.

        @param device: instance of a device
        @param subscription_id: the id to compare with the service

        @type device: RootDevice or Device
        @type subscription_id: str

        @return: if found, the service
        @rtype: Service or None
        """
        for k, service in device.services.items():
            if service.event_sid == subscription_id:
                return service
        return None


class EventListenerServer(object):
    """ EventListener server. Wraps BRisa's web server and listens for events.
    """

    def __init__(self, observer):
        """ Constructor for the EventListenerServer class.

        @param observer: observer that implements the _on_event() method
        """
        self.srv = WebServer()
        self.event_listener = EventListener(observer)
        self.srv.add_resource('', self.event_listener)
        self.srv.start()

    def host(self):
        """ Returns a tuple in the form (host, port) where the server is being
        hosted at.

        @return: the host and port of the server host
        @rtype: tuple
        """
        return (self.srv.get_host(), self.srv.get_port())

    def stop(self):
        """ Stops the EventListenerServer. For restarting after stopping with
        this method use EventListenerServer.srv.start().
        """
        if self.srv:
            self.srv.remove_resource('')
            self.event_listener.cleanup()
            self.event_listener = None
            self.srv.stop()
