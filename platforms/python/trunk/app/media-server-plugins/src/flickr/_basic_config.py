# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core import config

from brisa_media_server.conf.base import BaseConfig


plugin_section = 'media_server_plugin-flickr'


def get_basic_configuration():
    d = {'name': 'Flickr',
         'description': 'Browse Flickr pictures',
         'dialog_module': 'brisa_media_server.plugins.flickr.conf_gui',
         'dialog_class': 'ConfigGUI',
         'enable': True,
         'username': 'flickr_user',
         'private': 'no'}

    return d


def main():
    BaseConfig(plugin_section, get_basic_configuration())


if __name__ == '__main__':
    main()
