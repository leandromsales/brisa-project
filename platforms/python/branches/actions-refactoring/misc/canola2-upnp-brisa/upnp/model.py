from brisa.core.reactors import EcoreReactor
reactor = EcoreReactor()

import brisa
brisa.__enable_logging__ = False

import os
import time
import logging
import urllib

from terra.core.task import Task
from terra.core.manager import Manager
from terra.core.model import Model, ModelFolder
from terra.core.threaded_model import ThreadedModelFolder
from terra.core.plugin_prefs import PluginPrefs
from terra.utils.encoding import to_utf8 as decode

from brisa.upnp.control_point import ControlPointAV, ControlPoint


__all__ = ('SharedModelFolder', )

log = logging.getLogger('plugins.shared')

mger = Manager()
PluginDefaultIcon = mger.get_class('Icon/Plugin')
ModelAudio = mger.get_class('Model/Media/Audio')
ModelVideo = mger.get_class('Model/Media/Video/Local')
ModelImage = mger.get_class('Model/Media/Image')
ModelDirectory = mger.get_class('Model/Folder/Directory')
CanolaError = mger.get_class('Model/Notify/Error')


class UPnPModelAudio(ModelAudio):
    terra_type = 'Model/Media/Audio/Shared/UPnP'

    def __init__(self, name, parent, item):
        ModelAudio.__init__(self, name, None)

        self.parent = parent
        self.rating = None
        self.title = item.title
        urls = [r.value for r in item.resources]

        if urls:
            self.uri = urls[0]
        else:
            self.uri = None

        if hasattr(item, 'artists') and item.artists:
            self.artist = decode(item.artists[0])
        if hasattr(item, 'genres') and item.genres:
            self.genre = decode(item.genres[0])
        if hasattr(item, 'albums') and item.albums:
            self.album = decode(item.albums[0])

        self.cover = self.get_cover()

    def load_cover_path(self):
        canola_prefs = PluginPrefs('settings')
        try:
            path = canola_prefs['cover_path']
        except KeyError:
            path = canola_prefs['cover_path'] = \
                os.path.expanduser('~/.canola/covers/')
            canola_prefs.save()

        if not os.path.exists(path):
            os.makedirs(path)
        return path

    def get_cover(self):
        path = self.load_cover_path()
        artist = self.artist
        album = self.album
        cover = '%s/%s/cover.jpg' % (artist, album)
        cover = os.path.join(path, cover)
        return cover


class UPnPModelVideo(ModelVideo):
    terra_type = 'Model/Media/Video/Shared/UPnP'

    def __init__(self, name, parent, item):
        ModelVideo.__init__(self, None)
        self.parent = parent
        self.title = item.title
        urls = [r.value for r in item.resources]

        if urls:
            self.uri = urls[0]
        else:
            self.uri = None

        self.name = name
        self.thumbnail = None


class UPnPModelImage(ModelImage):
    terra_type = 'Model/Media/Image/Shared/UPnP'

    def __init__(self, name, parent, item):
        ModelImage.__init__(self, name, None)

        self.parent = parent
        self.title = item.title
        urls = [r.value for r in item.resources]

        if urls:
            self.uri = urls[0]
        else:
            self.uri = None

        self.width = None
        self.height = None

        resolution = None
        if len(item.resources) > 0:
            resolution = item.resources[0].resolution
        if resolution:
            (width, height) = resolution.lower().split('x')
            self.width = int(width)
            self.height = int(height)


class UPnPModelFolder(ThreadedModelFolder, ModelDirectory):
    terra_type = 'Model/Folder/Directory/Shared/UPnP'

    def __init__(self, name, parent, device, folder_id, folder_name,
                 control_point):
        ThreadedModelFolder.__init__(self, name, None)
        ModelDirectory.__init__(self, None, name, None)
        self.parent = parent
        self.device = device
        self.folder_id = folder_id
        self.folder_name = folder_name
        self.control_point = control_point
        self.is_daemon = True

    def insert_new_items(self, items):
        array = []
        for item in items:
            if self.must_die:
                log.warn('Forced loading stop (must die)')
                return False

            upnp_item = None
            name = decode(item.title)
            klass = decode(item.upnp_class.lower())

            if 'playlist' in klass:
                continue
            elif 'container' in klass:
                upnp_item = UPnPModelFolder(name, self, self.device, item.id,
                                            name, self.control_point)
            elif 'audio' in klass:
                upnp_item = UPnPModelAudio(name, self, item)
            elif 'video' in klass:
                upnp_item = UPnPModelVideo(name, self, item)
            elif 'image' in klass:
                upnp_item = UPnPModelImage(name, self, item)
            else:
                log.info('UPnP item not match, item class:%s', klass)
                continue

            if isinstance(upnp_item, UPnPModelFolder) or upnp_item.uri:
                self.append(upnp_item)
                time.sleep(0.1)


        return True

    def do_load(self):
        log.debug('Loading folder', self)
        time.sleep(1)

        start = 0
        screen_count = 30

        self.control_point.current_server = self.device
        try:
            items = self.control_point.browse(self.folder_id,
                                              'BrowseDirectChildren',
                                              '*',
                                              start,
                                              screen_count)['Result']
        except Exception, e:
            log.debug('Error browsing: %s' % e)

        log.debug('Browse from %d to %d response: %s' % (start, screen_count,
                  items))

        if items and self.insert_new_items(items):
            array = []
            max_count = screen_count
            while items and len(items) == max_count:
                if self.must_die:
                    log.warn('Forced loading stop (must die)')
                    break

                start += len(items)
                max_count = 1
                items = self.control_point.browse(self.folder_id,
                                                  'BrowseDirectChildren',
                                                  '*',
                                                  start,
                                                  max_count)['Result']
                log.debug('Browse from %d to %d response: %s' % (start,
                          screen_count, items))
                array.extend(items)
                time.sleep(0.3)

                if len(array) > screen_count:
                    self.insert_new_items(array)
                    array = []

            self.insert_new_items(array)

        if not self.is_loaded:
            self.inform_loaded()

    def do_unload(self):
        log.info('%d UPnP items removed', len(self.children))
        ThreadedModelFolder.do_unload(self)


class UPnPSearchDevicesIcon(PluginDefaultIcon):
    terra_type = 'Icon/Folder/Task/Shared/UPnPSearchDevices'
    icon = 'icon/main_item/upnp_search'
    plugin = 'upnp'


class UPnPSearchDevicesModelFolder(ThreadedModelFolder, Task):
    terra_type = 'Model/Folder/Task/Shared/UPnPSearchDevices'
    terra_task_type = 'Task/Shared/UPnP'

    icon = 'upnp_search'
    server_down_msg = 'Server is going down.'
    network_down_msg = 'Network is down.'

    def __init__(self, parent):
        Task.__init__(self)
        ThreadedModelFolder.__init__(self, 'Search servers', parent)
        self.parent = parent
        self._setup_network()
        self.is_daemon = True
        self.cleanup_error = True
        self.control_point = None
        self.fd_handler = None
        self.callback_fd = None

    def _get_state_reason(self):
        return ThreadedModelFolder._get_state_reason(self)

    def _set_state_reason(self, value):
        msg = UPnPSearchDevicesModelFolder.server_down_msg
        if value and value.message == msg:
            log.info('Invalid state reason message: %s', value.message)
            return

        ThreadedModelFolder._set_state_reason(self, value)

    state_reason = property(_get_state_reason, _set_state_reason)

    def _check_network(self, network):
        msg = UPnPSearchDevicesModelFolder.network_down_msg
        if network.status == 0:
            self.cleanup_error = False
            self.state_reason = CanolaError(msg)
        else:
            self.cleanup_error = True
            self.state_reason = None

    def _setup_network(self):
        self._network = Manager().get_status_notifier('Network')
        self._check_network(self._network)
        self._network.add_listener(self._check_network)

    def new_device_found(self, device):
        if not 'urn:schemas-upnp-org:service:ContentDirectory:1' in \
            device.services:
            log.info('UPnP device: %s doesn\'t have ContentDirectory service',
                     device.friendly_name)
            return

        for item in self.children:
            if item.device.udn == device.udn:
                log.info('UPnP device: %s already in list',
                         device.friendly_name)
                return

        folder = UPnPModelFolder(decode(device.friendly_name), self, device,
                                 '0', '', self.control_point)
        self.append(folder)
        time.sleep(0.1)

        if not self.is_loaded:
            self.inform_loaded()

    def del_device(self, udn):
        msg = UPnPSearchDevicesModelFolder.server_down_msg
        for item in self.children:
            if item.device.udn == udn:
                if self.is_valid and item.is_valid:
                    item.state_reason = CanolaError(msg)
                self.remove(item)
                break

        if not self.is_loaded:
            self.inform_loaded()

    def start_search(self):
        self.control_point.start_search(600, 'upnp:rootdevice')

    def do_load(self):
        time.sleep(1)
        self.control_point = ControlPointAV()
        self.control_point.subscribe('new_device_event', self.new_device_found)
        self.control_point.subscribe('removed_device_event', self.del_device)
        self.start_search()
        self.inform_loaded()

    def do_unload(self):
        log.info('%d UPnP devices removed', len(self.children))
        ControlPoint.stop(self.control_point)

        if self.cleanup_error:
            log.info('Cleanning state reason')
            self.state_reason = None

        ThreadedModelFolder.do_unload(self)
