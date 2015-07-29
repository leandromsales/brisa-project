# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php

# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006, John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007, Brisa Team

from core import Item, Person, Album
import sys
if sys.hexversion >= 0x20500f0:
    from xml.etree import ElementTree
    from xml.etree.ElementTree import Element, SubElement, tostring, _ElementInterface
else:
    from elementtree.ElementTree import Element, SubElement, tostring, _ElementInterface

class AudioItem(Item):
    """A piece of content that when rendered generates some audio."""

    klass = Item.klass + '.audioItem'

    genre = None
    description = None
    longDescription = None
    publisher = None
    language = None
    relation = None
    rights = None

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

    klass = AudioItem.klass + '.musicTrack'

    artist = None
    album = None
    originalTrackNumber = None
    playlist = None
    storageMedium = None
    contributor = None
    date = None

    def toElement(self):

        root = AudioItem.toElement(self)

        if self.artist is not None:
            SubElement(root, 'upnp:artist').text = self.artist

        if self.album is not None:
            SubElement(root, 'upnp:album').text = self.album

        if self.originalTrackNumber is not None:
            SubElement(root, 'upnp:originalTrackNumber').text = \
                self.originalTrackNumber

        if self.playlist is not None:
            SubElement(root, 'upnp:playlist').text = self.playlist

        if self.storageMedium is not None:
            SubElement(root, 'upnp:storageMedium').text = self.storageMedium

        if self.contributor is not None:
            SubElement(root, 'dc:contributor').text = self.contributor

        if self.date is not None:
            SubElement(root, 'dc:date').text = self.date

        return root

class AudioBroadcast(AudioItem):
    klass = AudioItem.klass + '.audioBroadcast'

class AudioBook(AudioItem):
    klass = AudioItem.klass + '.audioBook'

class MusicArtist(Person):
    klass = Person.klass + '.musicArtist'

class MusicAlbum(Album):
    klass = Album.klass + '.musicAlbum'
