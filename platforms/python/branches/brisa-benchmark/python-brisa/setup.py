# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from distutils.core import setup


long_description = '''
BRisa is an UPnP framework written in Python. It provides facilities for
creating UPnP devices and control points. It's API comprehends internet
messaging protocols and services (UDP, HTTP, SOAP), network utilitaries,
threading management, main loop, logging, configurations, webserver and more.
'''
version = '0.9.0'


def main():
    setup(
          name='brisa',
          version=version,
          description='UPnP framework',
          long_description=long_description,
          author='BRisa Team',
          author_email='brisa-develop@garage.maemo.org',
          url='https://garage.maemo.org/projects/brisa/',
          download_url='https://garage.maemo.org/projects/brisa/',
          license='MIT',
          maintainer='Leandro Melo de Sales (leandroal)',
          maintainer_email='leandro@embedded.ufcg.edu.br',
          platforms='any',
          keywords=['UPnP', 'Control Point', 'DLNA', 'Maemo', 'Device',
                    'Service'],
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
