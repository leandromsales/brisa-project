# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008, Brisa Team <brisa-develop@garage.maemo.org>

""" Basic service classes

If you want a device-side service class (for publishing a service of yours),
please refer to module brisa.upnp.device.service.

If you want a controlpoint-side service class (for performing actions on
services), please refer to module brisa.upnp.control_point.service.
"""

__all__ = ('BaseService', 'BaseStateVariable', 'Argument', 'Action')


from brisa.core.network import parse_url


def format_rel_url(url):
    """ Formats from the form path/to to /path/to, if required.

    @return: formatted string
    @rtype: string
    """
    if not url:
        return url
    if not url.startswith('/'):
        return '/%s' % url
    else:
        return url


def parse_base_url(url):
    parsed = parse_url(url)
    return '%s://%s' % (parsed[0], parsed[1])


class BaseService(object):

    def __init__(self, id, serv_type, url_base):
        """ Constructor for the BaseService class.

        @param serv_type: service type
        @param url_base: base URL on the form schema://URL

        @type serv_type: string
        @type url_base: string

        @note: url_base is expected on the form schema://URL. For example,
               suppose your device is located at http://192.168.1.15 and
               available on the specific port 12345. Also suppose your device
               has a service called Hello, with its resources available on the
               following addresses:

               Hello SCPD:      http://192.168.1.15:12345/Hello/SCPD
               Hello Control:   http://192.168.1.15:12345/Hello/Control
               Hello EventSub:  http://192.168.1.15:12345/Hello/Event
               Hello Present.:  http://192.168.1.15:12345/Hello/Presentation

               These addresses can be written in the form url_base/rel_url.
               In this case, url_base is http://192.168.1.15:12345 and
               rel_urls are /Hello/SCPD, /Hello/Control, and so on.

               Relative URL's must begin with '/' and the base URL must not end
               in '/' (not http://192.168.1.15:12345/).
        """
        self.id = id
        self.service_type = serv_type
        self.url_base = parse_base_url(url_base)
        self.scpd_url = '/%s/%s' % (id, 'scpd.xml')
        self.control_url = '/%s/%s' % (id, 'control')
        self.event_sub_url = '/%s/%s' % (id, 'eventSub')
        self.presentation_url = '/%s/%s' % (id, 'presentation')
        self._actions = {}
        self._state_variables = {}
        self._eventing_variables = {}

    def get_actions(self):
        return self._actions

    def get_variables(self):
        return self._state_variables


class BaseStateVariable(object):
    """ Represents a service state variable.
    """

    def __init__(self, service, name, send_events, data_type, values):
        """ Constructor for the StateVariable class.

        @param service: Service which holds this variable
        @param name: variable name
        @param send_events: send events option
        @param data_type: data type
        @param values: values

        @type service: Service
        @type name: string
        @type send_events: string
        @type data_type: string
        @type values: string
        """
        self.parent_service = service
        self.name = name
        self.send_events = send_events
        self.data_type = data_type
        self.allowed_values = values
