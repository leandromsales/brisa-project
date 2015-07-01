# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>
#
# Copyright 2001-2004, Twisted Matrix Laboratories.
# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006 John-Mark Gurney <gurney_j@resnet.uroegon.edu>

""" Facilities for publishing soap methods.
"""


from brisa import log
from brisa.upnp import soap
from brisa.utils.python import failure
from brisa.services.web_server import CustomResource


class ErrorCode(Exception):
    """ Wrapper for an error code. Contains a status attribute that corresponds
    with the error code.
    """

    def __init__(self, status):
        self.status = status


class UPnPPublisher(CustomResource):
    """ Wrapper for receiving soap calls and assigning them to correspondent
    methods. Extend UPnPPublisher and add the class to the web server as a
    resource and your methods will be exported.
    """

    isLeaf = 1
    namespace = ('', '')
    encoding = "UTF-8"

    def __init__(self):
        CustomResource.__init__(self)

    def getChildWithDefault(self, uri, params):
        # CustomResource child that render requests is UPnPPublisher itself.
        return self

    def lookup_function(self, function_name):
        """ Lookup published SOAP function.
        """
        return getattr(self, "soap_%s" % function_name, None)

    def _getCallResponse(self, request, response_obj, method_name,
                         function, *args, **kwargs):
        """ Performs the soap call, builds and returns a response.
        """
        result = function(*args, **kwargs)

        ns = self.namespace[1]
        try:
            method = result.keys()[0]
            result = result[method]
        except AttributeError, IndexError:
            result = {}
            method = ''
        response = soap.build_soap_call("{%s}%s" % (ns, method),
                                        result, encoding=None)
        return self._buildResponse(request, response, response_obj)

    def _buildError(self, failure, request, method_name, response_obj):
        """ Builds an error based on the failure code.
        """
        e = failure.value
        status = 500

        if isinstance(e, ErrorCode):
            status = e.status
        else:
            failure.printTraceback()

        response = soap.build_soap_error(status)
        return self._buildResponse(request, response, response_obj,
                                   status=status)

    def render(self, uri, request, response):
        """ Renders a request received.
        """
        data = request.body.read()
        headers = request.headers

        method_name, args, kwargs, ns = soap.parse_soap_call(data)

        try:
            headers['content-type'].index('text/xml')
        except:
            # Builds error if we don't have an content-type field with xml
            return self._buildError(failure.Failure(ErrorCode(415)), request,
                           method_name, response)

        function = self.lookup_function(method_name)

        if not function:
            return self._methodNotFound(request, response, method_name)
        else:
            return self._getCallResponse(request, response, method_name,
                                         function, *args, **kwargs)

        return None

    def _buildResponse(self, request, response, response_object, status=200):
        """ Builds a response for a call.
        """
        if status == 200:
            response_object.status = 200
        else:
            response_object.status = 500

        if self.encoding is not None:
            mime_type = 'text/xml; charset="%s"' % self.encoding
        else:
            mime_type = "text/xml"
        response_object.headers["Content-type"] = mime_type
        response_object.headers["Content-length"] = str(len(response))
        response_object.headers["EXT"] = ''
        response_object.body = response
        return response

    def _methodNotFound(self, request, response_obj, method_name):
        """ Treats the method not found error.
        """
        response = soap.build_soap_error(401)
        return self._buildResponse(request, response, response_obj, status=401)
