# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006 John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Content Directory service implementation.
"""

import traceback
from datetime import datetime
import time

from brisa.upnp.service import ServiceControl
from brisa import log
from brisa.services.cds.plugin import PluginManager
from brisa.upnp.didl.didl_lite import Element


def compare_objects(a, b):
    """ Compares two DIDL objects by their title for proper sorting.

    @param a: A DIDL object to be compared with.
    @param b: A DIDL object to compare with.

    @type a: DIDL
    @type b: DIDL

    @return: The comparation result.
    @rtype: boolean
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

    def __init__(self, plugins_folder, plugins_module_str):
        """ Constructor for the ContentDirectory class.

        @param plugins_folder: Name of the plugins folder.
        @param plugins_module_str: Path of the plugins folder.

        @type plugins_folder: string
        @type plugins_module_str: string
        """
        ServiceControl.__init__(self)
        self.plugin_manager = PluginManager(plugins_folder, plugins_module_str)
        self.plugin_manager.listener = self
        self.updateID = 0

    def start(self):
        """ Starts the ContentDirectory by loading the plugins.
        """
        self.plugin_manager.load_plugins()

    def stop(self):
        """ Stops the ContentDirectory servie by unloading the plugins.
        """
        self.plugin_manager.unload_plugins()

    def soap_GetSearchCapabilities(self, *args, **kwargs):
        """ Returns the search capabilities supported by the device.

        @param args: list of arguments for the GetSearchCapabilities UPnP\
        function
        @param kwargs: dict of arguments for the GetSearchCapabilities UPnP\
        function

        @type args: list
        @type kwargs: dict

        @return: the search capabilities supported by the device
        @rtype: dict
        """
        log.debug('Received action on ContentDirectory service: '\
                  'GetSearchCapabilities()')
        # While the search method is not properly implemented, the server does
        # not support search.
        return {'SearchCapabilitiesResponse': {'SearchCaps': ''}}

    def soap_GetSortCapabilities(self, *args, **kwargs):
        """ Returns the CSV list of meta-data tags that can be used in
        sortCriteria.

        @param args: list of arguments for the GetSortCapabilities UPnP\
        function
        @param kwargs: dict of arguments for the GetSortCapabilities UPnP\
        function

        @type args: list
        @type kwargs: dict

        @return: CSV list of meta-data
        @rtype: dict
        """
        log.debug('Received action on ContentDirectory service: '\
                  'GetSortCapabilities()')
        return {'SortCapabilitiesResponse': {'SortCaps': 'dc:title'}}

    def soap_GetSystemUpdateID(self, *args, **kwargs):
        """ Returns the current value of state variable SystemUpdateID.

        @param args: list of arguments for the GetSystemUpdateID UPnP\
        function
        @param kwargs: dict of arguments for the GetSystemUPdateID UPnP\
        function

        @type args: list
        @type kwargs: dict

        @return: current value of SystemUpdateID
        @rtype: dict
        """
        log.debug('Received action on ContentDirectory service: '\
                  'GetSystemUpdateID()')
        return {'GetSystemUpdateIDResponse': {'Id': self.updateID}}

    def soap_Browse(self, *args, **kwargs):
        """ Implements the Browse action for the ContentDirectory.

        @param args: list of arguments for the Browse UPnP\
        function
        @param kwargs: dict of arguments for the Browse UPnP\
        function

        @type args: list
        @type kwargs: dict

        @return: the results of the browsing
        @rtype: dict
        """
        begin = time.time()
#        begin = datetime.now()
        log.debug('BENCHMARK Browser BEG: %s', str(begin))
        
        log.debug('Received action on ContentDirectory service: '\
                  'Browse%s', args)

        # Formats the parameters for the real soap browse
        args = (kwargs['ObjectID'], kwargs['BrowseFlag'], kwargs['Filter'],
                kwargs['StartingIndex'], kwargs['RequestedCount'],
                kwargs['SortCriteria'], kwargs['CpId'])
        l = {}
        if args[0] in self:
            l['object'] = self[args[0]]
        l['query'] = 'Browse(ObjectID=%s, BrowseFlags=%s, Filter=%s, ' \
            'StartingIndex=%s RequestedCount=%s SortCriteria=%s CpId=%s)' % \
            tuple(map(repr, args))
        try:
            ret = self._soap_Browse(*args)
        except Exception, e:
            log.error('ContentDirectory.Browse problem: %s', e.message)

        l['response'] = ret

        CpId = "%s %s %s %s %s %s %s" % tuple(map(repr, args))
        CpId = (CpId.split(" ")[-1]).strip()
        log.debug('BENCHMARK Browser CPID: %s', str(CpId))
        end = time.time()
#        end = datetime.now()
        time_delta = end - begin
#        microseconds = time_delta.microseconds
        microseconds = time_delta
        log.debug('BENCHMARK Browser END: %s', str(end))
        log.debug('BENCHMARK Browser DIFF: %s', str(microseconds))

        return ret

    def _soap_Browse(self, *args):
        """ Real implementation of the soap browse.

        @param args: list of arguments for the Browse UPnP function
        @param kwargs: dict of arguments for the Browse UPnP function

        @type args: list
        @type kwargs: dict

        @return: the results of browsing
        @rtype: dict
        """
        (object_id, browse_flag, filter, starting_index, requested_count,
         sort_criteria, cpId) = args
        try:
            starting_index = int(starting_index)
            requested_count = int(requested_count)
            last_index = None
            plugin = self.plugin_manager.root_plugin

            if browse_flag == 'BrowseDirectChildren' and \
            requested_count != 0:
                last_index = requested_count + starting_index

            if ':' in object_id:
                namespace = object_id.split(':')[0]
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
            log.error('ContentDirectory.Browse internal problem: %s', e)

        return soap_result

    def soap_Search(self, *args, **kwargs):
        """ Search for objects that match some search criteria.

        @param args: list of arguments for the Search UPnP function
        @param kwargs: dict of arguments for the Search UPnP function

        @type args: list
        @type kwargs: dict

        TODO: forward the search to the respective plugin.
        TODO: implement the search.
        """
        return {'SearchResponse': {'Result': '',
                                   'NumberReturned': 0,
                                   'TotalMatches': 0,
                                   'UpdateID': self.updateID}}
