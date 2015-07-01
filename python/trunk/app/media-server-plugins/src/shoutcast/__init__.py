# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core import config, log

version = '0.1.1'
section_name = 'media_server_plugin-shoutcast'


def check_config():
    """ Checks if basic config have already been set up. If it has not been
    setup already, do it.
    """
    if not section_name in config.manager.get_section_names():
        log.info('First time configuration for Shoutcast plugin')
        from brisa_media_server.plugins.shoutcast import _basic_config
        _basic_config.main()

check_config()

from brisa_media_server.plugins.shoutcast.implementation import *
