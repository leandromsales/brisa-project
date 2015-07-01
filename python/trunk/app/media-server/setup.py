# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

import shutil

from distutils.core import setup


long_description = """
UPnP A/V Media Server 1.0 implementation with addition of a plugin
architecture. Allows users to share multimedia items over UPnP with almost
zero-configuration.
"""
version = '0.1.2'


def main():
    setup(
        name='python-brisa-media-server',
        version=version,
        description='BRisa Media Server',
        long_description=long_description,
        author='BRisa Team',
        author_email='brisa-develop@garage.maemo.org',
        url='https://garage.maemo.org/projects/brisa/',
        download_url='https://garage.maemo.org/projects/brisa/',
        license='MIT',
        maintainer='Andre Dieb Martins (dieb_)',
        maintainer_email='andre.dieb@gmail.com',
        platforms='any',
        scripts=['bin/brisa-media-server', 'bin/brisa-media-server-conf',
                 'bin/brisa-media-server-assistant'],
        keywords=['UPnP', 'Media Server', 'Multimedia', 'Sharing'],
        package_dir = {'brisa_media_server': 'src',
                       'brisa_media_server/services': 'src/services',
                       'brisa_media_server/services/media_registrar_ms':
                       'src/services/media_registrar_ms',
                       'brisa_media_server/plugins': 'src/plugins',
                       'brisa_media_server/plugins/media_library':
                       'src/plugins/media_library',
                       'brisa_media_server/conf': 'src/conf',
                       'brisa_media_server/assistant': 'src/assistant',
                       'brisa_media_server/xml_descriptions':
                       'src/xml_descriptions'},
        packages=['brisa_media_server',
                  'brisa_media_server/services',
                  'brisa_media_server/services/media_registrar_ms',
                  'brisa_media_server/plugins',
                  'brisa_media_server/plugins/media_library',
                  'brisa_media_server/conf',
                  'brisa_media_server/assistant',
                  'brisa_media_server/xml_descriptions'],
        package_data={'brisa_media_server/conf': ['*.png', '*.glade'],
                      'brisa_media_server/assistant': ['*.glade'],
                      'brisa_media_server/xml_descriptions': ['*.xml'],
                      'brisa_media_server/plugins/media_library': ['*.png',
                                                                   '*.glade']},
        classifiers=['Development Status :: 3 - Alpha',
                     'Environment :: Other Environment',
                     'Intended Audience :: Developers',
                     'Intended Audience :: End Users/Desktop',
                     'License :: OSI Approved :: MIT License',
                     'Natural Language :: English',
                     'Operating System :: Linux',
                     'Programming Language :: Python',
                     'Topic :: Multimedia'],
        data_files=[('share/dbus-1/services',
                    ['src/br.edu.ufcg.embedded.brisa.MediaServer.service']),
                    ('share/applications/hildon',
                    ['data/brisa_media_server_conf.desktop'])])

if __name__ == "__main__":
    main()
