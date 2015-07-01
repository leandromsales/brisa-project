# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Resource for the web server.
"""

import cherrypy


class Resource:
    """ Class representing a resource. Resources can contain
    resources and static files. One should use CustomResource
    class instead of this.
    """

    def __init__(self):
        self.root = None
        self.is_root = False
        self.exposed = True

    def __nonzero__(self):
        """ This fixes a bug that happened with some classes that inherit from
        Resource also inherit from basic types like dict. If the dict is empty,
        then this class would evaluate to False, giving unexpected results on
        codes like "if object:".
        """
        return True

    def add_resource(self, name, resource, exposed=True):
        """ Adds a new resource """
        if name == '' and not self.root:
            self.root = resource
        else:
            setattr(self, name.replace('.', '_'), resource)

    def add_static_file(self, name, static_file, exposed=True):
        """ Adds a static file """
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
        child = self.getChildWithDefault(uri, params)

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

    def getChildWithDefault(self, uri, params=None):
        """ Should be overwritten when needed. This method may apply (or not)
        some logic with the uri and parameters passed and match it with a
        child. For example, if a resource represents a folder, then it will use
        the uri and params to find the file or sub folder.
        """
        return None

    def __str__(self):
        return "<CustomResource, Root: %s, Exposed: %s, Render: %s>" % (
                 str(self.is_root),
                 str(self.exposed),
                 str(hasattr(self, 'render')))


class CustomResource(Resource):
    """ Points to the latest stable Resource class. We recommend to use
    CustomResource always instead of Resource, for coding maintenance purposes.
    """
    pass
