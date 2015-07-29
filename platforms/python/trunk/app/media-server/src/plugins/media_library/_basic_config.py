# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import os
import brisa_media_server

from brisa_media_server.conf.base import BaseConfig

user_home = os.path.expanduser('~')
plugin_section = 'media_server_plugin-media_library'


def guess_folder(match_list):
    matchs = []

    for k in [user_home, '%s/MyDocs' % user_home]:
        for (root, dirs, others) in os.walk(k):
            if root != k:
                break
            matchs.extend(['%s/%s' % (root, f) for f in match_list \
                           if f in dirs])

    return ':'.join(matchs)


def get_basic_configuration():
    audio_folder = guess_folder(['music', 'Music', 'audio', 'Audio',
                                 '.sounds'])
    video_folder = guess_folder(['video', 'Video', 'videos', 'Videos',
                                 '.videos'])
    image_folder = guess_folder(['pictures', 'Pictures', 'images', 'Images',
                                 '.images'])

    d = {'name': 'Media Library',
         'description': 'Serves local files',
         'dialog_module': 'brisa_media_server.plugins.media_library.conf_gui',
         'dialog_class': 'ConfigGUI',
         'audio_folders': audio_folder,
         'video_folders': video_folder,
         'image_folders': image_folder,
         'enable_audio_listing': True,
         'enable_video_listing': True,
         'enable_image_listing': True,
         'database_location': '%s/%s' % (user_home, '.brisa/media_library.db'),
         'enable': True}
    return d


def main():
    BaseConfig(plugin_section, get_basic_configuration())


if __name__ == "__main__":
    main()
