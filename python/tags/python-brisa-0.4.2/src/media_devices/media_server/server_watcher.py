#from pyinotify.pyinotify import WatchManager,\
#Notifier, EventsCodes, ProcessEvent
from pyinotify import WatchManager, Notifier, EventsCodes, ProcessEvent
from persistence import Facade
from media_services.cds.plugins.file_system_plugin import FSPlugin

import os
import logging
import brisa_config
import time

watcher = WatchManager()
plugin_classes = []
mask = EventsCodes.IN_DELETE | EventsCodes.IN_CREATE | \
       EventsCodes.IN_MODIFY | EventsCodes.IN_MOVED_FROM | \
       EventsCodes.IN_MOVED_TO

class ServerProcessEvent(ProcessEvent):

    def __init__(self):
        pass

    def _search_path_handle(self, path):
        for pclass in plugin_classes:
            for dir in pclass.dirs:
                if dir.startswith(path):
                    return pclass
        return None

    def process_IN_CREATE(self, event):
        path = os.path.join(event.path, event.name)
        if os.path.isdir(path):
            watcher.add_watch(path, mask, rec=True)
            pclass = self._search_path_handle(event.path)
            pclass.dirs.append(path)
        else:
            if os.path.isfile(path):
                pclass = self._search_path_handle(event.path)
                if pclass != None:
                    plugin = pclass()
                    time.sleep(.05)
                    plugin.add_item(path)
                    logging.info(path + ' was added to shared directory and \
                        plugin ' + plugin.name + " handles the process of \
                        addition")

    def process_IN_MOVED_TO(self, event):
        self.process_IN_CREATE(event)

    def process_IN_MOVED_FROM(self, event):
        self.process_IN_DELETE(event)

    def process_IN_MODIFY(self, event):
        # TODO: when this event create occurs, this event also occurs
        # here we need to check this situation

        #path = os.path.join(event.path, event.name)
        #if os.path.isfile(path):
        #    self.remove_item(path)
        pass

    def process_IN_DELETE(self, event):
        path = os.path.join(event.path, event.name)
        if os.path.isdir(path):
            for root, dirs, files in os.walk(path):
                for file in files:
                    print os.path.join(root, file)
        else:
            pclass = self._search_path_handle(event.path)
            if os.path.isfile(path) and pclass != None:
                self.remove_item(path)

    def remove_item(self, local_path):
        logging.info(local_path + \
                     ' was removed from the shared directory')
        Facade.removeItemByLocalPath(local_path)

class ServerWatcher(object):

    def __init__(self, ):
        self.notifier = Notifier(watcher, ServerProcessEvent())
        self._get_filesystem_plugins()
        self._add_watches()

    def _get_filesystem_plugins(self):
        fs_plugins_subclasses = FSPlugin.__subclasses__()
        for pclass in fs_plugins_subclasses:
            if pclass.use == 'on' and pclass.watch == 'on':
                plugin_classes.append(pclass)

    def _add_watches(self):
        for pclass in plugin_classes:
            self.add_dir(pclass.dirs)

    def add_dir(self, dir):
        watcher.add_watch(dir, mask, rec=True)

    def remove_dir(self, dir):
        watcher.rm_watch(rec=True)

    def run(self):
        while True:
            try:
                self.notifier.process_events()
                if self.notifier.check_events():
                    self.notifier.read_events()
            except KeyboardInterrupt:
                self.notifier.stop()
                break

#x = ServerWatcher()
#x.add_dir('/tmp')
#x.run()