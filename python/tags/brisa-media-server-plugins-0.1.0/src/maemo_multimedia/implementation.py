# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

import maemo_metalayer

from brisa.services.cds.plugin import *
from brisa.services.web_server import WebServer, CustomResource, static, \
StaticFile
from brisa.utils.network import decode
from brisa import config, log

log = log.getLogger('maemo_multimedia')


class MetaStoreWebResource(CustomResource):

    def __init__(self):
        CustomResource.__init__(self)

    def getChildWithDefault(self, p, request):
        item = maemo_metalayer.select_url_item_by_id(p)
        if item:
            return StaticFile(decode(item))
        else:
            return static.Data("", "")


class MetaStorePlugin(Plugin):
    name = 'maemo_multimedia'
    usage = config.get_parameter_bool('media_server_plugin-maemo-multimedia',
                                      'enable')

    def load(self):
        rp = self.plugin_manager.root_plugin

        audio_container = rp.get_container('Audio')
        videos_container = rp.get_container('Videos')
        images_container = rp.get_container('Images')

        if not audio_container:
            audio_container = rp.add_container('Audio', 0)
        if not videos_container:
            videos_container = rp.add_container('Videos', 0)
        if not images_container:
            images_container = rp.add_container('Images', 0)

        self.audio_container = rp.add_container('Maemo Audio Collection',
                                                 audio_container.id,
                                                 self)

        self.video_container = rp.add_container('Maemo Video Collection',
                                                 videos_container.id,
                                                 self)

        self.picture_container = rp.add_container('Maemo Picture Collection',
                                                  images_container.id,
                                                  self)
        WebServer().add_resource(self.name, MetaStoreWebResource())
        maemo_metalayer.create_database()

    def unload(self):
        WebServer().remove_resource(self.name)

    def browse_metadata(self, id):
        return maemo_metalayer.select_item(id)

    def browse(self, id, browse_flag, filter, starting_index,
                 requested_count, sort_criteria):
        if browse_flag == 'BrowseMetadata':
            return self.browse_metadata(id)
        else:
            if id == self.audio_container.id:
                return maemo_metalayer.select_items('audio')
            elif id == self.video_container.id:
                return maemo_metalayer.select_items('video')
            elif id == self.picture_container.id:
                return maemo_metalayer.select_items('image')
