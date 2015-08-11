# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Static file and data representations.
"""

from cherrypy.lib.static import serve_file

from brisa.services.web_server.resource import CustomResource


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
        request.headers["content-type"] = self.type
        request.headers["content-length"] = str(len(self.data))
        if request.method == "HEAD":
            return ''
        return self.data

    def __str__(self):
        return '<Data data: %s, type: %s>' % (str(self.data), str(self.type))
