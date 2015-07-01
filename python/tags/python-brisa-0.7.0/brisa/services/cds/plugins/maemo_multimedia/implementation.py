# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

import maemo_metalayer

from brisa.services.cds.plugin import *
from brisa.services.web_server import WebServer, CustomResource, static, \
StaticFile
from brisa.utils.network import decode
from brisa import config, log


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
    usage = config.get_parameter_bool('maemo_multimedia', 'usage')
    log = log.getLogger(name)

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)
        self.plugin_manager.root_plugin.add_container(
                                                    "maemo Audio Collection",
                                                    "16", "1", self)
        self.plugin_manager.root_plugin.add_container(
                                                    "maemo Video Collection",
                                                    "17", "2", self)
        self.plugin_manager.root_plugin.add_container(
                                                "maemo Picture Collection",
                                                "18", "3", self)
        WebServer().root_dir.add_resource(self.name, MetaStoreWebResource())

    def load(self):
        maemo_metalayer.create_database()

    def unload(self):
        pass

    def browse_metadata(self, id):
        return maemo_metalayer.select_item(id)

    def browse(self, id, browse_flag, filter, starting_index,
                 requested_count, sort_criteria):
        self.log.debug('Browse(%r, %r, %r, %r, %r, %r)', id, browse_flag,
                       filter, starting_index, requested_count, sort_criteria)

        if browse_flag == 'BrowseMetadata':
            return self.browse_metadata(id)
        else:
            if id == '16':
                return maemo_metalayer.select_items('audio')
            if id == '17':
                return maemo_metalayer.select_items('video')
            if id == '18':
                return maemo_metalayer.select_items('image')
