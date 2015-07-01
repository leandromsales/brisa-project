# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core import config, log

version = '0.1.1'
plugin_section = 'media_server_plugin-youtube'


def check_config():
    """ Checks if basic config have already been set up. If it has not been
    setup already, do it.
    """
    if not plugin_section in config.manager.get_section_names():
        log.info('First time configuration for Youtube plugin')
        from brisa_media_server.plugins.youtube import _basic_config
        _basic_config.main()

check_config()

from brisa_media_server.plugins.youtube.implementation import *
