# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.services.cds.plugins.flickr import brisa_flickr
from brisa.upnp.didl.didl_lite import Photo, Container
from brisa.services.cds.plugin import Plugin
from brisa.utils import properties
from brisa import log, config


class FlickrItem(Photo):

    protocolInfo = 'http-get:*:image/jpeg:*'

    def __init__(self, id, parent_container_id, namespace, title, url, type,
                 date):
        Photo.__init__(self, id, parent_container_id, date,
                       protocolInfo=self.protocolInfo,
                       type=type, name=title, title=title, namespace=namespace)
        self._uri = url

    def _gen_uri(self):
        return self._uri


class FlickrContainer(Container):

    def __init__(self, object_id, parent_id, name, namespace):
        Container.__init__(self, object_id, parent_id, 'FlickrContainer', name,
                           namespace)
        self.title = name


class FlickrPlugin(Plugin):
    """
    Plugin to acess an flickr account.
    """
    name='flickr'
    private='no'
    usage=config.get_parameter_bool('flickr', 'usage')
    log=log.getLogger(name)
    brisa_flickr.API_KEY='f544dbc725689997313a6437e1068b5a'
    brisa_flickr.SECRET='3281387735c48555'
    containers_list=None
    id = '9'

    def __init__(self, *args, **kwargs):
        """
        Initialize the flickr plugin.

        Arguments:
        args - The configuration parameters to the loadded plugin.
        """
        Plugin.__init__(self, *args, **kwargs)

    def _register(self):
        self.frcontainer = self.plugin_manager.root_plugin.\
                                        add_container("Flickr", self.id, "6",
                                                      self)

    def load(self):
        """Load Plugin.

        Load the plugin media information into the database.
        """
        self._register()

    def _register_containers(self):
        container_list = {}
        id = int(self.id) + 1
        self.user_container = FlickrContainer(id, self.frcontainer.object_id,
                                         'User Photos', self.name)
        self.interesting_container = FlickrContainer(id + 1,
                                                self.frcontainer.object_id,
                                                'Interesting Photos',
                                                self.name)
        container_list[str(id)] = [self.user_container]
        container_list[str(id+1)] = [self.interesting_container]
        return container_list

    def _register_itens(self, id):
        if len(self.containers_list[id]) == 1:
            if id == '10':
                photos = self._load_user_photos()
                ctid = self.user_container.id
            else:
                photos = brisa_flickr.get_interesting_photos()
                ctid = self.interesting_container.id
            fid = 100 * int(id)
            photoitem_list = []
            append = photoitem_list.append
            untitled_count = 0
            for photo in photos:
                if not photo['title']:
                    photo['title'] = 'Untitled %d' % untitled_count
                    untitled_count += 1
                photoitem = FlickrItem(str(fid), ctid,
                                   self.name, photo['title'],
                                   photo['url'], 'image/jpeg',
                                   '1900-01-01')
                fid += 1
                append(photoitem)
            self.containers_list[id].append(photoitem_list)

    def _load_user_photos(self):
        username = config.get_parameter('flickr', 'username')
        isprivate = config.get_parameter('flickr', 'private') == 'yes'
        brisa_flickr.username = username
        photos = brisa_flickr.get_user_photos(username, isprivate)
        return photos

    def show_containers(self):
        return [ct[0] for ct in self.containers_list.itervalues()]

    def find_item(self, id):
        for item_list in self.containers_list.values():
            if len(item_list) > 1:
                for item in item_list[1]:
                    if id == item.object_id.split(':')[1]:
                        return [item]
        return []

    def unload(self):
        pass

    def browse(self, id, browse_flag, filter, starting_index,
                requested_count, sort_criteria):
        self.log.debug('Browse: id= %s, browse_flag= %s,\
        filter= %s, starting_index= %s, requested_count= %s,\
        sort_criteria= %s', id, browse_flag, filter, starting_index,
                            requested_count, sort_criteria)
        if id == self.id:
            if not self.containers_list:
                self.containers_list = self._register_containers()
            return self.show_containers()
        elif int(self.id) < int(id) < 50:
            self._register_itens(id)
            return self.containers_list[id][1]
        else:
            return self.find_item(id)
