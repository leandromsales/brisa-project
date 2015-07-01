# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import logging
import traceback
import brisa_config
from persistence import Facade

from twisted.internet import reactor

from progress_bar import ProgressBar

class Plugin(object):
    '''
    The base class to all plugins.
    '''

    name = 'unimplmented_plugin'
    use = 'off'
    watch = 'off'
    observer = None

    def __init__(self, *args, **kwargs):
        '''
        Initialize the base plugin.
        Arguments:
        args - The configuration parameters to the loadded plugin.
        '''

        if "observer"in kwargs:
            self.observer = kwargs["observer"]

    def execute(self):
        '''Load the plugin media information into the database.'''

        self.load()
        self.finished()

    def load(self):
        '''Force subclasses implement the load method'''

        logging.warning(self.name + ': Load unimplemented')
        assert 0

    def unload(self):
        '''Load the plugin media information into the database.'''

        logging.warning(self.name + ': Unload unimplemented')
        assert 0

    def finished(self):

        logging.info("Finished " + self.name)

#        if self.observer != None:
#            self.observer.notify(self.name)
        # precisa destruir o plugin, ja que ele foi repassado pra thread?
        # usando por exemplo, del self?
    def getUsage(self):
        '''
        Return a usage message containing the structure of the
        args argument of the construct
        '''

        usage = 'Base plugins must not be used directly'

        return usage

class MediaItem(dict):
    '''A media item to be retorned by the plugin.'''

    def __init__(self):
        '''Set the mandatory fields of an media item dictionary.'''

        self['mediaType'] = None
        self['encoded'] = None

class PluginManager(object):
    '''Manages the loading and acess of plugins.'''

    # The classes of loaded plugins
    plugins_classes = {}

    # All plugins instances
    plugins = []

    def __init__(self):
        '''Constructor of the Plugin manager class'''

        self._recognize_plugins()
        logging.debug('Avaliable plugins: ' + str(self.get_plugin_list()))

        #self.current_progress_bar_value = 0
        #self.max_progress_bar_value = 100
        #self.progress_bar = ProgressBar(self.current_progress_bar_value,
        #                                    self.max_progress_bar_value, 40)
    def notify(self, plugin_name):


        logging.info(plugin_name)

        #total = len(self.plugins)
        #self.current_progress_bar_value = self.current_progress_bar_value + \
        #                              self.max_progress_bar_value / total
        #self.progress_bar.updateAmount(self.current_progress_bar_value)
        #self.progress_bar.set_label(plugin_name)
        #print self.progress_bar, "\r",
    def setup_plugins(self):

        logging.info("Loading Plugins")
        plugin_list = self.get_plugin_list()
        for plugin in plugin_list:
            self.add_plugin(plugin)

    def _recognize_plugins(self):
        '''Load all installed plugins into the plugin manager.'''

        plugin_list = Plugin.__subclasses__()
        for p in plugin_list:
            self.plugins_classes[p.name] = p

    def list_plugins(self):
        '''Deprecated: call get_plugin_list'''

        self.get_plugin_list()

    def get_plugin_list(self):
        '''
        List all plugins loaded into the plugin manager.
        The name of the plugin is also the key to acces it's class.
        Returns:
        list_of_plugins - A list containing the names of the loaded plugins.
        '''

        plugin_list = Plugin.__subclasses__()
        list_of_plugins = []
        for p in self.plugins_classes.keys():
            list_of_plugins.append(p)
        return list_of_plugins

    def add_plugin(self, type, *args, **kwargs):
        '''
        Add a new plugin configuration to the plugin manager.
        The same plugin can be loadded more than one time, since
        each instance may contain differents configurations.
        Arguments:
        type - The type of the plugin to be loadded into the manager
        args - The configuration parameters to the loadded plugin
        '''
        # Instantiate a new plugin of given type with the given argument.

        try:
            plugin_class = self.plugins_classes[type]
            if plugin_class.use == 'on':
                kwargs["observer"] = self
                plugin = plugin_class(*args, **kwargs)
                self.plugins.append(plugin)
        except Exception, e:
            logging.show_exception("Error while trying to add \
plugin " + type, e)

    def exec_plugins(self):
        '''Load all plugins in database.'''

        for p in self.plugins:
            try:
                #thread.start_new_thread(p.execute, ())
                p.execute()
            except Exception, e:
                logging.show_exception("Error while trying to load plugin \
" + p.name, e, 1)
        Facade.removeAllInvalidItems()