# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

from distutils.core import setup


long_description = """
Customization plugins for brisa-media-server (Youtube, Flickr, Shoutcast, Maemo
Multimedia).
"""
version = '0.1.2'


def main():
    setup(
        name='brisa-media-server-plugins',
        version=version,
        description='BRisa Media Server Plugins',
        long_description=long_description,
        author='BRisa Team',
        author_email='brisa-develop@garage.maemo.org',
        url='https://garage.maemo.org/projects/brisa/',
        download_url='https://garage.maemo.org/projects/brisa/',
        license='MIT',
        maintainer='Andre Dieb Martins (dieb_)',
        maintainer_email='andre.dieb@gmail.com',
        platforms='any',
        keywords=['UPnP', 'Media Server', 'Multimedia', 'Sharing', 'Plugins'],
        package_dir = {'brisa_media_server/plugins': 'src',
                       'brisa_media_server/plugins/shoutcast': 'src/shoutcast',
                       'brisa_media_server/plugins/youtube': 'src/youtube',
                       'brisa_media_server/plugins/flickr': 'src/flickr'},
        packages=['brisa_media_server/plugins',
                  'brisa_media_server/plugins/shoutcast',
                  'brisa_media_server/plugins/youtube',
                  'brisa_media_server/plugins/flickr'],
        package_data={'brisa_media_server/plugins/flickr': ['*.glade', '*.png'],
                      'brisa_media_server/plugins/shoutcast': ['*.glade',
                                                               '*.png'],
                      'brisa_media_server/plugins/youtube': ['*.glade',
                                                             '*.png']},
        classifiers=['Development Status :: 3 - Alpha',
                     'Environment :: Other Environment',
                     'Intended Audience :: Developers',
                     'Intended Audience :: End Users/Desktop',
                     'License :: OSI Approved :: MIT License',
                     'Natural Language :: English',
                     'Operating System :: POSIX :: Linux',
                     'Programming Language :: Python',
                     'Topic :: Multimedia'])

if __name__ == "__main__":
    main()
