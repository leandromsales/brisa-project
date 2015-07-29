# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

from distutils.core import setup


long_description = """
BRisa User Profile Server provides authentication and facilities for user
profiles with preferences and others.
"""
version = '0.1.0'


def main():
    setup(
        name='brisa-userprofile-server',
        version=version,
        description='BRisa User Profile Server',
        long_description=long_description,
        author='BRisa Team',
        author_email='brisa-develop@garage.maemo.org',
        url='https://garage.maemo.org/projects/brisa/',
        download_url='https://garage.maemo.org/projects/brisa/',
        license='MIT',
        maintainer='Andre Dieb Martins (dieb_)',
        maintainer_email='dieb@embedded.ufcg.edu.br',
        platforms='any',
        scripts=['bin/brisa-userprofile-server'],
        keywords=['UPnP', 'User Profile', 'Multimedia', 'Profile'],
        package_dir = {'brisa_userprofile_server': 'src',
                       'brisa_userprofile_server/services': 'src/services',
                       'brisa_userprofile_server/services/up_profile':
                       'src/services/up_profile',
                       'brisa_userprofile_server/services/up_profile/'\
                       'persistence':
                       'src/services/up_profile/persistence',
                       'brisa_userprofile_server/xml_descriptions':
                       'src/xml_descriptions'},
        packages=['brisa_userprofile_server',
                  'brisa_userprofile_server/services',
                  'brisa_userprofile_server/services/up_profile',
                  'brisa_userprofile_server/services/up_profile/persistence',
                  'brisa_userprofile_server/xml_descriptions'],
        package_data={'brisa_userprofile_server/xml_descriptions': ['*.xml']},
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
