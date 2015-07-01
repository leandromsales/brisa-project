# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team

from media_services.cds.plugins.plugin import Plugin

class TestPluginB(Plugin):
    name = 'TestPluginB'

    def sayHello(self):
        print 'Plugin B'
