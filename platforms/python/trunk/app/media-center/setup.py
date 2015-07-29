# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

from distutils.core import setup


long_description = """
BRisa Media Center is an application designed for playing and sharing media.
"""
version = '0.1.2'


def main():
    setup(
        name='brisa-media-center',
        version=version,
        description='BRisa Media Center',
        long_description=long_description,
        author='BRisa Team',
        author_email='brisa-develop@garage.maemo.org',
        url='https://garage.maemo.org/projects/brisa/',
        download_url='https://garage.maemo.org/projects/brisa/',
        license='MIT',
        maintainer='Andre Dieb Martins (dieb_)',
        maintainer_email='andre.dieb@gmail.com',
        platforms='any',
        scripts=['bin/brisa-media-center'],
        keywords=['UPnP', 'Media Center', 'Multimedia', 'Sharing', 'Player'],
        package_dir = {'brisa_media_center': 'src',
                       'brisa_media_center/view': 'src/view'},
        packages=['brisa_media_center',
                  'brisa_media_center/view'],
        package_data={'brisa_media_center/view/edje/themes/default': ['*.edj'],
                      'brisa_media_center/view/edje/themes/default/data/images':
                      ['*.png'],
                      'brisa_media_center/view/edje/themes/default/data/fonts':
                      ['*.TTF', '*.ttf']},
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
