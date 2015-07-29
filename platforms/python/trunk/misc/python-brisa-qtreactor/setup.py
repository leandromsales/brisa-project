# Licensed under the GPLv2 License
# Copyright 2009 Brisa Team <brisa-develop@garage.maemo.org>

from distutils.core import setup


long_description = """
Qt-based reactor that enables python-brisa to integrate transparently with
Qt applications.
"""
version = '0.10.0'
license = 'License :: OSI Approved :: GNU General Public License (GPL)'

def main():
    setup(
        name='python-brisa-qtreactor',
        version=version,
        description='QT-based reactor for python-brisa',
        long_description=long_description,
        author='BRisa Team',
        author_email='brisa-develop@garage.maemo.org',
        url='https://garage.maemo.org/projects/brisa/',
        download_url='https://garage.maemo.org/projects/brisa/',
        license='LGPL',
        maintainer='Andre Dieb Martins (dieb_)',
        maintainer_email='dieb@embedded.ufcg.edu.br',
        platforms='any',
        keywords=['Qt', 'Reactor', 'python-brisa'],
        packages=['brisa/core/reactors'],
        classifiers=['Development Status :: 3 - Alpha',
                     'Environment :: Other Environment',
                     'Intended Audience :: Developers',
                     'Intended Audience :: End Users/Desktop',
                     license,
                     'Natural Language :: English',
                     'Operating System :: POSIX :: Linux',
                     'Programming Language :: Python',
                     'Topic :: Multimedia'])

if __name__ == "__main__":
    main()
