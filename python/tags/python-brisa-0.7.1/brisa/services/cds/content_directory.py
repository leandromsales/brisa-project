# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006 John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Content Directory service implementation.
"""

import traceback

from brisa.upnp.service import ServiceControl
from brisa import log
from brisa.services.cds.plugin import PluginManagerSingleton
from brisa.upnp.didl.didl_lite import Element


def compare_objects(a, b):
    """ Compares two DIDL objects by their title for proper sorting.
    """
    if a.title.lower().startswith("all "):
        return -1

    if b.title.lower().startswith("all "):
        return 1

    return cmp(a.title.lower(), b.title.lower())


class ContentDirectory(ServiceControl, dict):
    """ ContentDirectory V1.0 Service implementation.
    """

    namespace = ('u', 'urn:schemas-upnp-org:service:ContentDirectory:1')

    def __init__(self):
        """ Constructor for the ContentDirectory class.
        """
        ServiceControl.__init__(self)
        self.plugin_manager = PluginManagerSingleton
        self.plugin_manager.listener = self
        self.updateID = 0

    def start(self):
        """ Starts the ContentDirectory by loading the plugins.
        """
        self.plugin_manager.load_plugins()

    def soap_GetSearchCapabilities(self, *args, **kwargs):
        """ Returns the search capabilities supported by the device.
        """
        log.debug('GetSearchCapabilities()')
        # While the search method is not properly implemented, the server does
        # not support search.
        return {'SearchCapabilitiesResponse': {'SearchCaps': ''}}

    def soap_GetSortCapabilities(self, *args, **kwargs):
        """ Returns the CSV list of meta-data tags that can be used in
        sortCriteria.
        """
        log.debug('GetSortCapabilities()')
        return {'SortCapabilitiesResponse': {'SortCaps': 'dc:title'}}

    def soap_GetSystemUpdateID(self, *args, **kwargs):
        """ Returns the current value of state variable SystemUpdateID.
        """
        log.debug('GetSystemUpdateID()')
        return {'GetSystemUpdateIDResponse': {'Id': self.updateID}}

    def soap_Browse(self, *args, **kwargs):
        """ Implements the Browse action for the ContentDirectory.
        """
        # Formats the parameters for the real soap browse
        args = (kwargs['ObjectID'], kwargs['BrowseFlag'], kwargs['Filter'],
                kwargs['StartingIndex'], kwargs['RequestedCount'],
                kwargs['SortCriteria'])
        l = {}
        if args[0] in self:
            l['object'] = self[args[0]]
        l['query'] = 'Browse(ObjectID=%s, BrowseFlags=%s, Filter=%s, ' \
            'StartingIndex=%s RequestedCount=%s SortCriteria=%s)' % \
            tuple(map(repr, args))
        try:
            ret = self._soap_Browse(*args)
        except Exception, e:
            log.debug('soap_Browse error: %s' % e.message)

        l['response'] = ret

        return ret

    def _soap_Browse(self, *args):
        """ Real implementation of the soap browse. Forwards the browse to the
        respective plugin and returns the browse.
        """
        (object_id, browse_flag, filter, starting_index, requested_count,
         sort_criteria) = args
        try:
            starting_index = int(starting_index)
            last_index = None
            plugin = self.plugin_manager.root_plugin

            if browse_flag == 'BrowseDirectChildren' and \
            requested_count != '0':
                last_index = int(requested_count) + starting_index

            log.debug('object_id is %s' % str(object_id))

            if ':' in object_id:
                namespace, object_id = object_id.split(':')
                plugin = self.plugin_manager.plugins_instances[namespace]

                if not plugin:
                    raise Exception("Could not get plugin associated with this\
                                    browse action")

            elements = plugin.browse(object_id, browse_flag, filter,
                                      starting_index, requested_count,
                                      sort_criteria)

            elements.sort(cmp=compare_objects)
            didl = Element()
            total = 0

            if plugin.has_browse_filter:
                for item in elements:
                    didl.add_item(item)
                    total = total + 1
            else:
                for item in elements[starting_index: last_index]:
                    didl.add_item(item)
                    total = total + 1

            didl_xml = didl.to_string()
            log.debug('didl browse: %s' % didl_xml)
            log.debug(u'didl browse: %s' % unicode(didl_xml))
            soap_result = {'Result': didl_xml,
                           'TotalMatches': len(elements),
                           'NumberReturned': total,
                           'UpdateID': self.updateID}
            soap_result = {'BrowseResponse': soap_result}
        except Exception, e:
            soap_result = {'BrowseResponse': {'Result': '',
                                              'TotalMatches': 0,
                                              'NumberReturned': 0,
                                              'UpdateID': self.updateID}}
            log.debug('Exception happened during _soap_Browse: %s' % e.message)

        return soap_result

    def soap_Search(self, *args, **kwargs):
        """ Search for objects that match some search criteria.

        TODO: forward the search to the respective plugin.
        TODO: implement the search.
        """
        return {'SearchResponse': {'Result': '',
                                   'NumberReturned': 0,
                                   'TotalMatches': 0,
                                   'UpdateID': self.updateID}}

    def on_start_plugin(self, plugin_name):
        """ Listener method for the END plugin event.
        """
        log.info('Media Server received START plugin event :: %s', plugin_name)

    def on_end_plugin(self, plugin_name):
        """ Listener method for the END plugin event.
        """
        log.info('Media Server received END plugin event :: %s', plugin_name)
