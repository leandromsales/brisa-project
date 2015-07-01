# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" DIDL-Lite classes (object, items, containers and etc).
"""

from xml.etree.ElementTree import _ElementInterface
from xml.etree import ElementTree

from brisa.upnp.didl import dlna
from brisa.utils.network import parse_xml
from brisa.utils import properties
from brisa.services.web_server import WebServer
from brisa import log


class Resource(object):
    """ Represents a resource. Used for generating the DIDL XML messages.
    """

    def __init__(self, uri='', protocolInfo='', bitrate=None, sizebytes=None,
                 duration=None, samplerate=None, resolution=None):
        self.uri = uri
        self.protocolInfo = protocolInfo
        self.bitrate = bitrate
        self.sizebytes = sizebytes
        self.duration = duration
        self.samplerate = samplerate
        self.resolution = resolution

    def from_element(self, elt):
        self.protocolInfo = elt.attrib['protocolInfo']
        self.uri = elt.text
        self.bitrate = elt.attrib.get('bitrate')
        self.size = elt.attrib.get('size')

    def to_string(self):
        return ElementTree.tostring(self.to_element())

    def set_duration(self, duration):
        self.duration = duration

    @classmethod
    def from_string(cls, aString):
        instance = cls()
        elt = parse_xml(aString)
        instance.from_element(elt.getroot())
        return instance

    def to_didl_element(self):
        root = ElementTree.Element('res')
        root.attrib['protocolInfo'] = self.protocolInfo
        root.text = self.uri

        if self.bitrate is not None and self.bitrate != "":
            root.attrib['bitrate'] = str(self.bitrate)

        if self.sizebytes is not None and self.sizebytes != "":
            root.attrib['size'] = str(self.sizebytes)

        if self.duration is not None and self.duration != "":
            root.attrib['duration'] = str(self.duration)

        if self.samplerate is not None and self.samplerate != "":
            root.attrib['sampleFrequency'] = self.samplerate

        if self.resolution is not None and self.resolution != "":
            root.attrib['resolution'] = self.resolution

        return root


class Object(object):
    """ Root class and most basic class of the content directory class
    hierachy.
    """

    upnp_class = 'object'

    def _get_uri(self):
        try:
            return self.resources[0].uri
        except:
            return ''

    uri = property(fget=_get_uri)

    def _gen_uri(self):
        if ':' in str(self.id):
            namespace, id = self.id.split(':')
            return "%s/%s/%s" % (WebServer().listen_url, namespace, id)
        else:
            return "%s/%s/%s" % (WebServer().listen_url, self.namespace,
                                 self.id)

    def __init__(self, id='0', parent_container_id=0, title='',
                 namespace='', type='',
                 restricted=False, creator=None, *args, **kwargs):
        self.resources = []

        try:
            if ':' in id:
                namespace, id = id.split(':')
        except:
            pass

        self.id = id
        self.namespace = namespace
        self.creator = creator
        self.restricted = restricted
        self.writeStatus = None
        self.parent_container_id = parent_container_id
        self.title = title
        self.type = type

    def check_update(self):
        return self

    def from_element(self, elt):
        self.elementName = elt.tag
        self.id = elt.attrib['id']
        self.parent_container_id = elt.attrib['parentID']
        self.restricted = elt.attrib['restricted']

        for child in elt.getchildren():
            if child.tag.endswith('title'):
                self.title = child.text
            elif child.tag.endswith('class'):
                self.upnp_class = child.text
            elif child.tag.endswith('res'):
                resource = Resource.from_string(ElementTree.tostring(child))
                self.resources.append(resource)

    @classmethod
    def from_string(cls, data):
        instance = cls()
        elt = parse_xml(data)
        instance.from_element(elt.getroot())
        return instance

    def to_didl_element(self):
        root = ElementTree.Element(self.elementName)
        if self.namespace:
            root.attrib['id'] = '%s' % self.object_id
            root.attrib['parentID'] = '%s:%s' % (self.namespace,\
                                                 str(self.parent_container_id))
        else:
            root.attrib['id'] = '%s' % str(self.id)
            root.attrib['parentID'] = '%s' % str(self.parent_container_id)

        ElementTree.SubElement(root, 'dc:title').text = self.title
        ElementTree.SubElement(root, 'upnp:class').text = self.upnp_class

        if self.restricted:
            root.attrib['restricted'] = 'true'
        else:
            root.attrib['restricted'] = 'false'

        if self.creator is not None:
            ElementTree.SubElement(root, 'dc:creator').text = self.creator

        if self.writeStatus is not None:
            ElementTree.SubElement(root, 'upnp:writeStatus').text = \
                self.writeStatus

        return root

    def to_string(self):
        return ElementTree.tostring(self.to_didl_element())

    def _get_object_id(self):
        return '%s:%s' % (str(self.namespace), str(self.id))

    def _set_object_id(self, s):
        self.namespace, self.id = s.split(':')

    object_id = property(_get_object_id, _set_object_id)


class Container(Object):
    """ An object that can contain other objects.
    """

    upnp_class = '%s%s' % (Object.upnp_class, '.container')

    elementName = 'container'
    createClass = None
    searchable = None
    searchClasses = []
    _childCount = 0

    def __init__(self, id='0', parent_container_id='0', type='',
                 title='', namespace='', parent_container=None,
                 local_path='', restricted=False, creator=None, **args):
        Object.__init__(self, id, parent_container_id, title, namespace, type,
                        restricted, creator)
        self.parent_container = parent_container
        self.containers = []
        self.items = []
        self.local_path = local_path

    def _get_childCount(self):
        return len(self.containers) + len(self.items)

    def _set_childCount(self, count):
        self._childCount = count

    childCount = property(_get_childCount, _set_childCount)

    def from_element(self, elt):
        Object.from_element(self, elt)
        self.childCount = int(elt.attrib.get('childCount', '0'))
        self.searchable = elt.attrib.get('searchable', '0') in \
                                    ['True', 'true', '1']
        self.searchClass = []
        append = self.searchClass.append
        for child in elt.getchildren():
            if child.tag.endswith('createclass'):
                self.createClass = child.text
            elif child.tag.endswith('searchClass'):
                append(child.text)

    def to_didl_element(self):
        root = Object.to_didl_element(self)

        root.attrib['childCount'] = str(self.childCount)

        if self.createClass:
            ElementTree.SubElement(root, 'upnp:createClass').text = \
                self.createClass

        if not isinstance(self.searchClasses, (list, tuple)):
            self.searchClasses = ['searchClass']

        SubElement = ElementTree.SubElement
        for searchClass in self.searchClasses:
            SubElement(root, 'upnp:searchClass').text = searchClass

        if self.searchable != "":
            root.attrib['searchable'] = str(self.searchable)

        return root

    def add_item(self, c):
        if not c in self.items:
            self.items.append(c)


class Item(Object):
    """ A class used to represent atomic (non-container) content
    objects.
    """

    upnp_class = '%s%s' % (Object.upnp_class, '.item')
    elementName = 'item'

    def __init__(self, id='0', date='1900-01-01', copyright='',
                 protocolInfo='', local_path='', rating=0, description='',
                 longDescription='', publisher='', producer='', author='',
                 protection='', contributor='', relation='', storage_medium='',
                 media_type='', duration='', sizebytes='', valid='1',
                 language='', bitrate='', rights='', genre=None, root='',
                 samplerate='', resolution='', type='', refID=None,
                 parent_container_id='0', title='', namespace='',
                 restricted=False, creator=None, **args):
        Object.__init__(self, id, parent_container_id, title, namespace, type,
                        restricted, creator)
        self.date = date
        self.copyright = copyright
        self.protocolInfo = protocolInfo
        self.local_path = local_path
        self.rating = rating
        self.description = description
        self.longDescription = longDescription
        self.publisher = publisher
        self.producer = producer
        self.author = author
        self.protection = protection
        self.contributor = contributor
        self.relation = relation
        self.storage_medium = storage_medium
        self.media_type = media_type
        self.duration = duration
        self.sizebytes = sizebytes
        self.valid = valid
        self.language = language
        self.bitrate = bitrate
        self.rights = rights
        self.genre = genre
        self.root = root
        self.samplerate = samplerate
        self.resolution = resolution
        self.type = type
        self.refID = refID

    def from_element(self, elt):
        Object.from_element(self, elt)
        for child in elt.getchildren():
            if child.tag.endswith('refID'):
                self.refID = child.text
                break

    def to_didl_element(self):

        root = Object.to_didl_element(self)

        if self.uri:
            self.resources.append(Resource(self.uri, self.protocolInfo,
                                           self.bitrate, self.sizebytes,
                                           self.duration, self.samplerate,
                                           self.resolution))
        else:
            self.resources.append(Resource(self._gen_uri(), self.protocolInfo,
                                           self.bitrate, self.sizebytes,
                                           self.duration, self.samplerate,
                                           self.resolution))

        append = root.append
        [append(resource.to_didl_element()) for resource in self.resources]

        if self.description is not None and self.description != "":
            ElementTree.SubElement(root, 'dc:description').text = \
                self.description

        if self.longDescription is not None and self.longDescription != "":
            ElementTree.SubElement(root, 'upnp:longDescription').text = \
                self.longDescription

        if self.copyright is not None and self.copyright != "":
            ElementTree.SubElement(root, 'dc:copyright').text = self.copyright

        if self.publisher is not None and self.publisher != "":
            ElementTree.SubElement(root, 'dc:publisher').text = self.publisher

        if self.producer is not None and self.producer != "":
            ElementTree.SubElement(root, 'dc:producer').text = self.producer

        if self.contributor is not None and self.contributor != "":
            ElementTree.SubElement(root, 'dc:contributor').text = \
                                                              self.contributor
        if self.language is not None and self.language != "":
            ElementTree.SubElement(root, 'dc:language').text = self.language

        if self.relation is not None and self.relation != "":
            ElementTree.SubElement(root, 'dc:relation').text = self.relation

        if self.rights is not None and self.rights != "":
            ElementTree.SubElement(root, 'dc:rights').text = self.rights

        if self.author is not None and self.author != "":
            element = ElementTree.SubElement(root, 'upnp:author')
            element.attrib['role'] = 'Composer'
            element.text = self.author

        if self.date is not None and self.date != "":
            ElementTree.SubElement(root, 'upnp:date').text = str(self.date)

        if self.rating is not None and self.rating != "":
            ElementTree.SubElement(root, 'upnp:rating').text = str(self.rating)

        if self.refID is not None and self.refID != "":
            ElementTree.SubElement(root, 'refID').text = self.refID

        if self.storage_medium is not None and self.storage_medium != "":
            ElementTree.SubElement(root, 'upnp:storage_medium').text = \
                self.storage_medium

        return root


class AudioItem(Item):
    """ A piece of content that when rendered generates audio.
    """

    upnp_class = '%s%s' % (Item.upnp_class, '.audioItem')

    valid_keys = ['genre', 'description', 'longDescription', 'publisher',
                  'language', 'relation', 'rights', 'artist', 'album']

    def __init__(self, id='0', region=0, channel_nr=0, name='',
                 date='1900-01-01', copyright='', protocolInfo='',
                 local_path='', rating=0, description='', longDescription='',
                 publisher='', producer='', author='', protection='',
                 contributor='', relation='', storage_medium='',
                 media_type='', duration='', sizebytes='', valid='1',
                 language='', bitrate='', rights='', genre=None, root='',
                 samplerate='', resolution='', type='DBAudioItem', refID=None,
                 parent_container_id='0', title='', namespace='',
                 restricted=False, creator=None, **args):
        Item.__init__(self, id, date, copyright, protocolInfo, local_path,
                      rating, description, longDescription, publisher,
                      producer, author, protection, contributor, relation,
                      storage_medium, media_type, duration, sizebytes,
                      valid, language, bitrate, rights, genre, root,
                      samplerate, resolution, type, refID, parent_container_id,
                      title, namespace, restricted, creator)
        self.region = region
        self.channel_nr = channel_nr
        self.name = name

    def from_element(self, elt):
        Item.from_element(self, elt)
        for child in elt.getchildren():
            tag = child.tag[child.tag.find('}')+1:]
            val = child.text
            if tag in AudioItem.valid_keys:
                setattr(self, tag, val)


class MusicTrack(AudioItem):
    """ A discrete piece of audio that should be interpreted as music.
    """

    upnp_class = '%s%s' % (AudioItem.upnp_class, '.musicTrack')

    valid_keys = ['original_track_number', 'playlist', 'storage_medium',
                  'contributor', 'ranking']

    def __init__(self, id='0', original_track_number=0, composer='',
                 artist=None, album='', name='', region=0,
                 date='1900-01-01', copyright='', protocolInfo='',
                 local_path='', rating=0, description='', longDescription='',
                 publisher='', producer='', author='', protection='',
                 contributor='', relation='', storage_medium='',
                 media_type='', duration='', sizebytes='', valid='1',
                 language='', bitrate='', rights='', genre=None, root='',
                 samplerate='', resolution='', type='DBMusicTrack', refID=None,
                 parent_container_id='0', title='', namespace='',
                 restricted=False, creator=None, channel_nr=0, ranking=None,
                 playlist=None, **args):
        AudioItem.__init__(self, id, region, channel_nr, name, date, copyright,
                           protocolInfo, local_path, rating, description,
                           longDescription, publisher, producer, author,
                           protection, contributor, relation, storage_medium,
                           media_type, duration, sizebytes, valid, language,
                           bitrate, rights, genre, root, samplerate,
                           resolution, type, refID, parent_container_id, title,
                           namespace, restricted, creator)
        self.original_track_number = original_track_number
        self.composer = composer
        self.ranking = ranking
        self.playlist = playlist
        self.album = album
        self.artist = artist

    def to_didl_element(self):
        root = AudioItem.to_didl_element(self)

        if self.album is not None and self.album != "":
            ElementTree.SubElement(root, 'upnp:album').text = str(self.album)

        if self.original_track_number is not None and \
           self.original_track_number > 0:
            ElementTree.SubElement(root, 'upnp:originalTrackNumber').text = \
                str(self.original_track_number)

        if self.genre is not None and self.genre != "":
            ElementTree.SubElement(root, 'upnp:genre').text = self.genre

        if self.artist is not None and self.artist != "":
            element = ElementTree.SubElement(root, 'upnp:artist')
            element.attrib['role'] = 'AlbumArtist'
            element.text = str(self.artist)

        return root


class Genre(Container):
    """ A container with a name denoting a genre.
    """
    upnp_class = '%s%s' % (Container.upnp_class, '.genre')

    def __init__(self, id='0', name='', parent_container_id='0',
                 type='DBContainer', title='', namespace='',
                 parent_container=None, local_path='',
                 restricted=False, creator=None, **args):
        Container.__init__(self, id, parent_container_id, type, title,
                           namespace, parent_container, local_path, restricted,
                           creator)
        self.name = name


class Person(Container):
    """ A container with a name denoting a person.
    """
    upnp_class = '%s%s' % (Container.upnp_class, '.person')

    def __init__(self, id='0', name='', parent_container_id='0',
                 type='DBContainer', title='', namespace='', local_path='',
                 parent_container=None, restricted=False, creator=None,
                 **args):
        Container.__init__(self, id, parent_container_id, type, title,
                           namespace, parent_container, local_path, restricted,
                           creator)
        self.name = name


class MusicArtist(Person):
    """ Music artist representation.
    """
    upnp_class = '%s%s' % (Person.upnp_class, '.musicArtist')


class MusicGenre(Genre):
    """ Music genre representation.
    """
    upnp_class = '%s%s' % (Genre.upnp_class, '.musicGenre')


class MovieGenre(Genre):
    """ Movie genre representation.
    """
    upnp_class = '%s%s' % (Genre.upnp_class, '.movieGenre')


class Album(Container):
    """ A container with a name denoting an album. 
    """
    upnp_class = '%s%s' % (Container.upnp_class, '.album')

    def __init__(self, id='0', name='', parent_container_id='0',
                 type='DBContainer', title='', namespace='', local_path='',
                 parent_container=None, restricted=False, creator=None,
                 **args):
        Container.__init__(self, id, parent_container_id, type, name,
                           namespace, parent_container, local_path, restricted,
                           creator)
        self.name = name


class MusicAlbum(Album):
    """ A music album representation.
    """
    upnp_class = '%s%s' % (Album.upnp_class, '.musicAlbum')

    def __init__(self, id='0', name='', album_art_uri='', toc='',
                 parent_container_id='0', type='DBContainer', title='',
                 namespace='', local_path='', parent_container=None,
                 restricted=False, creator=None,
                 **args):
        Album.__init__(self, id, name, parent_container_id, type, title,
                       namespace, local_path, parent_container, restricted,
                       creator)
        self.name = name
        self.album_art_uri = album_art_uri
        self.toc = toc


class PhotoAlbum(Album):
    """ A photo album representation.
    """
    upnp_class = '%s%s' % (Album.upnp_class, '.photoAlbum')


class VideoAlbum(Album):
    """ A video album representation.
    """
    upnp_class = '%s%s' % (Album.upnp_class, '.videoAlbum')


class AudioBroadcast(AudioItem):
    """ A remote audio item.
    """
    upnp_class = '%s%s' % (AudioItem.upnp_class, '.audioBroadcast')

    def to_didl_element(self):
        root = Item.to_didl_element(self)

        if self.region is not None and self.region != "":
            ElementTree.SubElement(root, 'upnp:region').text = self.region
        if self.channel_nr is not None and self.channel_nr != "":
            ElementTree.SubElement(root, 'upnp:channel_nr').text = \
                self.channel_nr
        return root

    def from_element(self, elt):
        DBItem.from_element(self, elt)
        for child in elt.getchildren():
            tag = child.tag
            val = child.text
            if tag in self.valid_keys:
                setattr(self, tag, val)


class AudioBook(AudioItem):
    """ Audio book item.
    """
    upnp_class = '%s%s' % (AudioItem.upnp_class, '.audioBook')


class VideoItem(Item):
    """ A video representation.
    """
    upnp_class = '%s%s' % (Item.upnp_class, '.videoItem')

    def __init__(self, id='0', region=0, channel_nr=0,
                 date='1900-01-01', copyright='', protocolInfo='',
                 local_path='', rating=0, description='', longDescription='',
                 publisher='', producer='', author='', protection='',
                 contributor='', relation='', storage_medium='',
                 media_type='', duration='', sizebytes='', valid='1',
                 language='', bitrate='', rights='', genre=None, root='',
                 samplerate='', resolution='', type='', refID=None,
                 parent_container_id='0', title='', namespace='',
                 restricted=False, creator=None, name='', **args):
        Item.__init__(self, id, date, copyright, protocolInfo, local_path,
                      rating, description, longDescription, publisher,
                      producer, author, protection, contributor, relation,
                      storage_medium, media_type, duration, sizebytes, valid,
                      language, bitrate, rights, genre, root, samplerate,
                      resolution, type, refID, parent_container_id, title,
                      namespace, restricted, creator)
        self.name = name


class Movie(VideoItem):
    """ A movie representation.
    """
    upnp_class = '%s%s' % (VideoItem.upnp_class, '.movie')

    def __init__(self, id='0', dvd_region_code=1, channel_name='',
                 scheduled_start_time=None, scheduled_end_time=None, actor='',
                 director='', name='', date='1900-01-01', copyright='',
                 protocolInfo='', local_path='', rating=0, description='',
                 longDescription='', publisher='', producer='', author='',
                 protection='', contributor='', relation='', storage_medium='',
                 media_type='', duration='', sizebytes='', valid='1',
                 language='', bitrate='', rights='', genre=None, root='',
                 samplerate='', resolution='', refID=None,
                 parent_container_id='0', title='', namespace='',
                 restricted=False, creator=None, type='', **args):
        VideoItem.__init__(self, id, region, channel_nr, name, date, copyright,
                                       protocolInfo, local_path, rating,
                                       description, longDescription, publisher,
                                       producer, author, protection,
                                       contributor, relation, storage_medium,
                                       media_type, duration, sizebytes, valid,
                                       language, bitrate, rights, genre, root,
                                       samplerate, resolution, type, refID,
                                       parent_container_id, title, namespace,
                                       restricted, creator)
        self.dvd_region_code = dvd_region_code
        self.channel_name = channel_name
        self.scheduled_start_time = scheduled_start_time
        self.scheduled_end_time = scheduled_end_time
        self.actor = actor
        self.director = director

    def to_didl_element(self):
        root = VideoItem.to_didl_element(self)

        if self.DVDRegionCode is not None and self.DVDRegionCode != "":
            ElementTree.SubElement(root, 'upnp:dvdregioncode').text = \
                self.DVDRegionCode

        if self.channelName is not None and self.channelName != "":
            ElementTree.SubElement(root, 'upnp:channelname').text = \
                self.channelName

        if self.scheduled_start_time is not None and \
           self.scheduled_start_time != "":
            ElementTree.SubElement(root, 'upnp:scheduled_start_time').text = \
                self.scheduled_start_time
        if self.scheduled_end_time is not None and \
           self.scheduled_end_time != "":
            ElementTree.SubElement(root, 'upnp:scheduled_end_time').text = \
                self.scheduled_end_time
        if self.actor is not None and self.actor != "":
            ElementTree.SubElement(root, 'upnp:actor').text = self.actor
        if self.director is not None and self.director != "":
            ElementTree.SubElement(root, 'upnp:director').text = self.director

        return root


class VideoBroadcast(VideoItem):
    """ A remove video representation.
    """
    upnp_class = '%s%s' % (VideoItem.upnp_class, '.videoBroadcast')


class MusicVideoClip(VideoItem):
    """ A music video clip representation.
    """
    upnp_class = '%s%s' % (VideoItem.upnp_class, '.musicVideoClip')

    def __init__(self, id='0', scheduled_start_time=None,
                 scheduled_end_time=None, name='', date='1900-01-01',
                 copyright='', protocolInfo='', local_path='', rating=0,
                 description='', longDescription='', publisher='', producer='',
                 author='', protection='', contributor='', relation='',
                 storage_medium='', media_type='', duration='', sizebytes='',
                 valid='1', language='', bitrate='', rights='', genre=None,
                 root='', samplerate='', resolution='', refID=None,
                 parent_container_id='0', title='', namespace='',
                 restricted=False, creator=None, type='', **args):
        VideoItem.__init__(self, id, date, copyright, protocolInfo,
                           local_path, rating, description, longDescription,
                           publisher, producer, author,
                           protection, contributor,
                           relation, storage_medium, media_type, duration,
                           sizebytes, valid, language, bitrate, rights,
                           genre, root, samplerate, resolution, type, refID,
                           parent_container_id, title, namespace, restricted,
                           creator)
        self.scheduled_start_time = scheduled_start_time
        self.scheduled_end_time = scheduled_end_time

    def to_didl_element(self):
        root = VideoItem.to_didl_element(self)

        if self.scheduled_start_time is not "":
            ElementTree.SubElement(root, 'upnp:scheduled_start_time').text = \
                self.scheduled_start_time
        if self.scheduled_end_time is not "":
            ElementTree.SubElement(root, 'upnp:scheduled_end_time').text = \
                self.scheduled_end_time

        return root


class ImageItem(Item):
    """ An image representation.
    """
    upnp_class = '%s%s' % (Item.upnp_class, '.imageItem')


class Photo(ImageItem):
    """ A photo representation.
    """
    upnp_class = '%s%s' % (ImageItem.upnp_class, '.photo')

    def __init__(self, id='0', date='1900-01-01', copyright='',
                 protocolInfo='', local_path='', rating=0, description='',
                 longDescription='', publisher='', producer='', author='',
                 protection='', contributor='', relation='', storage_medium='',
                 media_type='', duration='', sizebytes='', valid='1',
                 language='', bitrate='', rights='', genre=None, root='',
                 samplerate='', resolution='', type='', refID=None,
                 parent_container_id='0', title='', namespace='',
                 restricted=False, creator=None, album='', name='', **args):
        ImageItem.__init__(self, id, date, copyright, protocolInfo, local_path,
                           rating, description, longDescription, publisher,
                           producer, author, protection, contributor, relation,
                           storage_medium, media_type, duration, sizebytes,
                           valid, language, bitrate, rights, genre, root,
                           samplerate, resolution, type, refID,
                           parent_container_id, title, namespace, restricted,
                           creator)
        self.album = album
        self.name = name

    def to_didl_element(self):
        root = ImageItem.to_didl_element(self)

        if self.album is not "":
            ElementTree.SubElement(root, 'upnp:album').text = self.album

        return root


class PlaylistItem(Item):
    """ A playlist item represesentation.
    """
    upnp_class = '%s%s' % (Item.upnp_class, '.playlistItem')


class PlaylistContainer(Container):
    """ A playlist representation.
    """
    upnp_class = '%s%s' % (Container.upnp_class, '.playlistContainer')


class TextItem(Item):
    """ A text representation.
    """
    upnp_class = '%s%s' % (Item.upnp_class, '.textItem')


class StorageSystem(Container):
    """ A storage system representation.
    """
    upnp_class = '%s%s' % (Container.upnp_class, '.storageSystem')


class StorageVolume(Container):
    """ A storage volume representation.
    """
    upnp_class = '%s%s' % (Container.upnp_class, '.storageVolume')


class StorageFolder(Container):
    """ A storage folder representation.
    """
    upnp_class = '%s%s' % (Container.upnp_class, '.storageFolder')


class Element(_ElementInterface):
    """ Wrapper for elements. Can mount a complete tree of DIDL UPnP classes
    from a string and also mount the string from a complete tree.
    """

    def __init__(self):
        _ElementInterface.__init__(self, 'DIDL-Lite', {})
        self.attrib['xmlns'] = 'urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/'
        self.attrib['xmlns:dc'] = 'http://purl.org/dc/elements/1.1/'
        self.attrib['xmlns:upnp'] = 'urn:schemas-upnp-org:metadata-1-0/upnp/'
        self.attrib['xmlns:dlna'] = 'urn:schemas-dlna-org:metadata-1-0'
        self._items = []

    def add_container(self, id, parent_container_id, title, restricted=False):
        e = Container(id, parent_container_id, title, restricted, creator='')
        self.append(e.to_element())

    def add_item(self, item):
        self.append(item.to_didl_element())
        self._items.append(item)

    def num_items(self):
        return len(self)

    def get_items(self):
        return self._items

    def to_string(self):
        return ElementTree.tostring(self)

    @classmethod
    def from_string(cls, aString):
        instance = cls()
        elt = parse_xml(aString, 'utf-8')
        elt = elt.getroot()
        add_item = instance.add_item

        for node in elt.getchildren():
            upnp_class_name = node.find(
                ".//{urn:schemas-upnp-org:metadata-1-0/upnp/}class").text
            names = upnp_class_name.split('.')
            while names:
                class_name = names[-1]
                class_name = "%s%s" % (class_name[0].upper(), class_name[1:])
                try:
                    upnp_class = eval(class_name)
                    new_node = \
                        upnp_class.from_string(ElementTree.tostring(node))
                    add_item(new_node)
                    break
                except Exception, e:
                    names = names[:-1]
                    log.debug('element from string critical bug: %s' % str(e))
                    continue

        return instance
