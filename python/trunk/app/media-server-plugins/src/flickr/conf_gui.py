# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

__all__ = ('ConfigGUI', )

import gtk

from os.path import dirname, join
from brisa.core import config

from brisa_media_server.conf.base_gtk_gui import CheckWidget, EntryWidget


plugin_sect = 'media_server_plugin-flickr'


class ConfigGUI(object):

    if_path = join(dirname(__file__), 'gui.glade')

    def __init__(self):
        self.entry_widgets = []
        self.check_widgets = []
        self.builder = gtk.Builder()
        self.builder.add_from_file(self.if_path)
        self.window = self.builder.get_object('window1')
        self.window.connect('destroy', self.quit)
        self.builder.connect_signals(self)

        base_vbox = self.builder.get_object('vbox2')

        self.enable_plugin = CheckWidget(plugin_sect,
                                         'enable',
                                         'Enable Flickr')

        self.private = CheckWidget(plugin_sect,
                                   'private',
                                   'Private')

        self.username = EntryWidget(plugin_sect,
                                    'username',
                                    'Username')

        self.entry_widgets.append(self.username)
        self.check_widgets.extend([self.enable_plugin,
                                   self.private])

        base_vbox.pack_start(self.enable_plugin.get_widget())
        base_vbox.pack_start(self.private.get_widget())
        base_vbox.pack_start(self.username.get_widget())

    def run(self):
        self.window.show()

    def quit(self, w=None, ww=None):
        self.window.destroy()

    def on_save_clicked(self, w=None, ww=None):
        for e in self.entry_widgets:
            e.save()
        for c in self.check_widgets:
            c.save()
        config.manager.save()
        self.popup_save_confirmation()

    def on_revert_clicked(self, w=None, ww=None):
        for e in self.entry_widgets:
            e.revert()
        for c in self.check_widgets:
            c.revert()
        self.popup_revert_confirmation()

    def on_close_clicked(self, w=None, ww=None):
        # destroy() ?
        self.window.hide()

    def popup_save_confirmation(self):
        dialog = gtk.MessageDialog(message_format='Configuration saved.',
                                   buttons=gtk.BUTTONS_OK)
        dialog.run()
        dialog.destroy()

    def popup_revert_confirmation(self):
        dialog = gtk.MessageDialog(message_format='Configuration reverted.',
                                   buttons=gtk.BUTTONS_OK)
        dialog.run()
        dialog.destroy()
