# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from distutils.core import setup


long_description = '''
BRisa UPnP Framework
BRisa is a framework written in Python that allows creation of UPnP devices,
focusing on Media Servers and Media Renderer. Regarding to the Media Server
reference implementation, it allows users to share and search content from UPnP
A/V devices. It offers a plugin architecture enabling users to share their own
contents stored in multimedia portals, such as SHOUTcast, Flickr and recently
incorporated YouTube Videos. In addition, the framework also provides a very
simple UPnP control point API, which has being used by Canola Project.
'''
version = '0.8.0'


def main():
    setup(
          name='brisa',
          version=version,
          description='UPnP A/V Framework',
          long_description=long_description,
          author='BRisa Team',
          author_email='brisa-develop@garage.maemo.org',
          url='https://garage.maemo.org/projects/brisa/',
          download_url='https://garage.maemo.org/projects/brisa/',
          license='MIT',
          maintainer='Leandro Melo de Sales (leandroal)',
          maintainer_email='leandro@embedded.ufcg.edu.br',
          platforms='any',
          keywords=['UPnP', 'Control Point', 'Media Server', 'Media Renderer',
                    'DLNA', 'Maemo'],
          scripts=['bin/brisa-conf'],
          packages=['brisa',
                    'brisa/upnp',
                    'brisa/upnp/message_handle',
                    'brisa/upnp/didl',
                    'brisa/config',
                    'brisa/utils',
                    'brisa/log',
                    'brisa/control_point',
                    'brisa/services',
                    'brisa/services/cds',
                    'brisa/services/web_server',
                    'brisa/services/connmgr',
                    'brisa/threading',
                    'brisa/xml_descriptions'],
          package_data={'brisa/xml_descriptions': ['*.xml']},
          classifiers=['Development Status:: 4 - Beta',
                       'Environment:: Maemo',
                       'Intended Audience:: Developers, End Users/Desktop',
                       'License:: MIT/X Consortium License',
                       'Natural Language:: English',
                       'Operating System:: Linux',
                       'Programming Language:: Python',
                       'Topic:: Multimedia'])


if __name__ == '__main__':
    main()
