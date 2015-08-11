# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
#
# Copyright 2006, Elisa team <contact@fluendo.com>
# Copyright 2007-2008, Brisa Team <brisa-develop@garage.maemo.org>

""" Services related classes.
"""

from xml.etree import ElementTree
from brisa import log
from brisa.threading import run_async_call
from brisa.utils import properties
from brisa.utils.network import url_fetch, parse_url, http_call
from brisa.upnp.upnp_publisher import UPnPPublisher
from brisa.upnp.upnp_defaults import UPnPDefaults
from brisa.services.web_server import CustomResource, StaticFile


class ServiceControl(UPnPPublisher, object):
    """ Represents an UPnP Service Control. Through a service control a control
    point can invoke actions on devices and poll for values.

    For using a ServiceControl to deploy service's methods (actions) just
    inherit from it and add your methods with "soap_" preceding the name. For
    example:

    >>> class HelloWorldServiceControl(ServiceControl):
    ...     def __init__(self):
    ...         ServiceControl.__init__(self)
    ...
    ...     def soap_HelloWorld(self):
    ...         return 'Hello World!'


    Remember to set your method as the control attribute of some Service
    instance. This will make it automatically be exported.

    >>> service = Service()
    >>> control = ServiceControl()
    >>> service.control = control
    """
    pass


class Service(object):
    """ Represents an UPnP device Service. Contains information about the
    service, such as service location, UPnP URL's (control, event sub,
    presentation, scpd XML).

    Reference: \
    http://upnp.org/specs/arch/UPnP-arch-DeviceArchitecture-v1.0-20080424.pdf.
    """

    def __init__(self, location, control_url,
                 event_sub_url, presentation_url, scpd_url):
        """ Constructor for the Service class.

        @param location: location of the service
        @param control_url: control url (relative)
        @param event_sub_url: event sub url (relative)
        @param presentation_url: presentation url (relative)
        @param scpd_url: scpd XML url (relative)

        @type location: string
        @type control_url: string
        @type event_sub_url: string
        @type presentation_url: string
        @type scpd_url: string

        @note: relative above means relative to the location
        """
        self._actions = {}
        self._variables = {}
        self._var_subscribers = {}
        self._prop_friendly_name = ""
        self.description_xml_filepath = ""
        self.service_type = ""
        self.event_sid = ""

        self.timeout = 0
        self.location = location
        self.control = None

        parsed = parse_url(self.location)
        self.url_base = "%s://%s" % (parsed[0], parsed[1])

        self.control_url = self._format_url(control_url)
        self.event_sub_url = self._format_url(event_sub_url)
        self.presentation_url = self._format_url(presentation_url)
        self.scpd_url = self._format_url(scpd_url)

        self.friendly_name = self.control_url.split("/")[1]

    def actions(self):
        """ Returns service's actions.

        @rtype: dict
        """
        return self._actions

    def variables(self):
        """ Returns service's variables.

        @rtype: dict
        """
        return self._variables

    def var_subscribers(self):
        """ Returns service's subscribers for variables.

        @rtype: dict
        """
        return self._var_subscribers

    def build(self):
        """ Builds the service description synchronously.
        """
        self.async = False
        self.callback = None
        self._parse_service_description(self.scpd_url)

    def build_async(self, callback):
        """ Builds the service description asynchronously.

        @param callback: callback that will receive this service when finished
                         building
        @type callback: callable
        """
        self.async = True
        self.callback = callback
        self._parse_service_description(self.scpd_url)

    def _format_url(self, url):
        """ Formats a relative URL to '/path/to'.

        url must be either '/path/to' or 'path/to', never the complete URL.

        @param url: relative URL to be formatted
        @type url: string

        @return: the formatted url
        @rtype: string
        """
        if url and not url.startswith('/') and \
           not url.startswith('http'):
            return "/%s" % url
        return url

    def _set_service_id(self, service_id):
        """ Setter for the service_id property.

        @param service_id: id to be set
        @type service_id: string
        """
        prefix = UPnPDefaults.SERVICE_ID_PREFIX
        if service_id == 'X_MS_MediaReceiverRegistrar':
            prefix = UPnPDefaults.SERVICE_ID_MS_PREFIX
        self._prop_service_id = '%s%s' % (prefix, service_id)

    service_id = properties.gen_property_with_default("service_id",
                                                       fset=_set_service_id)

    def _set_subscription_id(self, subscription_id):
        """ Setter for the subscription_id property.

        @param subscription_id: id to be set
        @type subscription_id: string
        """
        self._prop_subscription_id = subscription_id
        if subscription_id:
            subscribe(self)

    subscription_id = properties.gen_property_with_default(
                                                  "subscription_id",
                                                  fset=_set_subscription_id)

    def get_state_variable(self, name):
        """ Returns a state variable.

        @param name: variable name
        @type name: string

        @rtype: StateVariable
        """
        return self._variables.get(name)

    def subscribe_for_variable(self, var_name, callback):
        """

        @param var_name: name of the variable
        @param callback: callback to receive notifications

        @type var_name: string
        @type callback: callable
        """
        variable = self.get_state_variable(var_name)
        if variable:
            variable.subscribe(callback)

    def _fetch_description(self, path):
        """ Fetches a description given its path.

        @param path: location of the description
        @type path: string

        @return: description in a ElementTree representation
        @rtype: ElementTree
        """
        if not path:
            return

        fd = url_fetch(path)
        self._parse_description(fd)

    def _fetch_description_async(self, path):
        """ Fetches a description asynchronously, given its path.
        Forwards success to self.fetch_description_async_done and error to
        self.fetch_description_async_error.

        @param path: location of the description
        @type path: string

        """
        if not path:
            self._fetch_description_async_done()

        if brisa.__skip_service_xml__:
            self._fetch_description_async_done()
            return

        run_async_call(url_fetch,
                       success_callback=self._fetch_description_async_done,
                       error_callback=self._fetch_description_async_error,
                       delay=0, url=path)

    def _fetch_description_async_error(self, cargo, error):
        """ Receives an error when fetching a description asynchronously.

        @param cargo: callback parameters
        @param error: exception raised

        @type error: Exception

        @rtype: boolean
        """
        log.debug("Error fetching service description")
        self._fetch_description_async_done()
        return True

    def _fetch_description_async_done(self, fd=None, cargo=None):
        self._parse_description(fd)

        if self.callback:
            self.callback(self)

    def _parse_description(self, fd):
        """ Receives a file descriptor when fetched with success.

        @param fd: file descriptor

        @type fd: file
        """
        try:
            if fd:
                tree = ElementTree.XML(file=fd).getroot()
            else:
                tree = None
        except:
            tree = None

        if tree:
            self.parse_actions(tree)

    def parse_actions(self, tree):
        """ Parses actions from a fetched tree

        @param tree: tree containing the actions
        @type tree: ElementTree
        """
        ns = "urn:schemas-upnp-org:service-1-0"

        for action_node in tree.findall('.//{%s}action' % ns):
            name = action_node.findtext('{%s}name' % ns)
            arguments = []
            for argument in action_node.findall('.//{%s}argument' % ns):
                arg_name = argument.findtext('{%s}name' % ns)
                arg_direction = argument.findtext('{%s}direction' % ns)
                arg_state_var = \
                    argument.findtext('{%s}relatedStateVariable' % ns)
                arguments.append(Argument(arg_name, arg_direction,
                                          arg_state_var))

            self._actions[name] = Action(self, name, arguments)

        for var_node in tree.findall('.//{%s}stateVariable' % ns):
            events = ["no", "yes"]
            send_events = events.index(var_node.attrib["sendEvents"])
            name = var_node.findtext('{%s}name' % ns)
            data_type = var_node.findtext('{%s}dataType' % ns)
            values = []
            for allowed in var_node.findall('.//{%s}allowedValue' % ns):
                values.append(allowed.text)
            self._variables[name] = StateVariable(self, name, send_events,
                                                  data_type, values)

    def _parse_service_description(self, path):
        """ Fetches a service description and parses its actions.

        @param path: description path
        @type path: string
        """
        if self.async:
            self._fetch_description_async(path)
        else:
            xml = self._fetch_description(path)

    def start_service(self):
        """ Starts the service by adding the scpd and the control to the
        service and returning the resource which will be available (and point
        to the service).

        @return: resource that can be added to the web server for making the
                 service available.
        @rtype: CustomResource
        """
        if self.control is None:
            raise Exception('missing control functions. Set the control \
attribute.')

        service_resource = CustomResource()
        service_resource.add_static_file('scpd.xml',
            StaticFile(self.description_xml_filepath))
        service_resource.add_resource('control', self.control)
        return service_resource

    def event_subscribe(self, event_host, callback, cargo):
        """ Subscribes for events.

        @param event_host: 2-tuple (host, port) with the event listener server.
        @param callback: callback
        @param cargo: callback parameters

        @type event_host: tuple
        @type callback: callable
        """
        if self.event_sid:
            # already registered
            return
        SubscribeRequest(self, event_host, callback, cargo)

    def event_unsubscribe(self, event_host, callback, cargo):
        """ Unsubscribes for events.

        @param event_host: 2-tuple (host, port) with the event listener server.
        @param callback: callback
        @param cargo: callback parameters

        @type event_host: tuple
        @type callback: callable
        """
        if not self.event_sid:
            # not registered
            return
        UnsubscribeRequest(self, event_host, callback, cargo)


class SubscribeRequest(object):
    """ Wrapper for an event subscription.
    """

    def __init__(self, service, event_host, callback, cargo):
        """ Constructor for the SubscribeRequest class.

        @param service: service that is subscribing
        @param event_host: 2-tuple (host, port) of the event listener server
        @param callback: callback
        @param cargo: callback parameters

        @type service: Service
        @type event_host: tuple
        @type callback: callable
        """
        log.debug("subscribe")
        self.callback = callback
        self.cargo = cargo
        self.service = service

        addr = "%s%s" % (service.url_base, service.event_sub_url)
        Paddr = parse_url(addr)

        headers = {}
        headers["Host"] = Paddr.hostname
        headers["User-agent"] = 'BRISA-CP'
        headers["TIMEOUT"] = 'Second-300'
        headers["NT"] = 'upnp:event'
        headers["CALLBACK"] = "<http://%s:%d/eventSub>" % event_host
        headers["HOST"] = '%s:%d' % (Paddr.hostname, Paddr.port)

        run_async_call(http_call, success_callback=self.response,
                       error_callback=self.error, delay=0,
                       method='SUBSCRIBE', url=addr,
                       headers=headers)

    def error(self, cargo, error):
        """ Callback for receiving an error.

        @param cargo: callback parameters passed at construction
        @param error: exception raised

        @type error: Exception

        @rtype: boolean
        """
        log.debug("error", error)
        self.service.event_sid = ""
        self.service.event_timeout = 0
        if self.callback:
            self.callback(self.cargo, "", 0)
        return True

    def response(self, http_response, cargo):
        """ Callback for receiving the HTTP response on a successful HTTP call.

        @param http_response: response object
        @param cargo: callback parameters passed at construction

        @type http_response: HTTPResponse

        @rtype: boolean
        """
        log.debug("response")
        compressed_headers = {}
        for k, v in dict(http_response.getheaders()).items():
            if not v:
                v = ""
            compressed_headers[k.lower()] = v.lower().strip()
            if 'sid' in compressed_headers:
                sid = compressed_headers['sid']
                timeout = 300
                if 'timeout' in compressed_headers:
                    stimeout = compressed_headers['timeout']
                    if stimeout[0:7] == "second-":
                        try:
                            timeout = int(stimeout[7:])
                        except ValueError:
                            pass
                self.service.event_sid = sid
                self.service.event_timeout = timeout
                if self.callback:
                    self.callback(self.cargo, sid, timeout)
        return True


class UnsubscribeRequest(object):
    """ Wrapper for an event unsubscription.
    """

    def __init__(self, service, event_host, callback, cargo):
        """ Constructor for the UnsubscribeRequest class.

        @param service: service that is unsubscribing
        @param event_host: 2-tuple (host, port) of the event listener server
        @param callback: callback
        @param cargo: callback parameters

        @type service: Service
        @type event_host: tuple
        @type callback: callable
        """
        self.old_sid = service.event_sid
        service.event_sid = ""
        service.event_timeout = 0

        self.callback = callback
        self.cargo = cargo
        self.service = service

        addr = "%s%s" % (service.url_base, service.event_sub_url)
        Paddr = parse_url(addr)

        headers = {}
        headers["Host"] = Paddr.hostname
        headers["User-agent"] = 'BRISA-CP'
        headers["HOST"] = '%s:%d' % (Paddr.hostname, Paddr.port)
        headers["SID"] = self.old_sid

        run_async_call(http_call, success_callback=self.response,
                       error_callback=self.error, delay=0,
                       method='UNSUBSCRIBE', url=addr, headers=headers)

    def error(self, cargo, error):
        """ Callback for receiving an error.

        @param cargo: callback parameters passed at construction
        @param error: exception raised

        @type error: Exception

        @rtype: boolean
        """
        if self.callback:
            self.callback(self.cargo, "")
        return True

    def response(self, data, cargo):
        """ Callback for receiving the HTTP response on a successful HTTP call.

        @param data: response object
        @param cargo: callback parameters passed at construction

        @type data: HTTPResponse

        @rtype: boolean
        """
        if self.callback:
            self.callback(self.cargo, self.old_sid)
        return True

    def event_subscribe(self, event_host, callback, cargo):
        """ Subscribes for events.

        @param event_host: 2-tuple (host, port) with the event listener server.
        @param callback: callback
        @param cargo: callback parameters

        @type event_host: tuple
        @type callback: callable
        """
        if self.event_sid:
            # already registered
            return
        SubscribeRequest(self, event_host, callback, cargo)

    def event_unsubscribe(self, event_host, callback, cargo):
        """ Unsubscribes for events.

        @param event_host: 2-tuple (host, port) with the event listener server.
        @param callback: callback
        @param cargo: callback parameters

        @type event_host: tuple
        @type callback: callable
        """
        if not self.event_sid:
            # not registered
            return
        UnsubscribeRequest(self, event_host, callback, cargo)


class Argument:
    """ Represents an action argument.
    """

    def __init__(self, arg_name, arg_direction, arg_state_var):
        """ Constructor for the Argument class.

        @param arg_name: argument name
        @param arg_direction: argument direction
        @param arg_state_var: argument related state variable

        @type arg_name: string
        @type arg_direction: string
        @type arg_state_var: string
        """
        self.name = arg_name
        self.direction = arg_direction
        self.state_Var = arg_state_var


class Action:
    """ Represents a service action.
    """

    def __init__(self, service, name, arguments):
        """ Constructor for the Action class.

        @param service: service which holds this action
        @param name: action name
        @param arguments: arguments list

        @type service: Service
        @type name: string
        @type arguments: list of Argument
        """
        # self.parent_service = service
        self.name = name
        self.arguments = arguments


class StateVariable:
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
        # TODO: check the parameters description and types
        # self.parent_service = service
        self.name = name
        self.send_events = send_events
        self.data_type = data_type
        self.allowed_values = values
