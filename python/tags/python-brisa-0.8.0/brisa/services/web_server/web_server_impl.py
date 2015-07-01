# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Implements the web server singleton.
"""

import random
import cherrypy
from cherrypy.process.servers import check_port

from thread import start_new_thread
from threading import Thread
from time import sleep

from brisa import log
from brisa.utils.network import parse_url
from brisa.threading import ThreadObject, ThreadManager
from brisa.services.web_server.static import StaticFile
from brisa.services.web_server.resource import CustomResource
from brisa.utils.network import get_active_ifaces
from brisa.utils.network import get_ip_address

_singleton = None


class _BrisaWebServer(ThreadObject):

    def __init__(self):
        """ This class may not be constructed manually. Instead, use
        BrisaWebServer() factory method for retrieving a reference to
        the singleton instance. """
        ThreadObject.__init__(self)
        self.setDaemon(True)
        self._root_dir = CustomResource()
        self._root_dir.is_root = True
        self._root_dir.exposed = True
        self._daemon = True
        self._listen_host = ''
        self._listen_port = 0

    # Presence of __del__ method messes with garbage collection!
    #
    # def __del__(self):
    #     """ Called in the end of the execution. ThreadManager calls all stop
    #     methods from ThreadObjects registered and removes them from its list,
    #     resulting in this call. """
    #     pass

    def get_host(self):
        return self._listen_host

    def get_port(self):
        return self._listen_port

    def _get_listen_url(self):
        return self._listen_url

    def _set_listen_url(self, url):
        u = parse_url(url)
        self._listen_port = u.port
        self._listen_host = u.hostname
        self._listen_url = url

    def _get_root_dir(self):
        return self._root_dir

    def _set_root_dir(self, r):
        # If you're setting this then you really know what you're doing.
        self._root_dir = r

    listen_url = property(_get_listen_url, _set_listen_url)
    root_dir = property(_get_root_dir, _set_root_dir)

    def add_static_file(self, name, static_file):
        """ Adds a static file to server's tree """
        if static_file:
            self._root_dir.add_static_file(name, static_file)

    def add_resource(self, name, resource):
        """ Adds a resource to server's tree """
        if resource:
            self._root_dir.add_resource(name, resource)

    def remove_resource(self, name):
        """ Removes a resource from the tree during execution """
        if hasattr(self._root_dir, name):
            delattr(self._root_dir, name)

    def run(self):
        """ Run method called when start() is called. Invoking this method
        directly may cause unexpected results (blocking). Instead, invoke
        start(). """
        self._mount_tree()
        self._configure()
        # cherrypy.quickstart is deprecated
        # start_new_thread(cherrypy.quickstart, (self._app, ))
        cherrypy.engine.start()

    def prepare_to_stop(self):
        """ Prepares to stop ThreadObject by stopping the server """
        cherrypy.engine.stop()
        cherrypy.server.bus.exit()
        # Once stopped, this object cannot be restarted, so force renewal
        global _singleton
        _singleton = None
        self._root_dir = None
        self._app = None
        return True

    def prepare_to_start(self, daemon=True):
        # Default listen url: localhost:port where port is on the dynamic
        # range (non registered ports)
        ifaces = get_active_ifaces()
        if ifaces:
            self._listen_host = get_ip_address(ifaces[0])
        else:
            self._listen_host = 'localhost'

        while not self._listen_port:
            self._listen_port = random.randint(49152, 65535)
            try:
                check_port(self._listen_host, self._listen_port)
            except IOError:
                self._listen_port = 0

        self._listen_url = 'http://%s:%d' % (self._listen_host,
                                             self._listen_port)
        self.daemon = True

    def _mount_tree(self):
        """ Mounts the tree and sets the Application object """
        self._app = cherrypy.tree.mount(self._root_dir)

    def _configure(self):
        """ TODO: Add more configuration entries here as needed.

        Embedded environment is:
            - 'engine.autoreload_on': False
            - 'checker.on': False
            - 'tools.log_headers.on': False
            - 'request.show_tracebacks': False
            - 'log.screen': False
            - 'engine.SIGHUP': None
            - 'engine.SIGTERM': None
        """
        cherrypy.config.update({'environment': 'embedded'})
        cherrypy.server.socket_host = self._listen_host
        cherrypy.server.socket_port = self._listen_port
        # HACK: CherryPy keeps this bind_addr across sections,
        # forcing update
        cherrypy.server.bind_addr = (self._listen_host, self._listen_port)
        cherrypy.server.socket_file = ''
        if cherrypy.server.httpserver:
            cherrypy.server.httpserver.bind_addr = (self._listen_host,
                                                    self._listen_port)


def WebServer():
    """ Returns the instance of BRisa's Web Server.
    """
    # Lazy initialization
    global _singleton
    if not _singleton:
        _singleton = _BrisaWebServer()
    return _singleton
