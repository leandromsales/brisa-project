# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import os

from brisa.core import config
from brisa_media_server.conf.base import BaseConfig


plugin = 'media_server_plugin-youtube'


def get_basic_configuration():
    d = {'name': 'Youtube',
         'enable': True,
         'description': 'Serves youtube video links',
         'dialog_module': 'brisa_media_server.plugins.youtube.conf_gui',
         'dialog_class': 'ConfigGUI',
         'username': 'youtube_user',
         'video_url': 'http://www.youtube.com/watch?v='}
    return d


def main():
    BaseConfig(plugin, get_basic_configuration())


if __name__ == "__main__":
    main()
