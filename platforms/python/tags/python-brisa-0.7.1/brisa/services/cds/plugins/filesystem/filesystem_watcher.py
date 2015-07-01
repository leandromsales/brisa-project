# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import os
import time

from pyinotify import WatchManager, Notifier, EventsCodes, ProcessEvent
from persistence import Facade
# TODO: change this reference
#   - Not use this import
#   - Use PluginManager().plugins_classes
from brisa.services.cds.plugins.filesystem.implementation import *
from brisa.services.cds.plugin import _PluginManager, Plugin

from brisa import log, config
from brisa.threading import ThreadObject

watcher = WatchManager()
plugin_classes = []

mask = EventsCodes.IN_DELETE | EventsCodes.IN_CREATE | \
       EventsCodes.IN_MODIFY | EventsCodes.IN_MOVED_FROM | \
       EventsCodes.IN_MOVED_TO


class FilesystemProcessEvent(ProcessEvent):

    def _search_path_handle(self, path):
        for plugin in plugin_classes:
            for dir in plugin.dirs:
                if path in dir:
                    return plugin
        return None

    def process_IN_CREATE(self, event):
        path = os.path.join(event.path, event.name)
        if os.path.isdir(path):
            watcher.add_watch(path, mask, rec=True)
            pclass = self._search_path_handle(event.path)
            if pclass:
                plugin = pclass()
                time.sleep(.05)
                plugin.dirs.append('%s%s%s' % (event.path, os.sep, event.name))
                plugin._add_item()

                log.info('%s was added to shared directory and plugin \
                %s handles the process of addition', path, plugin.name)
        else:
            if os.path.isfile(path):
                pclass = self._search_path_handle(event.path)
                if pclass:
                    plugin = pclass()
                    time.sleep(.05)
                    plugin._add_item()

                    log.info('%s was added to shared directory and plugin \
                    %s handles the process of addition', path, plugin.name)

    def process_IN_MOVED_TO(self, event):
        self.process_IN_CREATE(event)

    def process_IN_MOVED_FROM(self, event):
        self.process_IN_DELETE(event)

    def process_IN_MODIFY(self, event):
        # TODO: when this event create occurs, this event also occurs
        # here we need to check this situation
        pass

    def process_IN_DELETE(self, event):
        path = '%s%s%s' % (os.path.join(event.path), os.sep, event.name)

        pclass = self._search_path_handle(event.path)
        if pclass:
            dir = pclass.dirs[0]
            pclass.dirs = []
            pclass.dirs.append(dir)

            if event.is_dir:
                Facade.remove_container_by_local_path(''.join([path, '/']))
            else:
                Facade.remove_item_by_local_path(path)
            time.sleep(.05)
            log.info('%s was removed from the shared directory', path)


class FileSystemWatcher(ThreadObject):

    def __init__(self):
        ThreadObject.__init__(self)
        self.notifier = Notifier(watcher, FilesystemProcessEvent())

    def _add_watches(self):
        for pclass in plugin_classes:
            self.add_dir(pclass.dirs)

    def add_dir(self, dir):
        watcher.add_watch(dir, mask, rec=True)

    def add_plugin(self, plugin):
        if plugin.usage and plugin.watch:
            log.info('%s watcher was added.', plugin.name)
            plugin_classes.append(plugin)
            self._add_watches()

    def remove_dir(self, dir):
        watcher.rm_watch(rec=True)

    def run(self):
        while self.is_running():
            self.notifier.process_events()
            if self.notifier.check_events():
                self.notifier.read_events()
        self.notifier.stop()
