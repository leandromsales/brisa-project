# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.services.cds.plugin import *
from brisa.services.cds.plugins.canola.persistence import Facade
from brisa.services.web_server import WebServer, CustomResource, StaticFile,\
static
from brisa.utils.network import decode
from brisa import config
from brisa import log

#from twisted.web import static

from brisa.utils.database import resolve_database

db_uri = config.get_parameter('canola', 'connection')


class CanolaWebResourceAudio(CustomResource):

    def __init__(self):
        CustomResource.__init__(self)

    def getChildWithDefault(self, p, request):
        audio = Facade.select_url_audio_item_by_id(p)
        if audio:
            return static.StaticFile(decode(audio))
        else:
            return static.Data("", "")


class CanolaAudioPlugin(Plugin):
    id = '13'
    name = 'canola_audio'
    usage = config.get_parameter_bool('canola', 'usage_audio')
    log = log.getLogger(name)

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)
        self.plugin_manager.root_plugin.add_container("Canola Audios", self.id,
                                                      "1", self)
        WebServer().root_dir.add_resource(self.name, CanolaWebResourceAudio())
        resolve_database(db_uri, 'canola.db')

    def _register(self):
        pass

    def load(self):
        pass

    def unload(self):
        pass

    def browse(self, id, browse_flag, filter, starting_index,
                 requested_count, sort_criteria):
        self.log.debug('Browse: id=%r, browse_flag=%r,\
        filter=%r, starting_index=%r, requested_count=%r,\
        sort_criteria=%r', id, browse_flag, filter, starting_index,
                           requested_count, sort_criteria)
        if browse_flag == 'BrowseMetadata' and id != self.id:
            return Facade.select_audio_item(id)
        else:
            return Facade.select_audio_list()


class CanolaWebResourceMovie(CustomResource):

    def __init__(self):
        CustomResource.__init__(self)

    def getChildWithDefault(self, p, request):
        movie = Facade.select_url_video_item_by_id(p)[0]
        if movie:
            return StaticFile(decode(movie))
        else:
            return static.Data("", "")


class CanolaVideoPlugin(Plugin):
    id = '14'
    name = 'canola_video'
    usage = config.get_parameter_bool('canola', 'usage_video')
    log = log.getLogger(name)

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)
        self.plugin_manager.root_plugin.add_container("Canola Videos", self.id,
                                                      "2", self)
        WebServer().root_dir.add_resource(self.name, CanolaWebResourceMovie())
        resolve_database(db_uri, 'canola.db')

    def _register(self):
        pass

    def load(self):
        pass

    def unload(self):
        pass

    def browse(self, id, browse_flag, filter, starting_index,
                 requested_count, sort_criteria):
        self.log.debug('Browse: id=%r, browse_flag=%r,\
        filter=%r, starting_index=%r, requested_count=%r,\
        sort_criteria=%r', id, browse_flag, filter, starting_index,
                           requested_count, sort_criteria)
        if browse_flag == 'BrowseMetadata' and id != self.id:
            return Facade.select_video_item(id)
        else:
            return Facade.select_video_list()


class CanolaWebResourcePicture(CustomResource):

    def __init__(self):
        CustomResource.__init__(self)

    def getChildWithDefault(self, p, request):
        picture = Facade.select_url_picture_item_by_id(p)
        if picture:
            return StaticFile(decode(picture))
        else:
            return static.Data("", "")


class CanolaPicturePlugin(Plugin):
    id = '15'
    name = 'canola_picture'
    usage = config.get_parameter_bool('canola', 'usage_picture')
    log = log.getLogger(name)

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)
        self.plugin_manager.root_plugin.add_container("Canola Pictures",
                                                      self.id, "3", self)
        WebServer().root_dir.add_resource(self.name,
                                          CanolaWebResourcePicture())
        resolve_database(db_uri, 'canola.db')

    def _register(self):
        pass

    def load(self):
        pass

    def unload(self):
        pass

    def browse(self, id, browse_flag, filter, starting_index,
                 requested_count, sort_criteria):
        self.log.debug('Browse: id=%r, browse_flag=%r,\
        filter=%r, starting_index=%r, requested_count=%r,\
        sort_criteria=%r', id, browse_flag, filter, starting_index,
                           requested_count, sort_criteria)
        if browse_flag == 'BrowseMetadata' and id != self.id:
            return Facade.select_picture_item(id)
        else:
            return Facade.select_picture_list()
