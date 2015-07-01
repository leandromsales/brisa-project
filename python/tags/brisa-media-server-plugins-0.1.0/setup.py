# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

from distutils.core import setup


long_description = """
BRisa Media Server allows users to share media and has support for plugins.
This package contains plugins made by BRisa Team (such as Youtube, Canola,
Flickr, Shoutcast).
"""
version = '0.1.0'


def main():
    setup(
        name='brisa-media-server',
        version=version,
        description='BRisa Media Server',
        long_description=long_description,
        author='BRisa Team',
        author_email='brisa-develop@garage.maemo.org',
        url='https://garage.maemo.org/projects/brisa/',
        download_url='https://garage.maemo.org/projects/brisa/',
        license='MIT',
        maintainer='Andre Dieb Martins (dieb_)',
        maintainer_email='dieb@embedded.ufcg.edu.br',
        platforms='any',
        keywords=['UPnP', 'Media Server', 'Multimedia', 'Sharing', 'Plugins'],
        package_dir = {'brisa_media_server/plugins': 'src',
                       'brisa_media_server/plugins/shoutcast': 'src/shoutcast',
                       'brisa_media_server/plugins/youtube': 'src/youtube',
                       'brisa_media_server/plugins/maemo_multimedia':
                       'src/maemo_multimedia',
                       'brisa_media_server/plugins/flickr': 'src/flickr'},
        packages=['brisa_media_server/plugins',
                  'brisa_media_server/plugins/shoutcast',
                  'brisa_media_server/plugins/youtube',
                  'brisa_media_server/plugins/maemo_multimedia',
                  'brisa_media_server/plugins/flickr'],
        classifiers=['Development Status:: 3 - Alpha',
                     'Environment:: Maemo',
                     'Intended Audience:: Developers, End Users/Desktop',
                     'License:: MIT/X Consortium License',
                     'Natural Language:: English',
                     'Operating System:: Linux',
                     'Programming Language:: Python',
                     'Topic:: Multimedia'])

if __name__ == "__main__":
    main()
