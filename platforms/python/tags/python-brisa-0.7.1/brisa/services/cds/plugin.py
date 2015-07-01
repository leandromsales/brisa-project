# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Plugin architecture. Contains the base for writing new plugins and
facilities managing it.

Singletons
==========
- RootPluginSingleton: root plugin for the media server, initial target of any
                       browse. All plugin folders are added inside this root
                       plugin.
- PluginManagerSingleton: takes care of automatically finding the plugins (on
                          the plugin folder), instantiating them and loading
                          them asynchronously using PluginLoader.

Basic use
=========
The basic use of the plugin manager is to set some class of yours as a listener
and load the plugins. Note that your class MUST have both on_start_plugin() and
on_end_plugin() methods implemented.

>>> from brisa.services.cds.plugin import PluginManagerSingleton
>>> someclass = SomeClass()
>>> PluginManagerSingleton.listener = someclass
>>> PluginManagerSingleton.load_plugins()

Among other uses you might not want to set up a listener and only load the
plugins. Setting a listener is completely optional.
"""

from brisa import log
from brisa.threading import ThreadManager, ThreadObject
from brisa.upnp.didl.didl_lite import Object, Container

log = log.getLogger("PluginManager")


class Plugin(object):
    """ The base class to all plugins. For writing plugins inherit from this
    class and set the appropriate values for your plugin:

        - name: friendly name for your plugin. All objects inside your plugin
          will be named in the form name:id.

        - usage: can be on or off. This value is automatically set when your
          plugin has an entry on the configuration file. When set to 'off', the
          plugin manager will not instantiate the plugin.

        - watch: can be on or off.  When set to 'on', the variable usage will
          be watched for changes and the brisa.conf entry will also be watched.
          This means that the plugin will get loaded/unloaded automatically
          according to these variables.

        - has_browse_filter: can be True or False. If your plugin implements
          the browse() function in a way that it uses the slicing/sorting
          (filters) parameters, has_browse_filter should be set to True. If
          not, it should be False (which means that the filter will be applied
          by the plugin manager - your plugin returns browse result).
    """
    name = 'plugin_stub'
    usage = False
    watch = False
    has_browse_filter = False

    def __init__(self, *args, **kwargs):
        """ Initializes the base plugin. If overwritten, remember to set
        plugin_manager to PluginManagerSingleton.
        """
        self.plugin_manager = PluginManagerSingleton

    def execute(self):
        """ Loads the plugin media information into the database.
        """
        self.load()
        self.plugin_manager.notify_plugin_loaded(self.name)

    def load(self):
        """ Forces subclasses to implement the load method.
        """
        raise NotImplementedError("%s: load method not implemented" %
                                  self.name)

    def unload(self):
        """ Unloads the plugin media information from memory.
        """
        raise NotImplementedError("%s: unload method not implemented" %
                                  self.name)

    def browse(self, str_object_id, browse_flag, filter, starting_index,
                requested_count, sort_criteria):
        """ Browse implementation for the plugin. See Plugin documentation for
        information about the has_browse_filter flag.
        """
        raise NotImplementedError("browse not implemented for plugin %s" %
                                  self.name)

    def search(self, str_object_id, browse_flag, filter, starting_index,
                requested_count, sort_criteria):
        """ Search implementation for the plugin.
        """
        raise NotImplementedError("search not implemented for plugin %s" %
                                  self.name)


class MemObject(object):
    """ Represents an object with the the minimum information.
    """

    def __init__(self, id, title, parent_container_id='-1'):
        self.id = id
        self.title = title
        self.parent_container_id = parent_container_id

    def __repr__(self):
        """ Representation of the MemObject is his title.
        """
        return self.title


class MemDIDLObject(Object):
    """ Represents an object with all DIDL Object information.
    """

    def __init__(self, *args, **kargs):
        self.didlobject = MemObject(*args, **kargs)


class MemContainer(MemObject):
    """ Represents an container with the minimum information.
    """

    def __init__(self, id, title, parent_container_id='-1'):
        MemObject.__init__(self, id, title, parent_container_id)

    def _get_childCount(self):
        """ Returns the number of children of a MemContainer.
        """
        return len(self.containers)

    childCount = property(_get_childCount)


class MemDIDLContainer(Container):
    """ Represents an container with all DIDL Container information.
    """

    def __init__(self, *args, **kargs):
        Container.__init__(self, *args, **kargs)
        self.didlobject = MemContainer(*args, **kargs)

    def add_child(self, container):
        """ Adds a child container to the MemDIDLContainer.
        """
        container.parent_container_id = self.didlobject.id
        self.containers.append(container)


class _RootPlugin(Plugin):
    """ Root plugin for all plugins. Receives the first browse which returns
    the initial folder structure.
    """
    name = "rootplugin"
    containers = {}

    def __init__(self):
        pass

    def load(self):
        """ Adds root containers for the root plugin.
        """
        self.add_container("brisaServerRoot", "0")
        self.add_container("Music", "1", "0")
        self.add_container("Pictures", "3", "0")
        self.add_container("Playlists", "12", "0")
        self.add_container("Video", "2", "0")
        self.add_container("Audio Broadcast", "4", "1")
        self.add_container("Video Broadcast", "5", "2")
        self.add_container("Remote Pictures", "6", "3")

    def unload(self):
        """ Unload implementation.
        """
        self.containers.clear()

    def browse(self, id, browse_flag, filter, starting_index,
                requested_count, sort_criteria):
        """ Browse implementation.
        """
        log.debug('Browse: id= %s, browse_flag= %s, filter= %s,\
        starting_index= %s, requested_count= %s, sort_criteria= %s',
        id, browse_flag, filter, starting_index, requested_count,
        sort_criteria)
        if browse_flag == "BrowseDirectChildren":
            return self.containers[id].containers
        else:
            return [self.containers[id]]

    def search(self, id, browse_flag, filter, starting_index, requested_count,
                sort_criteria):
        """ TODO Search implementation.
        """
        return None

    def add_container(self, title, object_id, parent_container_id="-1",
                      plugin=None):
        """ Adds a container to the root plugin.

        @param title: title
        @param object_id: id
        @param parent_container_id: parent id
        @param plugin: container owner plugin

        @type title: string
        @type object_id: string
        @type parent_container_id: string
        @type plugin: Plugin

        @return: the added container
        @rtype: MemDIDLContainer
        """
        object_id = str(object_id)
        parent_container_id = str(parent_container_id)
        if plugin != None:
            if object_id == None:
                raise Exception("Error adding root plugin container: you must \
define the object_id")
            if not hasattr(plugin, "browse") or not hasattr(plugin, "name"):
                raise NotImplementedError("The plugin must implement the \
'browse' method and assign an attribute 'name'.")
            object_id = "%s:%s" % (plugin.name, object_id)

        new_container = MemDIDLContainer(id=object_id,
                                         title=title,
                                         parent_container_id=\
                                         parent_container_id)
        self.containers[object_id] = new_container

        if parent_container_id != "-1":
            self.containers[parent_container_id].add_child(new_container)
        return new_container

RootPluginSingleton = _RootPlugin()


class PluginLoader(ThreadObject):
    """ Class that loads a plugin asynchronously.
    """

    plugin = None

    def __init__(self, plugin):
        """ Constructor for class PluginLoader.

        @param plugin: plugin instance
        @type plugin: Plugin
        """
        ThreadObject.__init__(self)
        self.plugin = plugin

    def run(self):
        """ ThreadObject main actions. Call start() instead of calling this
        run() directly.
        """
        try:
            self.plugin.execute()
        except Exception, e:
            log.error("ERROR loading %s plugin: %s" % (self.plugin.name,
                                                         str(e)))

    def prepare_to_stop(self):
        """ Unloads the plugin when stopped.
        """
        self.plugin.unload()


class _PluginManager(object):
    """ Manages the loading of plugins.
    """
    # Plugins instances
    plugins_instances = {}

    # Plugins classes
    plugins_classes = {}

    def __init__(self):
        """ Instantiates the plugin manager and recognizes the plugins.
        """
        from brisa.services.cds import plugins
        self.root_plugin = RootPluginSingleton
        self.root_plugin.load()
        self.plugins_classes[self.root_plugin.name] = \
        self.root_plugin.__class__
        self._recognize_plugins()
        self.listener = None

    def load_plugins(self):
        """ Instantiates plugins and loads them using PluginLoader.
        """
        self._instantiate_plugins()
        self._load_plugins()
        log.debug('Available plugins: %s', str(self.plugins_classes.keys()))

    def _recognize_plugins(self):
        """ Recognizes plugins classes.
        """
        classes = Plugin.__subclasses__()

        plugins_class_ref = self.plugins_classes.values()
        for _class in classes:
            if _class not in plugins_class_ref:
                self.plugins_classes[_class.name] = _class

    def _instantiate_plugins(self):
        """ Instantiates all plugin classes into the plugin manager.
        """
        plugins = self.plugins_classes.values()
        for plugin in plugins:
            if plugin.usage:
                try:
                    self.plugins_instances[plugin.name] = plugin()
                except Exception, e:
                    log.error("Error while instantiating %s: %s" % \
                              (plugin.name, str(e)))

    def load_plugin(self, plugin):
        """ Loads a plugin using PluginLoader.

        @param plugin: plugin instance to be loaded
        @type plugin: Plugin
        """
        plugins_loader = PluginLoader(plugin)
        plugins_loader.setName("BRisa Plugin Loader (%s)" % plugin.name)
        plugins_loader.start()

    def _load_plugins(self):
        """ Loads all plugins selected using load_plugin() method.
        """
        try:
            plugins = self.plugins_instances.values()
            for plugin in plugins:
                log.info("START loading plugin %s", plugin.name)
                self.load_plugin(plugin)
                if self.listener is not None:
                    self.listener.on_start_plugin(plugin.name)
        except Exception, e:
            log.show_exception("Error while loading plugin %s" % plugin.name,
                                   e, 1)

    def notify_plugin_loaded(self, plugin_name):
        """ Notifies the listener and through the logger that a plugin has
        finished loading.
        """
        log.info("END loading plugin %s", plugin_name)
        if self.listener is not None:
            self.listener.on_end_plugin(plugin_name)

PluginManagerSingleton = _PluginManager()
