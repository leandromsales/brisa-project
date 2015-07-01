# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2009 Brisa Team <brisa-develop@garage.maemo.org>

import os
import dbus
import shutil

from os.path import join, abspath, exists, expanduser, isabs
from brisa.core import log, config
from brisa.upnp.didl.didl_lite import Element


log = log.getLogger('media-center-model')
home = join(expanduser('~'), '.media-center')
prefs_default_file = abspath(join(home, 'config.pickle'))
prefs_manager = None
prefs = 'media-center'
prefs_default = {}
bus = None


class SetupError(Exception):
    pass


class BackupError(Exception):
    pass


def clean_folder(folder):
    """ Deletes the folder subdirs and files.

    @param folder: absolute path to the folder
    @type folder: string
    """
    log.debug('Cleaning folder %s', folder)
    for root, dirs, files in os.walk(folder, topdown=False):
        [os.remove(join(root, f)) for f in files]
        [os.rmdir(join(root, folder)) for folder in dirs]


def setup_home(overwrite=False):
    """ Setup the home folder, if not already present.

    @param overwrite: when True erases the home folder and creates a new clean
                      one. Default behaviour is False.
    """
    if not exists(home):
        log.info('Home folder created')
        os.mkdir(home)
    elif overwrite:
        log.info('Overwriting home folder')
        clean_folder(home)
        os.mkdir(home)


def backup_prefs(backup_path):
    """ Perform a backup of the preferences to the path given.

    @param backup_path: absolute path to place the backup
    @type backup_path: string
    """
    log.info('Performing backup from %s to %s', prefs_default_file,
             backup_path)
    if exists(prefs_default_file):
        try:
            shutil.copy(prefs_default_file, backup_path)
        except IOError, io:
            raise BackupError(io.message)


def get_pref(pref):
    """ Returns a preference set on the media center section.

    @param pref: preference name
    @type pref: string

    @rtype: string
    """
    return pref_manager.get_parameter(prefs, pref)


def setup_prefs():
    """ Setup preferences.

    For using an alternate config file, use
    MEDIA_CENTER_CONFIG_FILE=/complete/path/to/your_config_file
    environment setup before running the media center.
    """
    global prefs_manager, prefs_default

    config_file = os.getenv('MEDIA_CENTER_CONFIG_FILE')
    if config_file:
        if not isabs(config_file):
            raise SetupException('Config. file path supplied on'\
                                 'MEDIA_CENTER_CONFIG_FILE is not an '\
                                 'absolute path')
        log.info('Replacing default config file by %s', config_file)
    else:
        config_file = prefs_default_file

    log.debug('Setting up preferences from %s', config_file)

    prefs_manager = config.ConfigurationManager(config_file, prefs_default)

    try:
        log.debug('Loading preferences')
        prefs_manager.update()
    except IOError, io:
        # Write first time
        log.debug('Writing first-time preferences')
        prefs_manager.save()

    log.info('Preferences setup completed')


def setup_bus():
    """ Setup bus section for accessing proxies.
    """
    global bus
    bus = dbus.SessionBus()


class MediaCenterModel(object):
    """ Model for the Media Center app.
    """

    media_server_if = 'br.edu.ufcg.embedded.brisa.MediaServer'
    media_server_obj = '/br/edu/ufcg/embedded/brisa/MediaServer'

    media_renderer_if = 'br.edu.ufcg.embedded.brisa.MediaRenderer'
    media_renderer_obj = '/br/edu/ufcg/embedded/brisa/MediaRenderer'

    msg_server_proxy_failure = 'Could not retrieve media server proxy'
    msg_renderer_proxy_failure = 'Could not retrieve media renderer proxy'

    msg_server_if_failure = 'Could not prepare server method interface.'
    msg_renderer_proxy_failure = 'Could not prepare renderer method interface'

    def __init__(self, start=True):
        """ Constructor for the MediaCenterModel class.
        """
        self.dms_proxy = None
        self.dmr_proxy = None
        self.dms_if = None
        self.dmr_if = None
        setup_home()
        setup_prefs()
        setup_bus()

        if start:
            self.acquire_media_server_proxy()
            self.acquire_media_renderer_proxy()

    def is_server_running(self):
        return bus.name_has_owner(self.media_server_if)

    def is_renderer_running(self):
        return bus.name_has_owner(self.media_renderer_if)

    def acquire_media_server_proxy(self, override=False):
        """ Acquires the media server proxy.

        @param override: if True, overrides any old proxy
        @type override: boolean
        """
        if not self.dms_proxy or override:
            log.debug('Retrieving media server proxy.')
            self.dms_proxy = bus.get_object(self.media_server_if,
                                            self.media_server_obj)
            self.dms_if = dbus.Interface(self.dms_proxy, self.media_server_if)

        if not self.dms_proxy:
            log.error(self.msg_server_proxy_failure)
            raise RuntimeError(self.msg_server_proxy_failure)
        if not self.dms_if:
            log.error(self.msg_server_if_failure)
            raise RuntimeError(self.msg_server_if_failure)

    def acquire_media_renderer_proxy(self, override=False):
        """ Acquires the media renderer proxy.

        @param override: if True, overrides any old proxy
        @type override: boolean
        """
        if not self.dmr_proxy or override:
            log.debug('Retrieving media renderer proxy.')
            self.dmr_proxy = bus.get_object(self.media_renderer_if,
                                            self.media_renderer_obj)
            self.dmr_if = dbus.Interface(self.dmr_proxy, self.media_renderer_if)

        if not self.dmr_proxy:
            log.error(self.msg_renderer_proxy_failure)
            raise RuntimeError(self.msg_renderer_proxy_failure)
        if not self.dmr_if:
            log.error(self.msg_renderer_if_failure)
            raise RuntimeError(self.msg_renderer_if_failure)

    # Media Server methods

    def dms_halt(self):
        """ Halts the media server.
        """
        log.info('Halting media server.')
        self.dms_if.dms_halt()

    def dms_cds_browse(self, id, browse_flag='BrowseDirectChildren',
                       filter='*', starting_index=0, requested_count=25,
                       sort_criteria='dc:title'):
        log.debug('DMS browse')
        ret = self.dms_if.dms_cds_browse(id, browse_flag, filter,
                                         starting_index, requested_count,
                                         sort_criteria)
        if ret:
            return Element.from_string(ret).get_items()
        else:
            return []

    def dms_cds_search(self, container_id, search_criteria, filter='*',
                            starting_index=0, requested_count=25,
                            sort_criteria='dc:title'):
        log.debug('DMS search')
        ret = self.dms_if.dms_cds_search(container_id, search_criteria, filter,
                                         starting_index, requested_count,
                                         sort_criteria)
        if ret:
            return Element.from_string(ret).get_items()
        else:
            return []

    def dms_cds_get_search_caps(self):
        log.debug('DMS get search caps')
        return self.dms_if.dms_cds_get_search_caps()

    def dms_cds_get_sort_caps(self):
        log.debug('DMS get sort caps')
        return self.dms_if.dms_cds_get_sort_caps()

    def dms_cds_get_sys_update_id(self):
        log.debug('DMS get system update id')
        return self.dms_if.dms_cds_get_system_update_id()

    def dms_cm_get_protocol_info(self):
        log.debug('(DMS cm get protocol info')
        return self.dms_if.dms_cm_get_protocol_info()

    def dms_cm_get_current_connection_ids(self):
        log.debug('DMS cm get current connection ids')
        return self.dms_if.dms_cm_get_current_connection_ids()

    def dms_cm_get_current_connection_info(self):
        log.debug('DMS cm get current connection info')
        return self.dms_if.dms_cm_get_current_connection_info()

    # Media Renderer methods

    def dmr_halt(self):
        """ Halts the media renderer.
        """
        log.info('Halting media renderer.')
        self.dmr_if.dmr_halt()

    def dmr_get_renderer_info(self):
        log.debug('Get renderer info.')
        return self.dmr_if.dmr_get_renderer_info()

    def dmr_avt_set_av_transport_uri(self, instance_id, current_uri,
                                     current_uri_metadata):
        log.debug('DMR avt set av transport uri')
        return self.dmr_if.dmr_avt_set_av_transport_uri(instance_id,
                                                        current_uri,
                                                        current_uri_metadata)

    def dmr_avt_get_media_info(self):
        log.debug('DMR avt get media info')
        return self.dmr_if.dmr_avt_get_media_info()

    def dmr_avt_get_media_info_ext(self):
        log.debug('DMR avt get media info ext')
        return self.dmr_if.dmr_avt_get_media_info_ext()

    def dmr_avt_get_transport_info(self):
        log.debug('DMR avt get transport info')
        return self.dmr_if.dmr_avt_get_transport_info()

    def dmr_avt_get_position_info(self):
        log.debug('DMR avt get position info')
        return self.dmr_if.dmr_avt_get_position_info()

    def dmr_avt_get_device_caps(self):
        log.debug('DMR avt get device caps')
        return self.dmr_if.dmr_avt_get_device_caps()

    def dmr_avt_get_transport_settings(self):
        log.debug('DMR avt get transport settings')
        return self.dmr_if.dmr_avt_get_transport_settings()

    def dmr_avt_play(self):
        log.debug('DMR avt play')
        return self.dmr_if.dmr_avt_play()

    def dmr_avt_stop(self):
        log.debug('DMR avt stop')
        return self.dmr_if.dmr_avt_stop()

    def dmr_avt_pause(self):
        log.debug('DMR avt pause')
        return self.dmr_if.dmr_avt_pause()

    def dmr_avt_seek(self, instance_id, unit, target):
        log.debug('DMR avt seek')
        return self.dmr_if.dmr_avt_seek(instance_id, unit, target)

    def dmr_avt_next(self):
        log.debug('DMR avt next')
        return self.dmr_if.dmr_avt_next()

    def dmr_avt_previous(self):
        log.debug('DMR avt previous')
        return self.dmr_if.dmr_avt_previous()

    def dmr_rc_list_presets(self):
        log.debug('DMR rc list presets')
        return self.dmr_if.dmr_rc_list_presets()

    def dmr_rc_select_preset(self, preset):
        log.debug('DMR rc select preset')
        return self.dmr_if.dmr_rc_select_preset(preset)

    def dmr_rc_get_volume(self, instance_id, channel):
        log.debug('DMR rc get volume')
        return self.dmr_if.dmr_rc_get_volume(instance_id, channel)

    def dmr_rc_set_volume(self, instance_id, channel, desired_volume):
        log.debug('DMR rc set volume')
        return self.dmr_if.dmr_rc_set_volume(instance_id, channel,
                                             desired_volume)

    def dmr_cm_get_protocol_info(self):
        log.debug('DMR cm get protocol info')
        return self.dmr_if.dmr_cm_get_protocol_info()

    def dmr_cm_get_current_connection_info(self):
        log.debug('DMR cm get current connection info')
        return self.dmr_if.dmr_cm_get_current_connection_info()

    def dmr_cm_get_current_connection_ids(self):
        log.debug('DMR cm get current connection ids')
        return self.dmr_if.dmr_cm_get_current_connection_ids()
