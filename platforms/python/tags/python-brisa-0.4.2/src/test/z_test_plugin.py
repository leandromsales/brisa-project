# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

#import os
#try :
#    os.remove('/etc/brisa/brisaobject.db')
#finally:
#    pass

from media_services.cds.plugins.plugin import Plugin
from media_services.cds.plugins.plugin import PluginManager

import test_createDummyDB

from twisted.web import resource


def main():

    test_createDummyDB.create_my_dummyBD()
    plugin_manager = PluginManager()
    plugins = plugin_manager.list_plugins()
    content = resource.Resource()
    print '>> plugins loaded:'
    for p in plugins:
        print p
    plugin_manager.add_plugin('filemp3',
                              '/media/sda8/musik/planet_hemp/usurio',
                              webResource = content,
                              urlbase = "http://192.168.1.74/content/")
#    plugin_manager.add_plugin('file',
#                       '/home/adrian/Musicas/Ayreon - Into The Electric \
#    Castle:/home/adrian/Musicas/Alejandro Sanz/1 - Viviendo deprisa:/home/\
#    adrian/Musicas/Audioslave/Audioslave:/home/adrian/Musicas/DragonForce - \
#     2005 - Inhuman Rampage - Advance')
    print '>> plugins added to plugin manager'
    plugin_manager.load_DB_all()
    print '>> plugins loaded into DB'

#    for item in plugin_manager:
#        print item


#    while plugin_manager.has_next:
#        1/0
#        pass

if __name__ == '__main__':
    main()