# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

__all__ = ('ConfigGUI', )

import gtk

from os.path import dirname, join
from brisa.core import config, log

from brisa_media_server.conf.base_gtk_gui import CheckWidget, ListWidget

plugin_sect = 'media_server_plugin-shoutcast'


class ConfigGUI(object):

    if_path = join(dirname(__file__), 'gui.glade')

    def __init__(self):
        self.list_widgets = []
        self.check_widgets = []
        self.builder = gtk.Builder()
        self.builder.add_from_file(self.if_path)
        self.window = self.builder.get_object('window1')
        self.window.connect('destroy', self.quit)
        self.builder.connect_signals(self)

        base_vbox = self.builder.get_object('vbox2')
        audio_hbox = self.builder.get_object('hbox2')
        video_hbox = self.builder.get_object('hbox3')
        image_hbox = self.builder.get_object('hbox5')

        self.enable_plugin = CheckWidget(plugin_sect,
                                         'enable',
                                         'Enable Shoutcast')

        self.preload = CheckWidget(plugin_sect,
                                   'preload',
                                   'Preload')
        self.show_mirrors = CheckWidget(plugin_sect,
                                        'show_mirrors',
                                        'Show Mirrors')

        self.ports_list = ListWidget(plugin_sect, 'ports', 'Ports')

        self.list_widgets.append(self.ports_list)

        self.check_widgets.extend([self.enable_plugin,
                                   self.preload,
                                   self.show_mirrors])

        base_vbox.pack_start(self.enable_plugin.get_widget())
        base_vbox.reorder_child(self.enable_plugin.get_widget(), 0)
        base_vbox.pack_start(self.preload.get_widget())
        base_vbox.pack_start(self.show_mirrors.get_widget())
        base_vbox.pack_start(self.ports_list.get_widget())

    def run(self):
        self.window.show()

    def quit(self, w=None, ww=None):
        self.window.destroy()

    def on_save_clicked(self, w=None, ww=None):
        for f in self.list_widgets:
            f.save()
        for c in self.check_widgets:
            c.save()
        config.manager.save()
        self.popup_save_confirmation()

    def on_revert_clicked(self, w=None, ww=None):
        for f in self.list_widgets:
            f.revert()
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
