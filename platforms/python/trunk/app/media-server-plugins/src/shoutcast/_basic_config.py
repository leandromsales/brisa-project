# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import os

from brisa.core import config

from brisa_media_server.conf.base import BaseConfig


plugin = 'media_server_plugin-shoutcast'


def get_basic_configuration():
    d = {'name': 'Shoutcast',
         'description': 'Serves shoutcast streams',
         'enable': True,
         'dialog_module': 'brisa_media_server.plugins.shoutcast.conf_gui',
         'dialog_class': 'ConfigGUI',
         'ports': '80:443:8080',
         'preload': False,
         'show_mirrors': False}
    return d


def main():
    BaseConfig(plugin, get_basic_configuration())


if __name__ == "__main__":
    main()
