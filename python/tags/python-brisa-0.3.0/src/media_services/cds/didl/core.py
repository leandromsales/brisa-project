# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php

# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006, John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007, Brisa Team

import sys
if sys.hexversion >= 0x20500f0:
    from xml.etree import ElementTree
    from xml.etree.ElementTree import Element, SubElement, tostring, _ElementInterface
else:
    from elementtree.ElementTree import Element, SubElement, tostring, _ElementInterface

import utils
from et import ET

class Resource(object):
    """An object representing a resource."""

    def __init__(self, data=None, protocolInfo=None):
        self.data = data
        self.protocolInfo = protocolInfo
        self.bitrate = None
        self.size = None

    def toElement(self):

        root = Element('res')
        root.attrib['protocolInfo'] = self.protocolInfo
        root.text = self.data

        if self.bitrate is not None:
            root.attrib['bitrate'] = str(self.bitrate)

        if self.size is not None:
            root.attrib['size'] = str(self.size)

        return root

    def fromElement(self, elt):
        self.protocolInfo = elt.attrib['protocolInfo']
        self.data = elt.text
        self.bitrate = elt.attrib.get('bitrate')
        self.size = elt.attrib.get('size')

    def toString(self):
        return ET.tostring(self.toElement())

    @classmethod
    def fromString(cls, aString):
        instance = cls()
        elt = utils.parse_xml(aString)
        #elt = ElementTree(elt)
        instance.fromElement(elt.getroot())
        return instance

class Object(object):
    """The root class of the entire content directory class heirachy."""

    klass = 'object'
    creator = None
    writeStatus = None
    #content = property(lambda x: x._content)
    #needupdate = None    # do we update before sending? (for res)
    date = None

    def __init__(self, cd=None, id=None, parentID=None, title=None, restricted = False,
        creator = None): #, **kwargs):

        self.res = []
        #self.cd = cd
        self.id = id
        self.parentID = parentID
        self.title = title
        self.creator = creator

        if restricted:
            self.restricted = 'true'
        else:
            self.restricted = 'false'

        #if kwargs.has_key('content'):
        #    self._content = kwargs['content']

#    def __lt__(self, other):
#        return self.__cmp__(other) < 0
#
#    def __le__(self, other):
#        return self.__cmp__(other) <= 0
#
#    def __eq__(self, other):
#        return self.__cmp__(other) == 0
#
#    def __ne__(self, other):
#        return self.__cmp__(other) != 0
#
#    def __gt__(self, other):
#        return self.__cmp__(other) > 0
#
#    def __ge__(self, other):
#        return self.__cmp__(other) >= 0
#
#    def __cmp__(self, other):
#        if not isinstance(other, self.__class__):
#            return 1
#        return cmp(self.id, other.id)

#    def __repr__(self):
#        cls = self.__class__
#        return '<%s.%s: id: %s, parent: %s, title: %s>' % \
#            (cls.__module__, cls.__name__, self.id, self.parentID,
#            self.title)

    def checkUpdate(self):
        return self

    def toElement(self):

        root = ET.Element(self.elementName)

        root.attrib['id'] = str(self.id)
        root.attrib['parentID'] = str(self.parentID)
        ET.SubElement(root, 'dc:title').text = self.title
        ET.SubElement(root, 'upnp:class').text = self.klass

        root.attrib['restricted'] = self.restricted

        if self.creator is not None:
            ET.SubElement(root, 'dc:creator').text = self.creator

        for res in self.res:
            root.append(res.toElement())

        if self.writeStatus is not None:
            ET.SubElement(root, 'upnp:writeStatus').text = self.writeStatus

        if self.date is not None:
            if isinstance(self.date, datetime):
                ET.SubElement(root, 'dc:date').text = self.date.isoformat()
            else:
                ET.SubElement(root, 'dc:date').text = self.date


        return root

    def toString(self):
        return ET.tostring(self.toElement())

    def fromElement(self, elt):
        """
        TODO:
         * creator
         * writeStatus
        """
        self.elementName = elt.tag
        self.id = elt.attrib['id']
        self.parentID = elt.attrib['parentID']
        self.restricted = elt.attrib['restricted']

        for child in elt.getchildren():
            if child.tag.endswith('title'):
                self.title = child.text
            elif child.tag.endswith('class'):
                self.klass = child.text
            elif child.tag.endswith('res'):
                res = Resource.fromString(ET.tostring(child))
                self.res.append(res)

    @classmethod
    def fromString(cls, data):
        instance = cls()
        elt = utils.parse_xml(data)
        #elt = ElementTree(elt)
        instance.fromElement(elt.getroot())
        return instance

class Item(Object):
    """A class used to represent atomic (non-container) content
    objects."""

    klass = Object.klass + '.item'
    elementName = 'item'
    refID = None
    #needupdate = True

#    def doUpdate(self):
#        # Update parent container
#        self.cd[self.parentID].doUpdate()

    def toElement(self):

        root = Object.toElement(self)

        if self.refID is not None:
            ET.SubElement(root, 'refID').text = self.refID

        return root

    def fromElement(self, elt):
        Object.fromElement(self, elt)
        for child in elt.getchildren():
            if child.tag.endswith('refID'):
                self.refID = child.text
                break

class Container(Object, list):
    """An object that can contain other objects."""

    klass = Object.klass + '.container'

    elementName = 'container'
    childCount = property(lambda x: len(x))
    createClass = None
    searchClass = None
    searchable = None
    updateID = 0
    needupdate = False
    childCount = 0

    def __init__(self, cd=None, id=None, parentID=None, title=None, restricted = 0,
        creator = None, **kwargs):
        Object.__init__(self, cd, id, parentID, title, restricted,
            creator, **kwargs)
        list.__init__(self)

    def doUpdate(self):
        self.updateID = (self.updateID + 1) % (1l << 32)

    def toElement(self):

        root = Object.toElement(self)

        root.attrib['childCount'] = str(self.childCount)

        if self.createClass is not None:
            ET.SubElement(root, 'upnp:createclass').text = self.createClass

        if not isinstance(self.searchClass, (list, tuple)):
            self.searchClass = ['searchClass']
        for i in self.searchClass:
            ET.SubElement(root, 'upnp:searchclass').text = i

        if self.searchable is not None:
            root.attrib['searchable'] = str(self.searchable)

        return root


    def fromElement(self, elt):
        Object.fromElement(self, elt)
        self.childCount = int(elt.attrib.get('childCount','0'))
        #self.searchable = int(elt.attrib.get('searchable','0'))
        self.searchable = elt.attrib.get('searchable','0') in ['True','true','1']
        self.searchClass = []
        for child in elt.getchildren():
            if child.tag.endswith('createclass'):
                self.createClass = child.text
            elif child.tag.endswith('searchClass'):
                self.searchClass.append(child.text)

    def __repr__(self):
        cls = self.__class__
        return '<%s.%s: id: %s, parent: %s, title: %s, cnt: %d>' % \
            (cls.__module__, cls.__name__, self.id, self.parentID,
            self.title, len(self))

class DIDLElement(_ElementInterface):
    def __init__(self):
        _ElementInterface.__init__(self, 'DIDL-Lite', {})
        self.attrib['xmlns'] = 'urn:schemas-upnp-org:metadata-1-0/DIDL-Lite'
        self.attrib['xmlns:dc'] = 'http://purl.org/dc/elements/1.1/'
        self.attrib['xmlns:upnp'] = 'urn:schemas-upnp-org:metadata-1-0/upnp'
        self._items = []

    def addContainer(self, id, parentID, title, restricted = False):
        e = Container(id, par
