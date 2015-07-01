#Licensed under the MIT license
#http://opensource.org/licenses/mit-license.php
#Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from media_services.cds.plugins.plugin import Plugin
from media_services.cds.plugins.plugin import PluginManager


def main():


    plugin_manager = PluginManager()
    plugins = plugin_manager.list_plugins()
    print '>> plugins loaded:'
    for p in plugins:
        print p
    plugin_manager.add_plugin(
                              'flickr',
                              username='brisa_project',
                              private='yes')
    print '>> plugins added to plugin manager'
    plugin_manager.load_DB_all()
    print '>> plugins loaded into DB'

if __name__ == '__main__':
    main()



