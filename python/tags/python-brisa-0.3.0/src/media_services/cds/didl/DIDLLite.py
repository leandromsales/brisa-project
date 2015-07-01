# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php

# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006, Frank Scholz <coherence@beebits.net>
# Copyright 2007, Brisa Team <brisa-develop@garage.maemo.org>

"""
TODO:

- use more XPath expressions in fromElement() methods

"""
import os
import string
from datetime import datetime

my_namespaces = {'http://purl.org/dc/elements/1.1/' : 'dc',
                 'urn:schemas-upnp-org:metadata-1-0/upnp/': 'upnp'
                 }
from et import ET, namespace_map_update, ElementInterface
namespace_map_update(my_namespaces)

import utils

def classChooser(mimetype):

    if mimetype == 'root':
        return StorageFolder
    if mimetype == 'directory':
        return StorageFolder
    else:
        if string.find (mimetype,'image/') == 0:
            return Photo
        if string.find (mimetype,'audio/') == 0:
            return AudioItem
        if string.find (mimetype,'video/') == 0:
            return VideoItem
    return None


class Resource(object):
    """An object representing a resource."""

    def __init__(self, data=None, protocolInfo=None):
        self.data = data
        self.protocolInfo = protocolInfo
        self.bitrate = None
        self.size = None

    def toElement(self):

        root = ET.Element('res')
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

    upnp_class = 'object'
    creator = None
    #res = None
    writeStatus = None
    date = None

    def __init__(self, id=None, parentID=None, title=None, restricted = False,
                 creator = None):
        self.res = []
        self.id = id
        self.parentID = parentID
        self.title = title
        self.creator = creator

        if restricted:
            self.restricted = 'true'
        else:
            self.restricted = 'false'

    def checkUpdate(self):
        return self

    def toElement(self):

        root = ET.Element(self.elementName)

        root.attrib['id'] = str(self.id)
        root.attrib['parentID'] = str(self.parentID)
        ET.SubElement(root, 'dc:title').text = self.title
        ET.SubElement(root, 'upnp:class').text = self.upnp_class

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
                self.upnp_class = child.text
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

    upnp_class = Object.upnp_class + '.item'
    elementName = 'item'
    refID = None

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


class ImageItem(Item):
    upnp_class = Item.upnp_class + '.imageItem'

    description = None
    longDescription = None
    rating = None
    storageMedium = None
    publisher = None
    rights = None

    def toElement(self):
        root = Item.toElement(self)
        if self.description is not None:
            ET.SubElement(root, 'dc:description').text = self.description

        if self.longDescription is not None:
            ET.SubElement(root, 'upnp:longDescription').text = self.longDescription

        if self.rating is not None:
            ET.SubElement(root, 'upnp:rating').text = self.rating

        if self.storageMedium is not None:
            ET.SubElement(root, 'upnp:storageMedium').text = self.storageMedium

        if self.publisher is not None:
            ET.SubElement(root, 'dc:publisher').text = self.contributor

        if self.rights is not None:
            ET.SubElement(root, 'dc:rights').text = self.rights

        return root

class Photo(ImageItem):
    upnp_class = ImageItem.upnp_class + '.photo'
    album = None

    def toElement(self):
        root = ImageItem.toElement(self)
        if self.album is not None:
            ET.SubElement(root, 'upnp:album').text = self.album
        return root

class AudioItem(Item):
    """A piece of content that when rendered generates some audio."""

    upnp_class = Item.upnp_class + '.audioItem'

    genre = None
    description = None
    longDescription = None
    publisher = None
    language = None
    relation = None
    rights = None

    valid_keys = ['genre', 'description', 'longDescription', 'publisher',
                  'langugage', 'relation', 'rights']

    def toElement(self):

        root = Item.toElement(self)

        if self.genre is not None:
            ET.SubElement(root, 'upnp:genre').text = self.genre

        if self.description is not None:
            ET.SubElement(root, 'dc:description').text = self.description

        if self.longDescription is not None:
            ET.SubElement(root, 'upnp:longDescription').text = \
                             self.longDescription

        if self.publisher is not None:
            ET.SubElement(root, 'dc:publisher').text = self.publisher

        if self.language is not None:
            ET.SubElement(root, 'dc:language').text = self.language

        if self.relation is not None:
            ET.SubElement(root, 'dc:relation').text = self.relation

        if self.rights is not None:
            ET.SubElement(root, 'dc:rights').text = self.rights

        return root

    def fromElement(self, elt):
        Item.fromElement(self, elt)
        for child in elt.getchildren():
            tag = child.tag
            val = child.text
            if tag in self.valid_keys:
                setattr(self, tag, val)


class MusicTrack(AudioItem):
    """A discrete piece of audio that should be interpreted as music."""

    upnp_class = AudioItem.upnp_class + '.musicTrack'

    artist = None
    album = None
    originalTrackNumber = None
    playlist = None
    storageMedium =
