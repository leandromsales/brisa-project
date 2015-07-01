# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team

class Plugin(object):
    '''
    The base class to all plugins.
    '''

    name = 'unimplmented_plugin'

    def __init__(self, *args, **kwargs):
        '''
        Initialize the base plugin.
        Arguments:
        args - The configuration parameters to the loadded plugin.
        '''
        pass

    def load(self):
        '''
        Load the plugin media information into the database.
        '''
        print self.name + ': Load unimplemented'

    def unload(self):
        '''
        Load the plugin media information into the database.
        '''
        print self.name + ': Unoad unimplemented'

    def getUsage(self):
        '''
        Return a usage message containing the structure of the
        args argument of the construct
        '''
        usage = 'Base plugins must not be used directly'
        return usage

class MediaItem(dict):
     '''
     A media item to be retorned by the plugin.
     '''
     def __init__(self):
         '''
         Set the mandatory fields of an media item dictionary.
         '''
         self['mediaType']=None
         self['encoded']=None



class PluginManager(object):
    '''
    Manages the loading and acess of plugins.
    '''

    # The classes of loaded plugins
    plugins_classes = {}

    # All plugins instances
    plugins = []

    def __init__(self):
        '''
        Constructor of the Plugin manager class
        '''
        self._load_plugins()

#    def _item_iterator(self):
#        for plugin in self.plugins
#        yield 1

#    def __iter__(self):
#        return self._item_iterator()

    def _load_plugins(self):
        '''
        Load all installed plugins into the plugin manager.
        '''
        plugin_list = Plugin.__subclasses__()
        for p in plugin_list:
            self.plugins_classes[p.name] = p

    def list_plugins(self):
        '''
        List all plugins loadded into the plugin manager.
        The name of the plugin is also the key to acces it's class.
        Returns:
        list_of_plugins - A list containing the names of the loaded plugins.
        '''
        plugin_list = Plugin.__subclasses__()
        list_of_plugins = []
        for p in self.plugins_classes.keys():
            list_of_plugins.append(p)
        return list_of_plugins

    def add_plugin(self, type, *args,**kwargs):
        '''
        Add a new plugin configuration to the plugin manager.
        The same plugin can be loadded more than one time, since
        each instance may contain differents configurations.
        Arguments:
        type - The type of the plugin to be loadded into the manager
        args - The configuration parameters to the loadded plugin
        '''
        # Instantiate a new plugin of given type with the given argument.
        plugin = self.plugins_classes[type](*args,**kwargs)
        self.plugins.append(plugin)

    def load_DB_all(self):
        '''
        Load all plugins in database.
        '''
        for p in self.plugins:
            try:
                p.load()
                del p
            except:
                print "Not possible to load plugin "+ p.name
