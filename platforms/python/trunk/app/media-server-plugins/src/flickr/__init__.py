# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

__all__ = ('implementation', )

from brisa.core import config, log

version = '0.1.1'
section_name = 'media_server_plugin-flickr'


def check_config():
    if section_name not in config.manager.get_section_names():
        log.info('First time configuration for Flickr plugin')
        from brisa_media_server.plugins.flickr import _basic_config
        _basic_config.main()

check_config()

from brisa_media_server.plugins.flickr.implementation import *
