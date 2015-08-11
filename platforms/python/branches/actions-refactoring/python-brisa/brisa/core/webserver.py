# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Webserver module wrapping cherrypy.
"""


__all__ = ('Resource', 'CustomResource', 'WebServer', 'StaticFile', 'Data')

import random
import cherrypy

from cherrypy.process.servers import check_port
from cherrypy.lib.static import serve_file

from brisa.core import log
from brisa.core.network import parse_url, get_active_ifaces, get_ip_address
from brisa import __enable_webserver_logging__, __enable_offline_mode__


log = log.getLogger('core.webserver')


if not __enable_offline_mode__:
    if not get_active_ifaces():
        raise RuntimeError('Network is down.')


class Resource(object):
    """ Class representing a resource. Resources can contain
    resources and static files. One should use CustomResource
    class instead of this.
    """

    def __init__(self, is_root=False):
        """ Constructor for the Resource class. """
        self.root = None
        self.is_root = is_root
        self.exposed = True

    def __nonzero__(self):
        """ This fixes a bug that happened with some classes that inherit from
        Resource also inherit from basic types like dict. If the dict is empty,
        then this class would evaluate to False, giving unexpected results on
        codes like "if object:".
        """
        return True

    def add_resource(self, name, resource, exposed=True):
        """ Adds a new resource

        @param name: name of the new resource
        @param resource: instance of the resource
        @param exposed: flag to decide either the resource is exposed or not

        @type name: string
        @type resource: resource class
        @type exposed: boolean
        """
        if name == '':
            self.root = resource
        else:
            setattr(self, name.replace('.', '_'), resource)

    def add_static_file(self, name, static_file, exposed=True):
        """ Adds a static file

        @param name: name of the static file to be added
        @param static_file: xml file to be added
        @param exposed: flag to decide either the file is exposed or not

        @type name: string
        @type static_file: StaticFile
        @type exposed: boolean
        """
        if name == '':
            # Default file to be loaded when the root is accessed
            self.root = static_file
        else:
            setattr(self, name.replace('.', '_'), static_file)

    def __call__(self, *args, **kwargs):
        """ When the resource is accessed through the server, this method is
        called and it should return the response body.

        (Example: brisa.upnp.upnp_publisher.py)
        """
        (uri, params) = (cherrypy.request.path_info, \
                        cherrypy.request.params)
        child = self.get_render(uri, params)

        # Default child not set
        if not child:
            # Not root
            if not self.root:
                # Then not found
                raise cherrypy.NotFound()
            else:
                # Is root
                child = self.root
        return child.render(uri, cherrypy.request, cherrypy.response)

    def get_render(self, uri, params=None):
        """ Should be overwritten when needed. This method may apply (or not)
        some logic with the uri and parameters passed and match it with a
        child. For example, if a resource represents a folder, then it will use
        the uri and params to find the file or sub folder.
        """
        return None


class CustomResource(Resource):
    """ Points to the latest stable Resource class. We recommend to use
    CustomResource always instead of Resource, for coding maintenance purposes.
    """
    pass


class StaticFile(object):
    """ Object that matches with a file and makes it available on the server.
    """
    exposed = True

    def __init__(self, path):
        """ Constructor for the StaticFile class.

        @param path: file path
        @type path string
        """
        self.path = path
        self.render()

    def render(self, uri=None, request=None, response=None):
        """ Method that renders a request and returns the response. In this
        case, serves the file.
        """
        return serve_file(self.path)

    def __call__(self, *args, **kwargs):
        return self.render(*args, **kwargs)

    def __str__(self):
        return "<StaticFile, Path: %s>" % self.path


class Data(CustomResource):
    """ Represents some data that can be available on the server.
    """

    def __init__(self, data, type):
        """ Constructor for the Data class.

        @param data: raw data
        @param type: data type

        @type data: string
        @type type: string
        """
        CustomResource.__init__(self)
        self.data = data
        self.type = type

    def render(self, uri, request, response):
        """ Method that renders a request and returns the response. In this
        case, serves the data.
        """
        request.headers["content-type"] = self.type
        request.headers["content-length"] = str(len(self.data))
        if request.method == "HEAD":
            return ''
        return self.data

    def __str__(self):
        return '<Data data: %s, type: %s>' % (str(self.data), str(self.type))


class WebServer(object):
    """ Cherrypy-based web server.
    """

    CustomResource = CustomResource
    StaticFile = StaticFile
    Data = Data

    msg_already_started = 'tried to start() WebServer when already started'
    msg_already_stopped = 'tried to stop() WebServer when already stopped'

    def __init__(self, listen_url=''):
        """ Constructor for the WebServer class.

        @param listen_url: use this URL for listening. RuntimeError will be
                           raised in case the URL is unusable. If you're
                           on doubt, do not pass any URL and the webserver
                           will generate one for you.

        @type listen_url: string
        """
        self.root_dir = CustomResource(is_root=True)
        self.running = False
        self._listen_host = ''
        self._listen_port = 0

        # Check if it's usable. Otherwise, generate!
        if listen_url:
            self.set_listen_url(listen_url)
        else:
            self._generate_random_address()

    def get_listen_url(self):
        """ Returns the URL currently set to listen on.

        @return: URL
        @rtype: string
        """
        return 'http://%s:%d' % (self._listen_host, self._listen_port)

    def set_listen_url(self, url):
        """ Sets the listening url, if it is usable.
        """
        p = parse_url(url)
        self._listen_host = p.hostname
        self._listen_port = p.port
        if not self.check_url(self._listen_host, self._listen_port):
            raise ValueError('URL cannot be used by the webserver.')

    def get_host(self):
        """ Returns the hostname of the URL currently set to listen on.

        @return: host
        @rtype: string
        """
        return self._listen_host

    def get_port(self):
        """ Returns the port of the URL currently set to listen on.

        @return: port
        @rtype: int
        """
        return self._listen_port

    def add_static_file(self, name, static_file):
        """ Adds a static file to server's tree. It will be available at
        listen_url/name.

        @param name: name to be published
        @param static_file: the static file instance

        @type name: string
        @type static_file: StaticFile
        """
        if static_file:
            self.root_dir.add_static_file(name, static_file)

    def add_resource(self, name, resource):
        """ Adds a resource to server's tree. It will be available at
        listen_url/name.

        @param name: name to be published
        @param resource: resource instance to be added

        @type name: string
        @type resource: Resource
        """
        if resource:
            self.root_dir.add_resource(name, resource)

    def remove_resource(self, name):
        """ Removes a resource from the tree.

        @param name: resource published name
        @type name: string
        """
        if hasattr(self.root_dir, name):
            delattr(self.root_dir, name)

    def is_running(self):
        """ Returns True if the webserver is running, False otherwise.
        """
        return self.running

    def start(self):
        """ Starts the webserver.
        """
        if not self.is_running():
            if not self.get_listen_url():
                self._generate_random_address()
            self._app = cherrypy.tree.mount(self.root_dir)
            self._configure()
            cherrypy.engine.start()
            self.running = True
        else:
            log.warning(self.msg_already_started)

    def stop(self):
        """ Stops the webserver.
        """
        if self.is_running():
            cherrypy.engine.stop()
            cherrypy.server.bus.exit()
            self._cleanup()
            self.running = False
        else:
            log.warning(self.msg_already_stopped)

    def check_url(self, host, port):
        """ Returns True if the webserver will be able to listen on this URL,
        False otherwise.

        @param host: host
        @param port: port

        @type host: string
        @type port: int

        @return: if the URL is usable
        @rtype: bool
        """
        try:
            check_port(host, port)
            return True
        except IOError:
            return False

    def _cleanup(self):
        self.root_dir = None
        self._app = None

    def _generate_random_address(self):
        # Default listen url: localhost:port where port is on the dynamic
        # range (non registered ports)
        ifaces = get_active_ifaces()
        host = None
        port = None
        if ifaces:
            host = get_ip_address(ifaces[0])
        else:
            host = 'localhost'

        while not port:
            port = random.randint(49152, 65535)
            try:
                check_port(host, port)
            except IOError:
                port = 0

        self._listen_host = host
        self._listen_port = port

    def _configure(self):

        class LogDisabler(object):

            def __call__(self, *args, **kwargs):
                pass

            def error(self, *args, **kwargs):
                pass

            def access(self, *args, **kwargs):
                return lambda: None

        if not __enable_webserver_logging__:
            cherrypy.log = LogDisabler()
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
