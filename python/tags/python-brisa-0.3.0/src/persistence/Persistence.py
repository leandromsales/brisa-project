# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team

from config_handle import *
from sqlobject import *
from sqlobject.inheritance import InheritableSQLObject

import sys
if sys.hexversion >= 0x20500f0:
    from xml.etree.ElementTree import Element, SubElement, tostring, _ElementInterface
else:
    from elementtree.ElementTree import Element, SubElement, tostring, _ElementInterface

__connection__ = BrisaConf().get_parameter('persistence','connection')
#sqlhub.processConnection = connectionForURI(connection)

class Object(InheritableSQLObject, object):

    """The root class of the entire content directory class heirachy."""

    title = StringCol(length=40)
    restricted = StringCol(length=50, default=None)
    className = StringCol(length=50, default=None)
    # TODO: need to check if this foreign key field is really necessary
    #media = ForeignKey('Media')

    parentID = ""
    klass = 'object'
    creator = None
    res = None
    writeStatus = None
    content = property(lambda x: x._content)
    needupdate = None    # do we update before sending? (for res)
    restricted = "0"

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

    def setParentId(self, parentId):
        self.parentID = parentId

    def __repr__(self):
        cls = self.__class__
        return '<%s.%s: id: %s, parent: %s, title: %s>' % \
            (cls.__module__, cls.__name__, self.id, self.parentID,
            self.title)

    def toElement(self):

        root = Element(self.elementName)

        if self.id is 1:
            root.attrib['id'] = str(0)
        else:
            root.attrib['id'] = str(self.id)
        root.attrib['parentID'] = self.parentID
        SubElement(root, 'dc:title').text = self.title
        SubElement(root, 'upnp:class').text = self.klass

        root.attrib['restricted'] = str(self.restricted)

        if self.creator is not None:
            SubElement(root, 'dc:creator').text = self.creator

        if self.res is not None:
            try:
                for res in iter(self.res):
                    root.append(res.toElement())
            except TypeError:
                root.append(self.res.toElement())

        if self.writeStatus is not None:
            SubElement(root, 'upnp:writeStatus').text = self.writeStatus

        return root

    def toString(self):
        return tostring(self.toElement())
#class Item(Object):
#
#    def __init__(self, *args, **kwargs):
#        SQLObject.__init__(self, *args, **kwargs)

class Container(Object, InheritableSQLObject):

    items = RelatedJoin('Item')
    container = ForeignKey('Container', default=None)
    containers = MultipleJoin('Container')

    klass = Object.klass + '.container'

    elementName = 'container'
    childCount = property(lambda x: len(x))
    createClass = None
    searchClass = None
    searchable = None
    updateID = 0
    needupdate = False
    restricted = 0
    creator = None

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

    def setParentId(self,parentId):
        self.parentID = parentId

    def toElement(self):

        root = Object.toElement(self)

        # only include if we have children, it's possible we don't
        # have our children yet, and childCount is optional.

        if len(self.containers)+len(self.items) > 0:
            root.attrib['childCount'] = str(len(self.containers)+len(self.items))

        if self.createClass is not None:
            SubElement(root, 'upnp:createclass').text = self.createClass

        if self.searchClass is not None:
            if not isinstance(self.searchClass, (list, tuple)):
                self.searchClass = ['searchClass']
            for i in searchClass:
                SubElement(root, 'upnp:searchclass').text = i

        if self.searchable is not None:
            root.attrib['searchable'] = str(self.searchable)

        return root

    def __repr__(self):
        cls = self.__class__
        return '<%s.%s: id: %s, parent: %s, title: %s>' % \
            (cls.__module__, cls.__name__, self.id, self.parentID,
            self.title)


class Person(Container):
    klass = Container.klass + '.person'

class MusicArtist(Container):

    klass = Person.klass + '.musicArtist'
    items = MultipleJoin('MusicTrack')

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

class Genre(Container):

    klass = Container.klass + '.genre'
    items = MultipleJoin('MusicTrack')

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

class MusicGenre(Genre):

    klass = Genre.klass + '.musicGenre'
    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

class MovieGenre(Genre):

    klass = Genre.klass + '.movieGenre'
    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

class Album(Container):
    klass = Container.klass + '.album'

class MusicAlbum(Container):

    klass = Album.klass + '.musicAlbum'
    albumArtURI = StringCol(length=150, default=None)
    toc = StringCol(length=50, default=None)
    parentMusicAlbum = ForeignKey('MusicAlbum', default=None)
    #artist = ForeignKey('Artist')

    items = RelatedJoin('MusicTrack')

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

class PhotoAlbum(Container):

    klass = Album.klass + '.photoAlbum'
    parentPhotoAlbum = ForeignKey('PhotoAlbum', default=None)

    items = RelatedJoin('Photo')

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

class Resource(object):
    """An object representing a resource."""

    def __init__(self, data, protocolInfo):
        self.data = data
        self.protocolInfo = protocolInfo
        self.bitrate = None
        self.size = None
        self.duration = None

    def setDuration(self,duration):
        self.duration = duration

    def toElement(self):

        root = Element('res')
        root.attrib['protocolInfo'] = self.protocolInfo
        root.text = self.data

        if self.bitrate is not None:
            root.attrib['bitrate'] = str(self.bitrate)

        if self.size is not None:
            root.attrib['size'] = str(self.size)

        if self.duration is not None:
            root.attrib['duration'] = str(self.duration)

        return root

class Item(Object, InheritableSQLObject):

    date = DateCol(default="1900-01-01")
    copyright = StringCol(length=100, default="")
    url = StringCol(length=255, default="")
    protocolInfo = StringCol(length=255)
    path = StringCol(length=255)
    rating = IntCol(default=0)
    description = StringCol(length=100, default="")
    longDescription = StringCol(default="")
    publisher = StringCol(length=100, default="")
    producer = StringCol(length=50, default="")
    protection = StringCol(length=50, default="")
    contributor = StringCol(length=50, default="")
    relation = StringCol(length=50, default="")
    storageMedium = StringCol(length=50, default="")
    mediaType = StringCol(length=25)
    mediaLength = StringCol(length=25, default="")

    containers = RelatedJoin('Container')
    playLists = RelatedJoin('PlayList')

    klass = Object.klass + '.item'
    elementName = 'item'
    refID = None
    needupdate = True

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

    def toElement(self):
        if (self.protocolInfo and self.path) is not None:
            self.res = (Resource(self.path, self.protocolInfo))
            if self.mediaLength != "":
                self.res.setDuration(self.mediaLength)

        root = Object.toElement(self)

        if self.refID is not None:
            SubElement(root, 'refID').text = str(self.refID)

        return root


class DinamicItem(Item, InheritableSQLObject):

    language = StringCol(length=50, default="")
    bitRate = StringCol(length=25, default="")
#    genre = ForeignKey('Genre', default="")
#    artist = ForeignKey('Artist', default="")

    def __init__(self, *a
