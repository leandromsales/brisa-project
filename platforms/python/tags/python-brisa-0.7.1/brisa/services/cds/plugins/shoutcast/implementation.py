# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa import config
from brisa import log
from brisa.services.cds.plugin import Plugin
from brisa_shoutcast import BrisaShoutcast
from brisa.upnp.didl.didl_lite import AudioBroadcast, Container


class ShoutCastItem(AudioBroadcast):

    protocolInfo = 'http-get:*:audio/mpeg:*'

    def __init__(self, id, parent_container_id, namespace,
                 title, uri, date, bitrate, mediatype, genre):
        AudioBroadcast.__init__(self, id,
                                parent_container_id=parent_container_id,
                                namespace=namespace, title=title, date=date,
                                bitrate=bitrate, media_type=mediatype,
                                genre=genre)
        self._uri = uri

    def _gen_uri(self):
        return self._uri


class ShoutCastContainer(Container):

    def __init__(self, id, parent_container_id, name, namespace):
        Container.__init__(self, id, parent_container_id, 'ShoutCastContainer',
                           name, namespace)
        self.name = name


class ShoutCastPlugin(Plugin):
    """ShoutCast plugin.

    Obtains the urls of the shoutcast stations from www.shoutcast.com
    """
    name = 'shoutcast'
    genre_list = ['Adult', 'Alternative', 'Ambient', 'Blues',
                  'Christian', 'Classical', 'Country',
                  'Dance', 'Euro', 'Electronic', 'Folk', 'Jazz',
                  'Latin', 'Metal', 'News', 'Oldies',
                  'Progressive', 'Punk', 'Rap', 'Reggae',
                  'Rock', 'Soundtrack', 'Sport', 'Soul', 'World']
    usage = config.get_parameter_bool('shoutcast', 'usage')
    allowed_ports = config.get_parameter('shoutcast', 'ports').split(':')
    if allowed_ports[0] == '0':
        allowed_ports = None
    containers_list = None
    shoutcast = BrisaShoutcast()
    log = log.getLogger(name)
    id = '8'

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)

    def register(self):
        self.sccontainer = self.plugin_manager.root_plugin.\
                                        add_container('SHOUTcast Stations',
                                                      self.id, '4', self)

    def register_genres(self):
        container_list={}
        id = int(self.id) + 1
        for genre in self.genre_list:
            container = ShoutCastContainer(id, self.sccontainer.object_id,
                                           genre, self.name)
            container_list[str(id)] = [container]
            id = id + 1
        return container_list

    def show_containers(self):
        return [ct[0] for ct in self.containers_list.itervalues()]

    def load(self):
        self.register()

    def unload(self):
        pass

    def register_itens(self, id):
        if len(self.containers_list[id]) == 1:
            genre_id = int(id) - int(self.id) - 1
            genre = self.genre_list[genre_id]
            stations = self.shoutcast.station_list_by_genre(genre,
                                                            self.allowed_ports)
            scitem_list = []
            append = scitem_list.append
            for station in stations:
                item = ShoutCastItem(station['id'], '%s:%s' % (self.name, id),
                                     self.name, station['name'],
                                     station['url'], '1993-01-01',
                                     station['bitrate'], 'audio/broadcast',
                                     genre)
                append(item)
            self.containers_list[id].append(scitem_list)

    def find_item(self, id):
        for item_list in self.containers_list.values():
            if len(item_list) > 1:
                for item in item_list[1]:
                    if id == item.object_id.split(':')[1]:
                        return [item]
        return []

    def browse(self, id, browse_flag, filter, starting_index,
                requested_count, sort_criteria):
        self.log.debug('Browse: id= %s, browse_flag= %s,\
        filter= %s, starting_index= %s, requested_count= %s,\
        sort_criteria= %s', id, browse_flag, filter, starting_index,
                            requested_count, sort_criteria)
        if id == self.id:
            if not self.containers_list:
                self.containers_list = self.register_genres()
            return self.show_containers()
        elif int(self.id) < int(id) < 50:
            self.register_itens(id)
            return self.containers_list[id][1]
        else:
            return self.find_item(id)
