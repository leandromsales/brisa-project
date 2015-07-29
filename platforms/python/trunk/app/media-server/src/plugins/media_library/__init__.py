# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core import config, log

version = '0.1.1'
section_name = 'media_server_plugin-media_library'


def check_config():
    if section_name not in config.manager.get_section_names():
        log.info('First time configuration for Media Library plugin')
        from brisa_media_server.plugins.media_library import _basic_config
        _basic_config.main()

check_config()


from brisa_media_server.plugins.media_library.implementation import *
