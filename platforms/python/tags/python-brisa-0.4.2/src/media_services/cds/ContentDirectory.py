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
from xml.etree import ElementTree
from xml.etree.ElementTree import Element, SubElement, tostring

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
        return {'SearchCapabilitiesResponse': {'SearchCaps': \
                                               'dc:title, upnp:artist'}}

    def soap_GetSortCapabilities(self, *args, **kwargs):
        """Required: Return the CSV list of meta-data tags that can be used in
        sortCriteria."""
        log.msg('GetSortCapabilities()')
        return {'SortCapabilitiesResponse': {'SortCaps': ''}}

    def soap_GetSystemUpdateID(self, *args, **kwargs):
        """Required: Return the current value of state variable
        SystemUpdateID."""

        log.msg('GetSystemUpdateID()')
        return {'SystemUpdateIdResponse': {'Id': self.updateID}}

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

        r = {'Result': didl.toString(), 'TotalMatches': total,
                'NumberReturned': didl.numItems(), }

        #TODO: didl update stuff
        r['UpdateID'] = 0
        result = {'BrowseResponse': r}
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
                for obj in objects[int(StartingIndex): int(StartingIndex)+ \
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
                for obj in objects[int(StartingIndex): int(StartingIndex)+ \
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

        r = {'Result': didl.toString(), 'TotalMatches': total,
                'NumberReturned': didl.numItems(), }

        r['UpdateID'] = 0

        result = {'SearchResponse': r}

        return result

    def soap_CreateObject(self, *args, **kwargs):
        """Create a new object."""

        (ContainerID, Elements) = args

        log.msg('CreateObject(ContainerID=%s, Elements=%s)' %
            (`ContainerID`, `Elements`))

    def soap_DestroyObject(self, *args, **kwargs):
        """Destroy the specified object."""

        (ObjectID) = args

        log.msg('DestroyObject(ObjectID=%s)' % `ObjectID`)

    def soap_UpdateObject(self, *args, **kwargs):
        """Modify, delete or insert object metadata."""

        (ObjectID, CurrentTagValue, NewTagValue) = args

        log.msg('UpdateObject(ObjectID=%s, CurrentTagValue=%s, ' \
            'NewTagValue=%s)' % (`ObjectID`, `CurrentTagValue`,
            `NewTagValue`))

    def soap_ImportResource(self, *args, **kwargs):
        """Transfer a file from a remote source to a local
        destination in the Content Directory Service."""

        (SourceURI, DestinationURI) = args

        log.msg('ImportResource(SourceURI=%s, DestinationURI=%s)' %
            (`SourceURI`, `DestinationURI`))

    def soap_ExportResource(self, *args, **kwargs):
        """Transfer a file from a local source to a remote
        destination."""

        (SourceURI, DestinationURI) = args

        log.msg('ExportResource(SourceURI=%s, DestinationURI=%s)' %
            (`SourceURI`, `DestinationURI`))

    def soap_StopTransferResource(self, *args, **kwargs):
        """Stop a file transfer initiated by ImportResource or
        ExportResource."""

        (TransferID) = args

        log.msg('StopTransferResource(TransferID=%s)' % TransferID)

    def soap_GetTransferProgress(self, *args, **kwargs):
        """Query the progress of a file transfer initiated by
        an ImportResource or ExportResource action."""

        (TransferID, TransferStatus, TransferLength, TransferTotal) = args

        log.msg('GetTransferProgress(TransferID=%s, TransferStatus=%s, ' \
            'TransferLength=%s, TransferTotal=%s)' %
            (`TransferId`, `TransferStatus`, `TransferLength`,
            `TransferTotal`))

    def soap_DeleteResource(self, *args, **kwargs):
        """Delete a specified resource."""

        (ResourceURI) = args

        log.msg('DeleteResource(ResourceURI=%s)' % `ResourceURI`)

    def soap_CreateReference(self, *args, **kwargs):
        """Create a reference to an existing object."""

        (ContainerID, ObjectID) = args

        log.msg('CreateReference(ContainerID=%s, ObjectID=%s)' %
            (`ContainerID`, `ObjectID`))

    def __repr__(self):
        return '<ContentDirectoryControl: cnt: %d, urlbase: %s, nextID: %d>' \
                    % (len(self), `self.urlbase`, self.nextID)

    def get_actionDict(self):

        return None


class ContentDirectoryControl(ServiceControl, dict):
    """This class implements the CDS actions over SOAP."""

    updateID = property(lambda x: x['0'].updateID)
    urlbase = property(lambda x: x._urlbase)

    def getnextID(self):
        ret = str(self.nextID)
        self.nextID += 1
        return ret

    def addContainer(self, parent, title, klass = Container, *args, **kwargs):
        ret = self.addObject(parent, klass, title, *args, **kwargs)
        self.children[ret] = self[ret]
        return ret

    def addItem(self, parent, klass, title, *args, **kwargs):
        if issubclass(klass, Container):
            return self.addContainer(parent, title, klass, *args, **kwargs)
        else:
            return self.addObject(parent, klass, title, *args, **kwargs)

    def addObject(self, parent, klass, title, *args, **kwargs):
        '''
        If the generated object (by klass) has an attribute content,
        it is installed into the web server.
        '''
        assert isinstance(self[parent], Container)
        nid = self.getnextID()
        i = klass(self, nid, parent, title, *args, **kwargs)
        if hasattr(i, 'content'):
            self.webbase.putChild(nid, i.content)
        #log.msg('children:', `self.children[parent]`, `i`)
        self.children[parent].append(i)
        self[i.id] = i
        return i.id

    def has_key(self, key):
        return dict.has_key(self, key)

    def delItem(self, id):
        if not id in self:
            log.msg('already removed:', id)
            return
        if isinstance(self[id], Container):
            while self.children[id]:
                self.delItem(self.children[id][0].id)
            assert len(self.children[id]) == 0
            del self.children[id]
        # Remove from parent
        self.children[self[id].parentID].remove(self[id])
        # Remove content
        if hasattr(self[id], 'content'):
            self.webbase.delEntity(id)
        del self[id]

    def getchildren(self, item):
        assert isinstance(self[item], Container)
        return self.children[item][: ]

    def __init__(self, title, *args, **kwargs):
        #debug.insertringbuf(reqname)
        #super(ContentDirectoryControl, self).__init__(*args)
        self.webbase = kwargs['webbase']
        self._urlbase = kwargs['urlbase']
        del kwargs['webbase'], kwargs['urlbase']
        fakeparent = '-1'
        self.nextID = 0
        self.children = {fakeparent: []}
        self[fakeparent] = Container(None, None, '-1', 'fake')
        #Create the root container
        root = self.addContainer(fakeparent, title)
        assert root == '0'

        # Add all Storeges Directories descripted by
        # storageDirectoryList in the root container
        for i in kwargs['storageDirectoryList']:
            self.addContainer(root, i['title'],
                                 path = i['path'],
                                 klass = i['klass'])
        del self[fakeparent]
        del self.children[fakeparent]

    # Required actions

    def soap_GetSearchCapabilities(self, *args, **kwargs):
        """Required: Return the searching capabilities supported by
        the device."""

        log.msg('GetSearchCapabilities()')
        return {'SearchCapabilitiesResponse': {'SearchCaps': ''}}

    def soap_GetSortCapabilities(self, *args, **kwargs):
        """Required: Return the CSV list of meta-data tags that can be used in
        sortCriteria."""

        log.msg('GetSortCapabilities()')
        return {'SortCapabilitiesResponse': {'SortCaps': ''}}

    def soap_GetSystemUpdateID(self, *args, **kwargs):
        """Required: Return the current value of state variable
        SystemUpdateID."""

        log.msg('GetSystemUpdateID()')
        return {'SystemUpdateIdResponse': {'Id': self.updateID}}

    BrowseFlags = ('BrowseMetaData', 'BrowseDirectChildren')

    def soap_Browse(self, *args):
        l = {}
        #debug.appendnamespace(reqname, l)
        if self.has_key(args[0]):
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

        didl = DIDLElement()
        result = {}

        # return error code if we don't exist anymore
        if ObjectID not in self:
            raise errorCode(701)

        # check to see if object needs to be updated
        self[ObjectID].checkUpdate()

        # make sure we still exist, we could of deleted ourself
        if ObjectID not in self:
            raise errorCode(701)

        if BrowseFlag == 'BrowseDirectChildren':
            ch = self.getchildren(ObjectID)[StartingIndex: StartingIndex + \
                                                            RequestedCount]
            for i in ch:
                if i.needupdate:
                    i.checkUpdate()
                didl.addItem(i)
            total = len(self.getchildren(ObjectID))
        else:
            didl.addItem(self[ObjectID])
            total = 1

        r = {'Result': didl.toString(), 'TotalMatches': total,
            'NumberReturned': didl.numItems(), }
        result = {'BrowseResponse': r}

        if hasattr(self[ObjectID], 'updateID'):
            r['UpdateID'] = self[ObjectID].updateID
        else:
            r['UpdateID'] = self.updateID

        return result

    # Optional actions

    def soap_Search(self, *args, **kwargs):
        """Search for objects that match some search criteria."""

        (ContainerID, SearchCriteria, Filter, StartingIndex,
         RequestedCount, SortCriteria) = args

        log.msg('Search(ContainerID=%s, SearchCriteria=%s, Filter=%s, ' \
            'StartingIndex=%s, RequestedCount=%s, SortCriteria=%s)' %
            (`ContainerID`, `SearchCriteria`, `Filter`,
            `StartingIndex`, `RequestedCount`, `SortCriteria`))

    def soap_CreateObject(self, *args, **kwargs):
        """Create a new object."""

        (ContainerID, Elements) = args

        log.msg('CreateObject(ContainerID=%s, Elements=%s)' %
            (`ContainerID`, `Elements`))

    def soap_DestroyObject(self, *args, **kwargs):
        """Destroy the specified object."""

        (ObjectID) = args

        log.msg('DestroyObject(ObjectID=%s)' % `ObjectID`)

    def soap_UpdateObject(self, *args, **kwargs):
        """Modify, delete or insert object metadata."""

        (ObjectID, CurrentTagValue, NewTagValue) = args

        log.msg('UpdateObject(ObjectID=%s, CurrentTagValue=%s, ' \
            'NewTagValue=%s)' % (`ObjectID`, `CurrentTagValue`,
            `NewTagValue`))

    def soap_ImportResource(self, *args, **kwargs):
        """Transfer a file from a remote source to a local
        destination in the Content Directory Service."""

        (SourceURI, DestinationURI) = args

        log.msg('ImportResource(SourceURI=%s, DestinationURI=%s)' %
            (`SourceURI`, `DestinationURI`))

    def soap_ExportResource(self, *args, **kwargs):
        """Transfer a file from a local source to a remote
        destination."""

        (SourceURI, DestinationURI) = args

        log.msg('ExportResource(SourceURI=%s, DestinationURI=%s)' %
            (`SourceURI`, `DestinationURI`))

    def soap_StopTransferResource(self, *args, **kwargs):
        """Stop a file transfer initiated by ImportResource or
        ExportResource."""

        (TransferID) = args

        log.msg('StopTransferResource(TransferID=%s)' % TransferID)

    def soap_GetTransferProgress(self, *args, **kwargs):
        """Query the progress of a file transfer initiated by
        an ImportResource or ExportResource action."""

        (TransferID, TransferStatus, TransferLength, TransferTotal) = args

        log.msg('GetTransferProgress(TransferID=%s, TransferStatus=%s, ' \
            'TransferLength=%s, TransferTotal=%s)' %
            (`TransferId`, `TransferStatus`, `TransferLength`,
            `TransferTotal`))

    def soap_DeleteResource(self, *args, **kwargs):
        """Delete a specified resource."""

        (ResourceURI) = args

        log.msg('DeleteResource(ResourceURI=%s)' % `ResourceURI`)

    def soap_CreateReference(self, *args, **kwargs):
        """Create a reference to an existing object."""

        (ContainerID, ObjectID) = args

        log.msg('CreateReference(ContainerID=%s, ObjectID=%s)' %
            (`ContainerID`, `ObjectID`))

    def __repr__(self):
        return '<ContentDirectoryControl: cnt: %d, urlbase: %s, nextID: %d>' \
                    % (len(self), `self.urlbase`, self.nextID)

    def get_actionDict(self):

        return None