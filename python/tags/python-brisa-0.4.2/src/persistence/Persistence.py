# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import brisa_config
from sqlobject import *
from sqlobject.inheritance import InheritableSQLObject

import sys
import os
from stat import ST_SIZE
from xml.etree.ElementTree import Element
from xml.etree.ElementTree import  SubElement
from xml.etree.ElementTree import tostring

__connection__ = brisa_config.get_parameter('persistence', 'connection')

_shared_url = ""

def set_shared_url(shared_url):
    _shared_url = shared_url

class Object(InheritableSQLObject, object):
    """The root class of the entire content directory class heirachy."""
    title = UnicodeCol(length=100)
    restricted = StringCol(length=50, default=None)
    className = StringCol(length=50, default=None)
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


class Container(Object, InheritableSQLObject):
#    class sqlmeta:
#        lazyUpdate = False
#        cacheValues = True

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

    def setParentId(self, parentId):
        self.parentID = parentId

    def toElement(self):
        root = Object.toElement(self)
        # only include if we have children, it's possible we don't
        # have our children yet, and childCount is optional.

        if len(self.containers)+len(self.items) > 0:
            root.attrib['childCount'] = str(len(self.containers) \
                                            +len(self.items))

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

    def setDuration(self, duration):
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
    copyright = UnicodeCol(length=100, default="")
    url = StringCol(length=255, default="")
    protocolInfo = StringCol(length=255)
    local_path = UnicodeCol(length=255, default="")
    rating = IntCol(default=0)
    description = UnicodeCol(length=100, default="")
    longDescription = UnicodeCol(default="")
    publisher = UnicodeCol(length=100, default="")
    producer = UnicodeCol(length=50, default="")
    protection = UnicodeCol(length=50, default="")
    contributor = UnicodeCol(length=50, default="")
    relation = UnicodeCol(length=50, default="")
    storageMedium = StringCol(length=50, default="")
    mediaType = StringCol(length=25)
    mediaLength = StringCol(length=25, default="")
    containers = RelatedJoin('Container')
    playLists = RelatedJoin('PlayList')
    klass = Object.klass + '.item'
    elementName = 'item'
    refID = None
    needupdate = True
    sizebytes = StringCol(length=32, default="")
    valid = StringCol(length=1, default="1")

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)
        if "kw" in kwargs and 'local_path' in kwargs['kw']:
            local_path = kwargs['kw']['local_path']
            if os.path.isfile(local_path):
                self.sizebytes = os.stat(local_path)[ST_SIZE]
            else:
                self.sizebytes = 0

    def toElement(self):
        if os.path.isfile(self.local_path):
            resource_url = _shared_url + "/" + str(self.id)
        else:
            resource_url = self.local_path
        self.res = (Resource(resource_url, self.protocolInfo))
        if self.mediaLength != "":
            self.res.setDuration(self.mediaLength)

        root = Object.toElement(self)

        if self.refID is not None:
            SubElement(root, 'refID').text = str(self.refID)

        return root


class DinamicItem(Item, InheritableSQLObject):
    language = UnicodeCol(length=50, default="")
    bitRate = StringCol(length=25, default="")

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class StaticItem(Item, InheritableSQLObject):
    author = UnicodeCol(length=50, default="")

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class BroadCastItem(Item, InheritableSQLObject):
    region = IntCol(default = 0)
    channelNr = IntCol(default = 0)
    bitRate = StringCol(length=25, default="")

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class VideoItem(Item):
    klass = Item.klass + '.videoItem'

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class AudioItem(Item):
    klass = Item.klass + '.audioItem'


class MusicTrack(DinamicItem):
    originalTrackNumber = IntCol(default=0)
    composer = UnicodeCol(length=100, default="")
    musicArtist = ForeignKey('MusicArtist', default=None)
    musicGenre = ForeignKey('MusicGenre', default=None)
    musicAlbum = RelatedJoin('MusicAlbum')
    klass = AudioItem.klass + '.musicTrack'

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)

    def toElement(self):
        root = DinamicItem.toElement(self)
        if self.musicGenre is not None:
            SubElement(root, 'upnp:genre').text = self.musicGenre.title

        if self.description is not None:
            SubElement(root, 'dc:description').text = self.description

        if self.longDescription is not None:
            SubElement(root, 'upnp:longDescription').text = \
                self.longDescription

        if self.publisher is not None:
            SubElement(root, 'dc:publisher').text = self.publisher

        if self.language is not None:
            SubElement(root, 'dc:language').text = self.language

        if self.relation is not None:
            SubElement(root, 'dc:relation').text = self.relation

        if self.copyright is not None:
            SubElement(root, 'dc:rights').text = self.copyright

        if self.musicArtist is not None:
            SubElement(root, 'upnp:artist').text = self.musicArtist.title

        if len(self.musicAlbum) is not 0:
            SubElement(root, 'upnp:album').text = self.musicAlbum[0].title

        if self.originalTrackNumber is not None:
            SubElement(root, 'upnp:originalTrackNumber').text = \
                str(self.originalTrackNumber)

        if self.playLists is not None:
            SubElement(root, 'upnp:playlist').text = self.playLists

        if self.storageMedium is not None:
            SubElement(root, 'upnp:storageMedium').text = self.storageMedium

        if self.contributor is not None:
            SubElement(root, 'dc:contributor').text = self.contributor

        if self.date is not None:
            SubElement(root, 'dc:date').text = str(self.date)
        return root


class Movie(DinamicItem):
    klass = VideoItem.klass + '.movie'
    DVDRegionCode = IntCol()
    channelName = UnicodeCol(length=50, default="")
    scheduledStartTime = DateTimeCol()
    scheduledEndTime = DateTimeCol()
    actor = UnicodeCol(length=50, default="")
    director = UnicodeCol(length=20, default="")
    movieGenre = ForeignKey('MovieGenre', default=None)

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class MusicVideoClip(DinamicItem):
    klass = VideoItem.klass + '.musicVideoClip'
    scheduledStartTime = DateTimeCol()
    scheduledEndTime = DateTimeCol()

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class PlayList(SQLObject):
    title = StringCol(length=100)
    items = RelatedJoin('Item')


class Text(SQLObject):
    staticItem = ForeignKey('StaticItem')

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class ImageItem(Item):
    klass = Item.klass + '.imageItem'


class Photo(StaticItem):
    klass = ImageItem.klass + '.photo'
    dateTaken = DateCol()
    photoalbums = RelatedJoin('PhotoAlbum')

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class VideoBroadcast(BroadCastItem):
    iconpath = StringCol(length=255)

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


class AudioBroadcast(BroadCastItem):
    klass = AudioItem.klass + '.audioBroadcast'
    radioCallSign = UnicodeCol(length=100, default=""),
    radioStationID = IntCol(),
    radioBand = UnicodeCol(length=50, default=""),

    def __init__(self, *args, **kwargs):
        SQLObject.__init__(self, *args, **kwargs)


def createDB():
    for table in (Object, Container, Item, MusicArtist, Genre, MusicGenre,
                  MovieGenre, DinamicItem, StaticItem, BroadCastItem,
                  VideoItem, Movie, MusicVideoClip, MusicTrack, Text,
                  Photo, VideoBroadcast, AudioBroadcast, MusicAlbum,
                  PhotoAlbum):
        table.createTable(ifNotExists=1)


Container.sqlmeta.addJoin(MultipleJoin('Container',
                                       joinMethodName='containers'))
MusicArtist.sqlmeta.addJoin(MultipleJoin('MusicTrack', joinMethodName='items'))
Genre.sqlmeta.addJoin(MultipleJoin('MusicTrack', joinMethodName='items'))


# ------- USAGE EXAMPLES
if __name__ == "__main__":
    pass
    createDB()

    genre = MusicGenre.get(14)
    print list(genre)
