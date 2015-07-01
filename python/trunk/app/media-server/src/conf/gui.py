# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core.reactors import Gtk2Reactor
reactor = Gtk2Reactor()

import gtk
import gobject

from os.path import dirname, join
from brisa.core import log
from brisa.core.config import manager


cur_dir = dirname(__file__)
glade_path = join(cur_dir, 'gui.glade')
icon_path = join(cur_dir, 'brisa.png')

plugin_sect = 'media_server_plugin-'


class PluginConf(object):

    def __init__(self, plugin):
        self.dict = manager.items(plugin)
        self.desc = self.dict.get('description', '')
        self.name = self.dict.get('name', '')
        self.dialog_module = self.dict.get('dialog_module', '')
        self.dialog_class = self.dict.get('dialog_class', '')

    def get_row(self):
        return (self, self.name, self.desc)

    def run_dialog(self):
        if not self.dialog_module or not self.dialog_class:
            log.error('Dialog for plugin conf %s not set.' % self.name)
            return

        dialog_module = __import__(self.dialog_module,
                                   fromlist=[self.dialog_class])

        if not hasattr(dialog_module, self.dialog_class):
            log.error('Dialog module for plugin conf %s does not contain the'\
                      ' dialog class specified.' % self.name)
            return

        dialog_class = getattr(dialog_module, self.dialog_class)

        if not hasattr(dialog_class, 'run'):
            log.error('Dialog class for %s does not have a run method.' %
                      self.name)
            return

        dialog_class().run()


class MediaServerConf(object):

    def __init__(self):
        self.plugins = {}
        self.window = gtk.Builder()
        self.list = gtk.ListStore(object, str, str)
        self.window.add_from_file(glade_path)
        self.main = self.window.get_object('main')
        self.tree = self.window.get_object('treeview1')

        # Setup list
        name_column = gtk.TreeViewColumn('Plugin', gtk.CellRendererText(),
                                         text=1)
        desc_column = gtk.TreeViewColumn('Description', gtk.CellRendererText(),
                                         text=2)
        name_column.set_visible(True)
        desc_column.set_visible(True)
        self.tree.append_column(name_column)
        self.tree.append_column(desc_column)
        self.main.connect('destroy', self.quit)
        self.main.set_icon_from_file(icon_path)
        self.window.connect_signals(self)
        self.tree.set_model(self.list)
        self.tree.show()

        # Setup plugins
        self.get_plugins()

    def quit(self, w=None, t=None):
        reactor.main_quit()

    def run(self):
        log.debug('Starting up...')
        self.main.show()
        reactor.main()

    def get_plugins(self):
        psecs = [n for n in manager.get_section_names() if plugin_sect in n]

        for p in psecs:
            try:
                log.debug('Creating conf obj for plugin %s' % p)
                self.plugins[p] = PluginConf(p)
            except Exception, e:
                log.error('Could not load plugin %s' % p)

        self.update_treeview()

    def update_treeview(self):
        self.list.clear()
        self.tree.hide()

        for p in self.plugins.values():
            log.debug('Appending: %s' % str(p.get_row()))
            self.list.append(p.get_row())

        self.tree.show()

    def on_treeview1_row_activated(self, treeview, path, row, data=None):
        (model, iter) = self.tree.get_selection().get_selected()
        model.get_value(iter, 0).run_dialog()

    def on_close_clicked(self, w=None, ww=None):
        log.debug('Close')
        self.quit()

    def on_edit_clicked(self, w=None, ww=None):
        log.debug('Edit')
        self.on_treeview1_row_activated(None, None, None)


def run():
    MediaServerConf().run()


if __name__ == '__main__':
    run()
