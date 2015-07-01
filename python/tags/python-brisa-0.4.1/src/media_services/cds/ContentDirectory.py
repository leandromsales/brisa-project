# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006 John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from twisted.python import log
from twisted.web import resource, static
from twisted.internet import defer
from twisted.python import failure

import os
import sys
if sys.hexversion >= 0x20500f0:
    from xml.etree import ElementTree
    from xml.etree.ElementTree import Element, SubElement, tostring
else:
    from elementtree.ElementTree import Element, SubElement, tostring

from persistence import Facade
from persistence import Persistence
from media_services.cds.didl import DIDLLite
from upnp import debug
import traceback
from urllib import quote
from upnp import ServiceControl
from upnp import errorCode

from didl.DIDLLite import Container

class ContentDirectory(ServiceControl, dict):
    BrowseFlags = ('BrowseMetaData', 'BrowseDirectChildren')
    updateID = 0

    def __init__(self):
        pass

    def soap_GetSearchCapabilities(self, *args, **kwargs):
        """Required: Return the searching capabilities supported by
        the device."""
        log.msg('GetSearchCapabilities()')
        return {'SearchCapabilitiesResponse': { 'SearchCaps': \
                                               'dc:title, upnp:artist' }}

    def soap_GetSortCapabilities(self, *args, **kwargs):
        """Required: Return the CSV list of meta-data tags that can be used in
        sortCriteria."""
        log.msg('GetSortCapabilities()')
        return { 'SortCapabilitiesResponse': { 'SortCaps': '' }}

    def soap_GetSystemUpdateID(self, *args, **kwargs):
        """Required: Return the current value of state variable
        SystemUpdateID."""

        log.msg('GetSystemUpdateID()')
        return { 'SystemUpdateIdResponse': { 'Id': self.updateID }}

    def soap_Browse(self, *args):
        l = {}
        #debug.appendnamespace(reqname, l)
        if args[0] in self:
            l['object'] = self[args[0]]
        l['query'] = 'Browse(ObjectID=%s, BrowseFlags=%s, Filter=%s, ' \
            'StartingIndex=%s RequestedCount=%s SortCriteria=%s)' % \
            tuple(map(repr, args))
        try:
            ret = self.thereal_soap_Browse(*args)
        except defer.Deferred, x:
            ret = doRecallgen(x, self.soap_Browse, *args)

        l['response'] = ret

        return ret

    def thereal_soap_Browse(self, *args):
        """Required: Incrementally browse the native heirachy of the Content
        Directory objects exposed by the Content Directory Service."""

        (ObjectID, BrowseFlag, Filter, StartingIndex, RequestedCount,
            SortCriteria) = args

        StartingIndex = int(StartingIndex)
        RequestedCount = int(RequestedCount)

        didl = DIDLLite.DIDLElement()

        ObjectID = int(ObjectID)

        root_object = None

        if ObjectID is 0:
            root_object = Facade.selectObjectById(1)
            #TODO: is this really necessary?
            ObjectID = 0
        else:
            root_object = Facade.selectObjectById(ObjectID)

        total = 0

        if BrowseFlag == 'BrowseDirectChildren':
            if isinstance(root_object, Persistence.Container):
                indexTotal = len(root_object.items) + \
                                    len(root_object.containers)
                childrenList = []
                if len(root_object.containers) is not 0:
                    for i in root_object.containers:
                        childrenList.append(i)

                if len(root_object.items) is not 0:
                    for i in root_object.items:
                        childrenList.append(i)

                for i in \
                    childrenList[StartingIndex: StartingIndex+RequestedCount]:
                        i.setParentId(str(ObjectID))
                        didl.addItem(i)
                        total = total + 1

        else:
                if ObjectID is 0:
                    root_object.setParentId("-1")
                    didl.addItem(root_object)
                else:
                    if isinstance(root_object, Persistence.Container):
                        root_object.\
                            setParentId(str(root_object.container.id))
                    else:
                        root_object.\
                            setParentId(str(root_object.containers[0].id))

                    didl.addItem(root_object)

                total = 1

        r = { 'Result': didl.toString(), 'TotalMatches': total,
                'NumberReturned': didl.numItems(), }

        #TODO: didl update stuff
        r['UpdateID'] = 0
        result = { 'BrowseResponse': r }
        return result

        # Optional actions
    def soap_Search(self, *args, **kwargs):
        """Search for objects that match some search criteria."""

        (ContainerID, SearchCriteria, Filter, StartingIndex,
         RequestedCount, SortCriteria) = args

        didl = DIDLLite.DIDLElement()

        ContainerID = int(ContainerID)

        total = 0

        wantedTitle = "None"
        objects = Facade.searchObjectByContainInTitle(wantedTitle)

        if SearchCriteria.find("dc:title") is not -1:
            looked = SearchCriteria.split("dc:title")[1]
            if looked.find("=") is not -1:
                wantedTitle = looked.split("=")[1]
                wantedTitle = wantedTitle.strip('")( ')

            if looked.find("contains") is not -1:
                wantedTitle = looked.split("contains")[1]
                wantedTitle = wantedTitle.strip('")( ')

            objects = Facade.searchObjectByContainInTitle(wantedTitle)
            if objects.count() is not 0:
                for obj in objects[int(StartingIndex):int(StartingIndex)+ \
                                   int(RequestedCount)]:
                    # Container always have a id number less than its Items
                    if obj.id <= ContainerID:
                        pass
                    else:
                        if isinstance(obj, Persistence.Container):
                            obj.setParentId(str(obj.container.id))
                        else:
                            obj.setParentId(str(obj.containers[0].id))
                        didl.addItem(obj)
                        total = total + 1

        if SearchCriteria.find("upnp:artist") is not -1:
            looked = SearchCriteria.split("upnp:artist")[1]
            print looked
            if looked.find("=") is not -1:
                wantedTitle = looked.split("=")[1]
                wantedTitle = wantedTitle.strip('")( ')

            if looked.find("contains") is not -1:
                wantedTitle = looked.split("contains")[1]
                wantedTitle = wantedTitle.strip('")( ')

            objects = Facade.selectMusicArtistByTitle(wantedTitle)
            if objects.count() is not 0:
                for obj in objects[int(StartingIndex):int(StartingIndex)+ \
                                   int(RequestedCount)]:
                    # Container always have a id number less than its Items
                    if obj.id <= ContainerID:
                        pass
                    else:
                        if len(obj.items) is not 0:
                            for i in obj.items[int(StartingIndex):\
                                               int(StartingIndex)+ \
                                               int(RequestedCount)]:
                                i.setParentId(str(obj.id))
                                didl.addItem(i)
                                total = total + 1

        log.msg('Search(ContainerID=%s, SearchCriteria=%s, Filter=%s, ' \
            'StartingIndex=%s, RequestedCount=%s, SortCriteria=%s)' %
            (`ContainerID`, `SearchCriteria`, `Filter`,
            `StartingIndex`, `RequestedCount`, `SortCriteria`))

        r = { 'Result': didl.toString(), 'TotalMatches': total,
                'NumberReturned': didl.numItems(), }

        r['UpdateID'] = 0

        result = { 'SearchResponse': r }


