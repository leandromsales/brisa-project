# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

import shutil

from os import path

from brisa.upnp.didl.didl_lite import ImageItem, Container, MusicTrack, Movie
from brisa.services.web_server import WebServer
from brisa.utils.database import resolve_database
from brisa.utils.sqlite import SQLWorkerThread, SQLQueueManager
from brisa import config, log


dbpath = config.get_parameter('media_server_plugin-maemo-multimedia',
                              'database_location')
log = log.getLogger('maemo Multimedia')

queue_manager = SQLQueueManager()
worker_thread = SQLWorkerThread(dbpath)
worker_thread.set_sql_queue_manager(queue_manager)
worker_thread.start()


def get_uri(id, dir):
    return "%s%s%s" % (WebServer().listen_url, dir, id)


def create_database():
    try:
        queue_manager.put_and_join(('ALTER TABLE metadata add id int(11)', ()))
    except:
        pass

    queue_manager.put_and_join(('select filename from metadata', ()))
    index = 1
    k = queue_manager.task_result
    for row in k:
        filename = unicode(row[0])
        sql = 'UPDATE metadata set id=? WHERE filename=?'
        queue_manager.put_and_join((sql, (index, filename)))
        index += 1


def select_items(mime):
    """Select a audio list from DB."""
    try:
        object = MetadataItemsList(mime)
        return object.didl_list()
    except Exception, e:
        log.debug("Select metadata_list error: %s" % e)
        return []


def select_metadata_item(id):
    try:
        item = MetadataItems(id)
        return [item.didlobject]
    except:
        log.debug("Select metadata_item error: %s" % e)
        return []


def select_url_item_by_id(objectId):
    """Select the url from audio identify by Id."""
    try:
        object = MetadataItemsList()
        return object.url_item(objectId.split('/')[2])
    except Exception, e:
        log.debug("Select url item error: %s" % e)
        return []


def select_item(id):
    queue_manager.put_and_join(('SELECT MIME FROM metadata WHERE id=?',
                                (unicode(id), )))

    k = queue_manager.task_result
    if 'audio' or 'video' or 'image' in mime:
        return select_metadata_item(id)
    else:
        return []


class MetadataItemsList(object):
    allowed_mimes = {'audio': ['audio/x-mp3', 'audio/x-m4a', 'audio/x-ms-wma'],
                     'video': ['video/avi', 'video/mpeg', 'video/x-mp4',
                               'video/x-msvideo', 'video/3gpp'],
                     'image': ['image/jpeg', 'image/png', 'image/gif']}

    def __init__(self, category=None):
        self.category = category
        if self.category:
            self.items = self.items_list()

    def items_list(self):
        keys = ''
        for item in self.allowed_mimes[self.category]:
            keys += ' MIME = "%s"' % item
            if item != self.allowed_mimes[self.category][-1]:
                keys += ' or'
        search_phrase = 'SELECT id FROM metadata WHERE %s' % keys

        queue_manager.put_and_join((search_phrase, ()))
        k = queue_manager.task_result
        items = []
        for row in k:
            items.append(unicode(row[0]))
        return items

    def didl_list(self):
        return [MetadataItems(str(e)).didlobject for e in self.items]

    def url_item(self, id):
        queue_manager.put_and_join(('SELECT * FROM metadata WHERE id=?',
                                   (unicode(id), )))
        k = queue_manager.task_result
        debug('maemo metalayer url item result ', str(k))
        return unicode(k[0][0])


class MetadataItems(object):

    def __init__(self, id):
        queue_manager.put_and_join(('SELECT * FROM metadata WHERE id=?',
                                   (unicode(id), )))

        k = queue_manager.task_result[0]

        self.mime = unicode(k[10])
        self.title = unicode(k[2])
        self.namespace = 'maemo_multimedia'
        self.parent_id = '16'
        self.object_id = id
        self.uri = get_uri(id, '/maemo_multimedia/')

        self.mediaType = self.mime
        self.protocolInfo = 'http-get:*:%s:*' % self.mime

        if k[1]:
            self.album = str(k[1])
        else:
            self.album = 'Unknown Album'
        if k[3]:
            self.date = unicode(k[3])
        if k[4]:
            self.publisher = unicode(k[4])
        if k[5]:
            self.artist = unicode(k[5])
        else:
            self.artist = "Unknown Artist"
        if k[6]:
            self.genre = unicode(k[6])
        else:
            self.genre = 'Unknown Genre'
        if k[7]:
            try:
                self.originalTrackNumber = int(k[7])
            except:
                self.originalTrackNumber = int(k[7].split('/')[0])
        if k[8]:
            self.bitrate = int(k[8])
        if k[9]:
            self.length = int(k[9])
        if self.resolution:
            self.resolution = unicode(k)
        if k[15]:
            self.channelNr = int(k[15])
        if k[16]:
            self.samplerate = int(k[16])
        if k[20]:
            self.sizebytes = int(k[20])

        if 'audio' in self.mime:
            self.didlobject = MusicTrack(self)
        elif 'video' in self.mime:
            self.didlobject = Movie(self)
        elif 'image' in self.mime:
            self.didlobject = ImageItem(self)

    def __getattribute__(self, name):
        try:
            value = object.__getattribute__(self, name)
        except AttributeError:
            value = None
        return value

    def _get_object_id(self):
        return self._object_id

    def _set_object_id(self, object_id):
        self._object_id = "%s:%s" % (self.namespace, object_id)
        self.uri = object_id

    object_id = property(_get_object_id, _set_object_id)
