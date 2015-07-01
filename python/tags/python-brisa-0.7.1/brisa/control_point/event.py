# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" UPnP event support.
"""

from xml.etree import ElementTree

from brisa import log
from brisa.threading import ThreadManager
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

        @param observer: observer class with a _on_event() method
        """
        CustomResource.__init__(self)
        self.observer = observer

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
        data = request.body
        ThreadManager().run_async_function(self.forward_notification,
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
            tree = None

        if tree:
            for prop1 in tree.findall('.//{%s}property' %
                                      'urn:schemas-upnp-org:event-1-0'):
                # prop1 = <e:property> <Ble> cont </Ble> </e:property>
                for prop2 in prop1:
                    # <Ble>cont</Ble>
                    changed_vars[prop2.tag] = prop2.text

        if self.observer and 'sid' in headers:
            self.observer._on_event(headers['sid'], changed_vars)


class EventListenerServer(object):
    """ EventListener server. Wraps BRisa's web server and listens for events.
    """

    def __init__(self, observer):
        """ Constructor for the EventListenerServer class.

        @param observer: observer that implements the _on_event() method
        """
        self.srv = WebServer()
        self.srv.add_resource('', EventListener(observer))
        self.srv.start()

    def host(self):
        """ Returns a tuple in the form (host, port) where the server is being
        hosted at.
        """
        return (self.srv.get_host(), self.srv.get_port())

    def stop(self):
        """ Stops the EventListenerServer. For restarting after stopping with
        this method use EventListenerServer.srv.start().
        """
        if self.srv:
            self.srv.stop()
