# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.services.cds.plugins.SQLWorkerThread import SQLWorkerThread
from brisa.services.cds.plugins.SQLQueueManager import SQLQueueManager
from brisa.services.web_server import WebServer
from brisa.upnp.didl.didl_lite import *
from brisa.utils import properties
from brisa import config


def get_uri(id, dir):
    return "%s%s%s" % (WebServer().listen_url, dir, id)

queue_manager = SQLQueueManager()
worker_thread = SQLWorkerThread(config.get_parameter('canola',
                                             'connection').split('sqlite:')[1])
worker_thread.set_sql_queue_manager(queue_manager)
worker_thread.start()


class Audio(Container):

    def __init__(self, *args, **kargs):
        Container.__init__(*args, **kargs)

class AudioItemsList(object):

    def __init__(self):
        self.items = self.items_list()

    def items_list(self):
        queue_manager.put_and_join("SELECT id FROM files;")
        k = queue_manager.task_result
        return k

    def didl_list(self):
        return [AudioItems(str(e[0])).didlobject for e in self.items]

    def url_audio(self, id):
        try:
            queue_manager.put_and_join("SELECT path FROM files WHERE id='%s'" \
                                       % id)
            k = queue_manager.task_result
            return unicode(k[0][0])
        except Exception, e:
            return False

class AudioItems(object):
    protocolInfo = 'http-get:*:audio/mpeg:*'

    def __init__(self, id):
        queue_manager.put_and_join("SELECT * FROM audios WHERE id='%s';" % id)
        audio = queue_manager.task_result
        self.title = audio[1]
        self.namespace = 'canola_audio'
        self.parent_id = '13'
        self.object_id = id
        self.uri = get_uri(id, '/canola_audio/')
        self.didlobject = AudioItem(self)
        self.date = '01-01-01'
        self.bitrate = None
        self.mediaType = 'audio/mpeg'
        self.genre = audio[5]
        self.description = None
        self.duration = None
        self.author = None
        self.rating = None
        self.longDescription = None
        self.copyright = None
        self.publisher = None
        self.producer = None
        self.protection = None
        self.contributor = None
        self.relation = None
        self.storageMedium = None
        self.language = None
        self.sizebytes = None
        self.rights = None
        self.region = None
        self.channelNr = None

    def _get_object_id(self):
        return self._object_id

    def _set_object_id(self, _id):
        self._object_id = "%s:%s" % (self.namespace, _id)
        self.uri = self._object_id

    object_id = property(_get_object_id, _set_object_id)


class Video(Container):

    def __init__(self, *args, **kargs):
        Container.__init__(*args, **kargs)

class VideoItemsList(object):

    def __init__(self):
        items = []
        self.items = self.items_list()

    def items_list(self):
        queue_manager.put_and_join("SELECT id FROM video;")
        k = queue_manager.task_result
        return k

    def didl_list(self):
        return [VideoItems(str(e[0])).didlobject for e in self.items]

    def url_video(self, id):
        queue_manager.put_and_join("SELECT url FROM video WHERE id='%s';" % id)
        k = queue_manager.task_result
        return k

class VideoItems(object):
    protocolInfo = 'http-get:*:video/mpeg:*'

    def __init__(self, id):
        queue_manager.put_and_join("SELECT * FROM video WHERE id='%s';" % id)
        video = queue_manager.task_result
        self.title = video[1]
        self.namespace = 'canola_video'
        self.parent_id = '14'
        self.object_id = id
        self.uri = get_uri(id, '/canola_video/')
        self.didlobject = Movie(self)
        self.date = '01-01-01'
        self.bitrate = None
        self.mediaType = 'video/mpeg'
        self.genre = ''
        self.description = None
        self.duration = None
        self.author = None
        self.rating = None
        self.longDescription = None
        self.copyright = None
        self.publisher = None
        self.producer = None
        self.protection = None
        self.contributor = None
        self.relation = None
        self.storageMedium = None
        self.language = None
        self.sizebytes = None
        self.rights = None
        self.region = None
        self.channelNr = None
        self.DVDRegionCode = None
        self.channelName = None
        self.scheduledStartTime = None
        self.scheduledEndTime = None
        self.actor = None
        self.director = None

    def _get_object_id(self):
        return self._object_id

    def _set_object_id(self, object_id):
        self._object_id = "%s:%s" % (self.namespace, object_id)
        self.uri = object_id

    object_id = property(_get_object_id, _set_object_id)

class Picture(Container):

    def __init__(self, *args, **kargs):
        Container.__init__(*args, **kargs)

class PictureItemsList(object):

    def __init__(self):
        items = []
        self.items = self.items_list()

    def items_list(self):
        queue_manager.put_and_join("SELECT id FROM picture;")
        k = queue_manager.task_result
        return k

    def didl_list(self):
        return [PictureItems(str(e[0])).didlobject for e in self.items]

    def url_picture(self, id):
        queue_manager.put_and_join("SELECT url FROM video WHERE id='%s';" % id)
        k = queue_manager.task_result
        return k

class PictureItems(object):
    protocolInfo = 'http-get:*:image/jpeg:*'

    def __init__(self, id):
        queue_manager.put_and_join("SELECT * FROM picture WHERE id='%s';" % id)
        picture = queue_manager.task_result
        self.title = picture[1]
        self.namespace = 'canola_picture'
        self.parent_id = '15'
        self.object_id = id
        self.uri = get_uri(id, '/canola_picture/')
        self.didlobject = ImageItem(self)
        self.date = '01-01-01'
        self.bitrate = None
        self.mediaType = 'image/jpeg'
        self.genre = ''
        self.description = None
        self.duration = None
        self.author = None
        self.rating = None
        self.longDescription = None
        self.copyright = None
        self.publisher = None
        self.producer = None
        self.protection = None
        self.contributor = None
        self.relation = None
        self.storageMedium = None
        self.language = None
        self.sizebytes = None
        self.rights = None
        self.region = None
        self.channelNr = None

    def _get_object_id(self):
        return self._object_id

    def _set_object_id(self, object_id):
        self._object_id = "%s:%s" % (self.namespace, object_id)
        self.uri = object_id

    object_id = property(_get_object_id, _set_object_id)
