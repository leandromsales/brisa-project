# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.

# Copyright 2006, Elisa team <contact@fluendo.com>
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import sys

from xml.etree import ElementTree
import urllib2
import utils
from upnp_publisher import UPnPPublisher
from twisted.web import resource, static
from upnp_defaults import UPnPDefaults


class ServiceControl(UPnPPublisher, object):

    def __init__(self, title, *args, **kwargs):
        super(ServiceControl, self).__init__(*args)

    def get_actionDict(self):
        assert 0, 'set here the dict of actions'
        return self.actionDic


class Service(object):

    def __init__(self, location, control_url,
                 event_sub_url, presentation_url, scpd_url):
        self._actions = {}
        self._variables = {}
        self._var_subscribers = {}
        self.service_type_version = ""
        self.service_id = ""
        self.subscription_id = ""
        self.friendly_name = ""
        self.timeout = 0
        self.location = location
        self.control = None
        self.control_url = control_url
        self.event_sub_url = event_sub_url
        self.presentation_url = presentation_url
        self.scpd_url = scpd_url
        self.handle_urls(control_url, event_sub_url, presentation_url,
                         scpd_url)
        self.handle_url_base()

    def handle_urls(self, control_url, event_sub_url, presentation_url,
                    scpd_url):
        if not control_url.startswith('/') and not \
            control_url.startswith('http'):
                self.control_url = "/%s" % control_url
        if not event_sub_url.startswith('/') and not \
            control_url.startswith('http'):
                self.event_sub_url = "/%s" % event_sub_url
        if presentation_url and not presentation_url.startswith('/') and not \
            control_url.startswith('http'):
                self.presentation_url = "/%s" % presentation_url
        if not scpd_url.startswith('/') and not control_url.startswith('http'):
            self.scpd_url = "/%s" % scpd_url

    def handle_url_base(self):
        parsed = urllib2.urlparse.urlparse(self.location)
        self.url_base = "%s://%s" % (parsed[0], parsed[1])

    def get_friendly_name(self):
        if self.friendly_name is "":
            self.friendly_name = self.control_url.split("/")[1]

        return self.friendly_name

    def set_service_id(self, service_id):
        self.service_id = UPnPDefaults.SERVICE_ID_PREFIX + service_id

    def set_service_type_version(self, service_type_version):
        self.service_type_version = UPnPDefaults.SERVICE_TYPE_VERSION_PREFIX +\
                                        service_type_version

    def get_scpd_url(self):
        if not self.scpd_url.startswith('http'):
            return self.url_base + self.scpd_url
        return self.scpd_url

    def get_control_url(self):
        if not self.control_url.startswith('http'):
            return self.url_base + self.control_url
        return self.control_url

    def get_event_sub_url(self):
        if not self.event_sub_url.startswith('http'):
            return self.url_base + self.event_sub_url
        return self.event_sub_url

    def get_presentation_url(self):
        if not self.presentation_url.startswith('http'):
            return self.url_base + self.presentation_url
        return self.presentation_url

    def set_sid(self, sid):
        self.subscription_id = sid
        if sid:
            subscribe(self)

    def get_state_variable(self, name):
        return self._variables.get(name)

    def subscribe(self):
        event.subscribe(self)
        global subscribers
        subscribers[self.get_sid()] = self

    def unsubscribe(self):
        pass

    def subscribe_for_variable(self, var_name, callback):
        variable = self.get_state_variable(var_name)
        if variable:
            variable.subscribe(callback)

    def renew_subscription(self):
        pass

    def parse_actions(self):
        handle = utils.url_fetch(self.get_scpd_url())
        if not handle:
            return

        tree = ElementTree(file=handle).getroot()
        ns = "urn:schemas-upnp-org:service-1-0"
        for action_node in tree.findall('.//{%s}action' % ns):
            name = action_node.findtext('{%s}name' % ns)
            arguments = []
            for argument in action_node.findall('.//{%s}argument' % ns):
                arg_name = argument.findtext('{%s}name' % ns)
                arg_direction = argument.findtext('{%s}direction' % ns)
                arg_state_var = argument.\
                                    findtext('{%s}relatedStateVariable' % ns)
                arguments.append(action.Argument(arg_name, arg_direction,
                                                 arg_state_var))
            self._actions[name] = action.Action(self, name, arguments)

        for var_node in tree.findall('.//{%s}stateVariable' % ns):
            events = ["no", "yes"]
            send_events = events.index(var_node.attrib["sendEvents"])
            name = var_node.findtext('{%s}name' % ns)
            data_type = var_node.findtext('{%s}dataType' % ns)
            values = []
            for allowed in var_node.findall('.//{%s}allowedValue' % ns):
                values.append(allowed.text)
            self._variables[name] = variable.StateVariable(self,
                                                           name,
                                                           send_events,
                                                           data_type, values)

    def parse_serviceDescription(path):
        #TODO return a service object from a scpd.xml
        pass

    def startService(self):
        if self.control is None:
            assert 0, 'where is your control functions?'

        serviceResource = resource.Resource()
        serviceResource.putChild("scpd.xml",
                                 static.File(self.create_description_xml()))
        serviceResource.putChild("control", self.control)
        return serviceResource

    def startServiceDesc(self, description):
        if self.control is None:
            assert 0, 'where is your control functions?'

        serviceResource = resource.Resource()
        serviceResource.putChild("scpd.xml", static.File(description))
        serviceResource.putChild("control", self.control)
        return serviceResource

    def create_description_xml(self):
        #TODO: implement-me, please
        if self.description is "":
            self.description = 'do the parse'

        return self.description

    def insertControl(self, control):
        self.control = control
        self._create_actions(control.get_actionDict())

    def _create_actions(self, actionDict):
        pass

    def add_description(self, descName):
        self.description = descName