# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

import mimetypes
import cherrypy

import mutagen.mp3
import mutagen.asf
import os
from stat import ST_SIZE

from brisa.services.cds.plugins.filesystem.persistence import Facade
from brisa.services.cds.plugins.filesystem.filesystem_watcher import \
FileSystemWatcher, FilesystemProcessEvent
from brisa.upnp.didl.didl_lite import Container

from brisa import log, config
from brisa.utils.network import decode
from brisa.utils.database import resolve_database
from brisa.services.cds.plugin import Plugin
from brisa.services.web_server import WebServer, CustomResource, static

encoding = config.get_parameter("brisa", "encoding")


class FSWebResource(CustomResource):

    def __init__(self):
        CustomResource.__init__(self)

    def getChildWithDefault(self, path, request):
        try:
            id = int(path.split('/')[-1])
        except:
            return ''

        item = Facade.get_by_id(id)

        # we must have some check here. It can return more than one item (bug)
        if not item:
            return static.Data('', '')
        else:
            local_path = item.local_path
            return static.StaticFile(decode(local_path))


class FSPlugin(Plugin):
    name = 'filesystem'
    usage = True
    has_browse_filter = True

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)
        self.dir_web = FSWebResource()
        WebServer().root_dir.add_resource(self.name, self.dir_web)

    def _register(self):
        root_containers = [("Audio Genres", "1"), ("Audio Artists", "1"),
                           ("Audio Albums", "1"), ("Video Folders", "2"),
                           ("All Video", "2"), ("All Music", "1"),
                           ("Local Images", "3"), ("All Pictures", "3")]
        root_plugin = self.plugin_manager.root_plugin

        db_uri = config.get_parameter('persistence', 'connection')
        dbpath = resolve_database(db_uri, 'filesystem.db')

        if os.path.exists(dbpath):
            Facade.sync_database()
            for (title, parent_id) in root_containers:
                container = Facade.select_container_by_title(title)
                if container:
                    root_plugin.add_container(title, container.id, parent_id,
                                              self)
        else:
            Facade.create_db()
            for (title, parent_id) in root_containers:
                container = Facade.add_container(title, self.name)
                if container:
                    root_plugin.add_container(title, container.id, parent_id,
                                              self)

    def create_server_resource(self):
        self.content = ServerResource()
        self.root_device.webResourceList.append((content_name, self.content))

    def load(self):
        '''Load the plugin media information into the database.'''
        self._register()

        self.plugins = {}

        if FSPluginAudio.usage:
            plugin = FSPluginAudio()
            self.plugins[FSPluginAudio.name] = plugin
            plugin.load()

        if FSPluginVideo.usage:
            plugin = FSPluginVideo()
            self.plugins[FSPluginVideo.name] = plugin
            plugin.load()

        if FSPluginImage.usage:
            plugin = FSPluginImage()
            self.plugins[FSPluginImage.name] = plugin
            plugin.load()

        Facade.remove_all_invalid_items()
        fs_watcher = FileSystemWatcher()
        fs_watcher.setName("BRisa Plugin Watcher (%s)" % self.name)

        fs_watcher.add_plugin(FSPluginAudio)
        fs_watcher.add_plugin(FSPluginVideo)
        fs_watcher.add_plugin(FSPluginImage)
        fs_watcher.start()

    def browse(self, str_object_id, browse_flag, filter, starting_index,
                requested_count, sort_criteria):
        starting_index = int(starting_index)
        requested_count = int(requested_count)

        if ':' in str_object_id:
            int_object_id = int(str_object_id.split(':')[1])
        else:
            int_object_id = int(str_object_id)

        if ':' in sort_criteria:
            sort_criteria = sort_criteria.split(':')[1]

        object = Facade.get_by_id(int_object_id, skip_children=False,
                                  starting_index=starting_index,
                                  requested_count=requested_count,
                                  sort_criteria=sort_criteria)
        result = []
        if browse_flag == 'BrowseDirectChildren':
            if isinstance(object, Container):
                items_count = len(object.items)
                container_count = len(object.containers)
                for i in object.containers:
                    result.append(i)

                for i in object.items:
                    result.append(i)
        else:
            object.parent_id = '%s:%s' % (self.name, str_object_id)
            result.append(object)
        return result


class FSPluginAudio(object):
    name = 'filesystem_audio'
    usage = config.get_parameter_bool('audio', 'usage')
    watch = config.get_parameter_bool('audio', 'watch')
    dirs = config.get_parameter_as_list('audio', 'dirs')

    proto_template = "http-get:*:_MEDIA_TYPE_:*"
    template_musictrack = {'namespace': FSPlugin.name,
                           'composer': '',
                           'title': '',
                           'album': '',
                           'copyright': '',
                           'sizebytes': '',
                           'protocolInfo': '',
                           'description': '',
                           'longDescription': '',
                           'publisher': '',
                           'producer': '',
                           'media_type': '',
                           'language': '',
                           'local_path': '',
                           'genre': '',
                           'artist': '',
                           'duration': '00:00:00',
                           'bitrate': '',
                           'original_track_number': 0,
                           'samplerate': '',
                           'date': ''}

    id3_attrs = {'title': 'TIT2', 'artist': 'TPE1', 'album': 'TALB',
                 'genre': 'TCON', 'date': 'TDRC',
                 'composer': 'TCOM', 'publisher': 'TPUB'}

    wma_attrs = {'title': 'Title', 'artist': 'Author',
                 'album': 'WM/AlbumTitle', 'genre': 'genre', 'date': 'WM/Year',
                 'composer': 'composer', 'publisher': 'publisher',
                 'description': 'Description',
                 'originalTrackNumber': 'WM/TrackNumber'}

    audio_types = ['.mp3', '.wma']

    def load(self):
        self._add_item()

    def unload(self):
        pass

    def _reset_template_musictrack(self):
        for key in self.template_musictrack.keys():
            self.template_musictrack[key] = ''
        self.template_musictrack['duration'] = '00:00:00'
        self.template_musictrack['original_track_number'] = 0
        self.template_musictrack['namespace'] = FSPlugin.name

    def _add_item(self):
        for directory in self.dirs:
            for root, dirs, files in os.walk(directory):
                root = root + os.path.sep
                for filename in files:
                    path = os.path.join(root, filename)
                    sizebytes = os.stat(path)[ST_SIZE]
                    path_encoded = unicode(path, errors='replace').\
                                           encode('ascii', 'ignore')
                    (basename, ext) = os.path.splitext(filename)
                    if not Facade.has_item_by_local_path(path_encoded,
                                                         sizebytes):
                        if ext.lower() in self.audio_types:
                            self.template_musictrack['sizebytes'] = sizebytes
                            self.template_musictrack['folder_path'] = root
                            self._real_add_item(path)
                            self._reset_template_musictrack()
                    else:
                        # Item exists, get it
                        item = Facade.select_item_by_local_path(path_encoded)
                        item.valid = "1"
                        item.save()

    def _real_add_item(self, filename):
        '''Add description in media to DB'''
        self._get_audio_info(filename)
        self._store()

    def _get_audio_info(self, filename):
        '''Get the information about the audio media file.'''

        self.template_musictrack['local_path'] = filename

        (root, ext) = os.path.splitext(filename)
        media_type = mimetypes.types_map[ext.lower()]
        self.template_musictrack['media_type'] = media_type

        protocol_info = self.proto_template.replace("_MEDIA_TYPE_", media_type)
        self.template_musictrack['protocolInfo'] = protocol_info

        #Parse id3
        if media_type == 'audio/mpeg':
            self._parse_id3(filename)
        else:
            self._parse_wmaattr(filename)

        if self.template_musictrack['title'].strip() == "":
            title = _stp_tit_str(os.path.basename(root))
            title = title.replace("-", " ").replace("_", " ")
            self.template_musictrack['title'] = title

        try:
            self.template_musictrack['original_track_number'] = int(self.
                self.template_musictrack['original_track_number'].split('/')\
                [0])
        except:
            self.template_musictrack['original_track_number'] = 0

        self.template_musictrack['artist'] = _stp_tit_str(self.
                                                template_musictrack['artist'])
        self.template_musictrack['album'] = _stp_tit_str(self.
                                                template_musictrack['album'])
        self.template_musictrack['title'] = _stp_tit_str(self.
                                                template_musictrack['title'])
        self.template_musictrack['genre'] = _stp_tit_str(self.
                                                template_musictrack['genre'])

        if self.template_musictrack['album'] == "":
            self.template_musictrack['album'] = "Unknown Album"

        if self.template_musictrack['artist'] == "":
            self.template_musictrack['artist'] = "Unknown Artist"

        if self.template_musictrack['genre'] == "":
            self.template_musictrack['genre'] = "Unknown Genre"

    def _store(self):
        field = self.template_musictrack['local_path']
        self.template_musictrack['local_path'] = _enc(field, encoding)

        field = self.template_musictrack['title']
        self.template_musictrack['title'] = _enc(field, encoding)

        field = self.template_musictrack['copyright']
        self.template_musictrack['copyright'] = _enc(field, encoding)

        field = self.template_musictrack['description']
        self.template_musictrack['description'] = _enc(field, encoding)

        field = self.template_musictrack['longDescription']
        self.template_musictrack['longDescription'] = _enc(field, encoding)

        field = self.template_musictrack['publisher']
        self.template_musictrack['publisher'] = _enc(field, encoding)

        field = self.template_musictrack['producer']
        self.template_musictrack['producer'] = _enc(field, encoding)

        field = self.template_musictrack['media_type']
        self.template_musictrack['media_type'] = _enc(field, encoding)

        field = self.template_musictrack['genre']
        self.template_musictrack['genre'] = _enc(field, encoding)

        field = self.template_musictrack['artist']
        self.template_musictrack['artist'] = _enc(field, encoding)

        field = self.template_musictrack['album']
        self.template_musictrack['album'] = _enc(field, encoding)

        Facade.add_music_track(**self.template_musictrack)

    def _parse_id3(self, file):
        id3info = mutagen.mp3.Open(file)
        items = self.id3_attrs.items()

        for (db_field, id3_field) in items:
            if id3_field in id3info:
                self.template_musictrack[db_field] = \
                    unicode(id3info[id3_field])
            else:
                self.template_musictrack[db_field] = ""

        (root, ext) = os.path.splitext(file)
        self.template_musictrack['media_type'] = mimetypes.types_map[ext.
                                                                       lower()]

        if self.template_musictrack['date'] == '':
            self.template_musictrack['date'] = '1901-01-01'

        if 'year' in self.template_musictrack:
            del self.template_musictrack['year']

        self.template_musictrack['bitrate'] = id3info.info.bitrate
        self.template_musictrack['samplerate'] = id3info.info.sample_rate
        self.template_musictrack['duration'] = format_time(id3info.info.length)

    def _parse_wmaattr(self, file):
        wmainfo = mutagen.asf.Open(file)
        items = self.wma_attrs.items()

        for (db_field, wma_field) in items:
            if wma_field in wmainfo:
                self.template_musictrack[db_field] = \
                    unicode(wmainfo[wma_field])
            else:
                self.template_musictrack[db_field] = ""

        (root, ext) = os.path.splitext(file)
        self.template_musictrack['media_type'] = mimetypes.types_map[ext.
                                                                       lower()]

        if self.template_musictrack['date'] == '':
            self.template_musictrack['date'] = '1901-01-01'

        if 'year' in self.template_musictrack:
            del self.template_musictrack['year']

        self.template_musictrack['bitrate'] = wmainfo.info.bitrate
        self.template_musictrack['samplerate'] = wmainfo.info.sample_rate
        self.template_musictrack['duration'] = format_time(wmainfo.info.length)


def format_time(time):
    hours, left_time = divmod(time/1000, 3600)
    minutes, left_time = divmod(left_time, 60)
    seconds = left_time
    return '%02d:%02d:%02d' % (hours, minutes, seconds)


class FSPluginVideo(object):
    name = 'filesystem_video'
    usage = config.get_parameter_bool('video', 'usage')
    watch = config.get_parameter_bool('video', 'watch')
    dirs = config.get_parameter_as_list('video', 'dirs')

    proto_template = "http-get:*:_MEDIA_TYPE_:*"
    template_video = {'namespace': FSPlugin.name,
                      'title': '',
                      'sizebytes': '',
                      'protocolInfo': '',
                      'description': '',
                      'longDescription': '',
                      'mediaType': '',
                      'local_path': '',
                      'duration': '00:00:00',
                      'resolution': '',
                      'samplerate': ''}

    video_types = ['.mpg', '.avi', '.mpeg', '.mpe', '.mpa', '.m1v', '.mpg',
                 '.qt', '.mov', '.movie', '.mp4']

    def load(self):
        self._add_item()

    def unload(self):
        pass

    def _reset_template_video(self):
        for key in self.template_video.keys():
            self.template_video[key] = ''
        self.template_video['duration'] = '00:00:00'
        self.template_video['namespace'] = FSPlugin.name

    def _add_item(self):
        for directory in self.dirs:
            for root, dirs, files in os.walk(directory):
                root = root + os.path.sep
                for filename in files:
                    path = os.path.join(root, filename)
                    sizebytes = os.stat(path)[ST_SIZE]
                    path_encoded = unicode(path, errors='replace').\
                                           encode('ascii', 'ignore')
                    (basename, ext) = os.path.splitext(filename)
                    if not Facade.has_item_by_local_path(path_encoded,
                                                         sizebytes):
                        if ext.lower() in self.video_types:
                            self.template_video['sizebytes'] = sizebytes
                            self.template_video['folder_path'] = root
                            self._real_add_item(path)
                            self._reset_template_video()
                    else:
                        item = Facade.select_item_by_local_path(path)
                        item.valid = "1"
                        item.save()

    def _real_add_item(self, filename):
        '''Add description in media to DB'''
        (root, ext) = os.path.splitext(filename)
        title = _stp_tit_str(os.path.basename(root))
        title = title.replace("-", " ").replace("_", " ")
        self.template_video['title'] = title

        media_type = mimetypes.types_map[ext.lower()]
        self.template_video['media_type'] = media_type

        protocol_info = self.proto_template.replace("_MEDIA_TYPE_", media_type)
        self.template_video['protocol_info'] = protocol_info

        self.template_video['media_type'] = mimetypes.types_map[ext.lower()]
        self.template_video['local_path'] = filename
        self.template_video['root'] = _enc(root.split('/')[-2], encoding)
        self._store()

    def _store(self):
        field = self.template_video['local_path']
        self.template_video['local_path'] = _enc(field, encoding)

        field = self.template_video['title']
        self.template_video['title'] = _enc(field, encoding)

        field = self.template_video['root']
        self.template_video['root'] = _enc(field, encoding)

        Facade.add_video_item(**self.template_video)


class FSPluginImage(object):
    name = 'filesystem_image'
    usage = config.get_parameter_bool('image', 'usage')
    watch = config.get_parameter_bool('image', 'watch')
    dirs = config.get_parameter_as_list('image', 'dirs')

    proto_template = "http-get:*:_MEDIA_TYPE_:*"
    template_image = {'namespace': FSPlugin.name,
                      'title': '',
                      'sizebytes': '',
                      'protocolInfo': 'http-get:*:image/jpeg:*',
                      'description': '',
                      'longDescription': '',
                      'media_type': '',
                      'local_path': '',
                      'root': '', }

    image_types=['.jpg', '.ras', '.xwd', '.bmp', 'jpe', 'jpeg', '.xpm', '.ief',
                 '.pbm', '.tif', '.gif', '.ppm', '.xbm', '.tiff', '.rgb',
                 '.pgm', '.png', 'pnm']

    def __init__(self):
        self.albums = {}

    def load(self):
        self._add_item()

    def unload(self):
        pass

    def _reset_template_image(self):
        for key in self.template_image.keys():
            self.template_image[key] = ''
        self.template_image['protocol_info'] = 'http-get:*:image/jpeg:*'
        self.template_image['namespace'] = FSPlugin.name

    def _add_item(self):
        for directory in self.dirs:
            for root, dirs, files in os.walk(directory):
                for filename in files:
                    path = os.path.join(root, filename)
                    sizebytes = os.stat(path)[ST_SIZE]
                    path_encoded = unicode(path, errors='replace').\
                                           encode('ascii', 'ignore')
                    (basename, ext) = os.path.splitext(filename)
                    if not Facade.has_item_by_local_path(path_encoded,
                                                         sizebytes):
                        if ext.lower() in self.image_types:
                            self.template_image['sizebytes'] = sizebytes
                            self.template_image['folder_path'] = root
                            self._real_add_item(path)
                            self._reset_template_image()
                    else:
                        item = Facade.select_item_by_local_path(path)
                        item.valid = "1"
                        item.save()

    def _real_add_item(self, filename):
        '''Add description in media to DB'''
        (root, ext) = os.path.splitext(filename)
        title = _stp_tit_str(os.path.basename(root))
        title = title.replace("-", " ").replace("_", " ")

        self.template_image['title'] = _enc(title, encoding)
        self.template_image['local_path'] = _enc(filename, encoding)
        self.template_image['root'] = _enc(root.split('/')[-2], encoding)

        media_type = mimetypes.types_map[ext.lower()]
        self.template_image['media_type'] = media_type

        protocol_info = self.proto_template.replace("_MEDIA_TYPE_", media_type)
        self.template_image['protocol_info'] = protocol_info
        self._store()

    def _store(self):
        field = self.template_image['local_path']
        self.template_image['local_path'] = _enc(field, encoding)

        field = self.template_image['title']
        self.template_image['title'] = _enc(field, encoding)

        field = self.template_image['root']
        self.template_image['root'] = _enc(field, encoding)

        photo = Facade.add_photo(**self.template_image)


def _stp_tit_str(string):
    return string.strip().title()


def _enc(string, encoding="utf-8"):
    string = string.replace('\x00', '')
    if not isinstance(string, unicode):
        return unicode(string, encoding, errors="ignore")
    return string
