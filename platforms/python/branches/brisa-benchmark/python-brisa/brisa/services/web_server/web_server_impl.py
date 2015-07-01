# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Implements the web server singleton.
"""

import random
import cherrypy
from cherrypy.process.servers import check_port

from brisa import log
from brisa.utils.network import parse_url
from brisa.threading import thread_manager
from brisa.services.web_server.static import StaticFile
from brisa.services.web_server.resource import CustomResource
from brisa.utils.network import get_active_ifaces
from brisa.utils.network import get_ip_address

_singleton = None


class _BrisaWebServer(object):

    def __init__(self):
        """ This class may not be constructed manually. Instead, use
        BrisaWebServer() factory method for retrieving a reference to
        the singleton instance. """
        self._root_dir = CustomResource()
        self._root_dir.is_root = True
        self._root_dir.exposed = True
        self._daemon = True
        self._listen_host = ''
        self._listen_port = 0
        thread_manager.register_stop_function(self.stop)
        log.debug('Webserver created and registered to be stopped when the'\
                  ' program leaves')

    def get_host(self):
        """ Returns the host which is being listened. """
        return self._listen_host

    def get_port(self):
        """ Returns the port which is being listened. """
        return self._listen_port

    def _get_listen_url(self):
        """ Returns the listen url. """
        return self._listen_url

    def _set_listen_url(self, url):
        """ Method which sets the listen url.

        @param url: the url to be listened

        @type url: string
        """
        u = parse_url(url)
        self._listen_port = u.port
        self._listen_host = u.hostname
        self._listen_url = url

    def _get_root_dir(self):
        """ Returns the root directory. """
        return self._root_dir

    def _set_root_dir(self, r):
        """ Method that sets the root directory.

        @param r: directory to be set.

        @type r: string
        """
        # If you're setting this then you really know what you're doing.
        self._root_dir = r

    listen_url = property(_get_listen_url, _set_listen_url)
    root_dir = property(_get_root_dir, _set_root_dir)

    def add_static_file(self, name, static_file):
        """ Adds a static file to server's tree

        @param name: name of the static file
        @param static_file: the static file instance

        @type name: string
        @type static_file: StaticFile
        """
        if static_file:
            self._root_dir.add_static_file(name, static_file)

    def add_resource(self, name, resource):
        """ Adds a resource to server's tree

        @param name: name of the resource to be added
        @param resource: resource to be added

        @type name: string
        @type resource: class of the resource to be added

        """
        if resource:
            self._root_dir.add_resource(name, resource)

    def remove_resource(self, name):
        """ Removes a resource from the tree during execution

        @param name: resource to be removed

        @type name: string
        """
        if hasattr(self._root_dir, name):
            delattr(self._root_dir, name)

    def start(self):
        """ Run method called when start() is called. Invoking this method
        directly may cause unexpected results (blocking). Instead, invoke
        start(). """
        self.prepare_to_start()
        self._mount_tree()
        self._configure()
        cherrypy.engine.start()

    def stop(self):
        """ Prepares to stop ThreadObject by stopping the server """
        cherrypy.engine.stop()
        cherrypy.server.bus.exit()
        # Once stopped, this object cannot be restarted, so force renewal
        global _singleton
        _singleton = None
        self._root_dir = None
        self._app = None

    def prepare_to_start(self, daemon=True):
        """ Method that prepares the webserver to start. """
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
