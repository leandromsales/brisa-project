# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Builder module for services.
"""

from xml.etree import cElementTree as ElementTree

from brisa.core import log

from brisa.upnp.base_action import BaseArgument, BaseAction
from brisa.upnp.base_service import BaseStateVariable


class BaseServiceBuilder(object):

    def __init__(self, service, fd):
        self.service = service
        self.fd = fd
        self._actions = {}
        self._variables = {}

    def build(self):
        if not self._parse_description(self.fd):
            return False
        self._build_service()
        return True

    def _parse_description(self, fd):
        """ Parses the actions and state variables of a service given a file
        descriptor containing the SCPD XML. File descriptor must be open.

        @param fd: file descriptor
        @type fd: file

        @return: True if service build succeeded, otherwise False.
        @rtype: bool
        """
        try:
            data = fd.read()
            data = data[data.find("<"):data.rfind(">")+1]
            tree = ElementTree.XML(data)
            if tree:
                self._parse_actions(tree)
                self._parse_variables(tree)
                return True
        except Exception, e:
            log.debug('Could not build service SCPD XML. %s' % str(e))

        return False

    def _parse_actions(self, tree):
        """ Parses actions from a fetched tree.

        @param tree: tree containing the actions
        @type tree: ElementTree
        """
        ns = "urn:schemas-upnp-org:service-1-0"
        for node in tree.findall('.//{%s}action' % ns):
            name = node.findtext('{%s}name' % ns)
            args = []
            for arg in node.findall('.//{%s}argument' % ns):
                arg_direction = arg.findtext('{%s}direction' % ns)
                arg_state_var = arg.findtext('{%s}relatedStateVariable' % ns)
                arg_name = arg.findtext('{%s}name' % ns)
                args.append((arg_name, arg_direction, arg_state_var))
            self._actions[name] = args

    def _parse_variables(self, tree):
        """ Parses variables from a fetched tree.

        @param tree: tree containing the actions
        @type tree: ElementTree
        """
        ns = "urn:schemas-upnp-org:service-1-0"
        for node in tree.findall('.//{%s}stateVariable' % ns):
            events = ["no", "yes"]
            # Avoid breaking when sendEvents is not specified.
            send_events = events.index(node.attrib.get('sendEvents', 'no'))
            name = node.findtext('{%s}name' % ns)
            data_type = node.findtext('{%s}dataType' % ns)
            values = [a.text for a in node.findall('.//{%s}allowedValue' % ns)]
            self._variables[name] = (send_events, data_type, values)

    def _build_service(self):
        for name in self._variables.iterkeys():
            send_events, data_type, values = self._variables[name]
            self.service._state_variables[name] = self.\
                        _create_state_var(name, send_events,
                                       data_type, values)
        for action_name, action_args in self._actions.iteritems():
            args = []
            for arg_name, arg_direction, arg_state_var in action_args:
                args.append(self._create_argument(arg_name,
                    arg_direction,
                    self.service._state_variables[arg_state_var]))
            self.service._actions[action_name] = self.\
                _create_action(action_name, args)

    def _create_argument(self, arg_name, arg_direction, arg_state_var):
        return BaseArgument(arg_name, arg_direction, arg_state_var)

    def _create_action(self, name, args):
        return BaseAction(self.service, name, args)

    def _create_state_var(self, name, send_events, data_type, values):
        return BaseStateVariable(self.service,
                    name, send_events, data_type, values)
