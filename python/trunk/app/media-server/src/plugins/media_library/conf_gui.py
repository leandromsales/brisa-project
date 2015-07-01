# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

__all__ = ('ConfigGUI', )

import gtk

from os.path import dirname, join
from brisa.core import config
from brisa_media_server.conf.base_gtk_gui import FolderManagerWidget, \
                                                 CheckWidget


plugin_sect = 'media_server_plugin-media_library'


class ConfigGUI(object):

    if_path = join(dirname(__file__), 'gui.glade')

    def __init__(self):
        self.folder_widgets = []
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
                                         'Enable Media Library')

        self.enable_audio = CheckWidget(plugin_sect,
                                        'enable_audio_listing',
                                        'Audio Folder Watch')
        self.enable_video = CheckWidget(plugin_sect,
                                        'enable_video_listing',
                                        'Video Folder Watch')
        self.enable_image = CheckWidget(plugin_sect,
                                        'enable_image_listing',
                                        'Image Folder Watch')

        self.audio_folders = FolderManagerWidget(plugin_sect, 'audio_folders')
        self.video_folders = FolderManagerWidget(plugin_sect, 'video_folders')
        self.image_folders = FolderManagerWidget(plugin_sect, 'image_folders')

        self.folder_widgets.extend([self.audio_folders,
                                    self.video_folders,
                                    self.image_folders])
        self.check_widgets.extend([self.enable_plugin,
                                   self.enable_audio,
                                   self.enable_video,
                                   self.enable_image])

        base_vbox.pack_start(self.enable_plugin.get_widget())
        base_vbox.reorder_child(self.enable_plugin.get_widget(), 0)
        audio_hbox.pack_start(self.enable_audio.get_widget())
        audio_hbox.pack_start(self.audio_folders.get_widget())
        video_hbox.pack_start(self.enable_video.get_widget())
        video_hbox.pack_start(self.video_folders.get_widget())
        image_hbox.pack_start(self.enable_image.get_widget())
        image_hbox.pack_start(self.image_folders.get_widget())

    def run(self):
        self.window.show()

    def quit(self, w=None, ww=None):
        self.window.destroy()

    def on_save_clicked(self, w=None, ww=None):
        for f in self.folder_widgets:
            f.save_folders_list()
        for c in self.check_widgets:
            c.save()
        config.manager.save()
        self.popup_save_confirmation()

    def on_revert_clicked(self, w=None, ww=None):
        for f in self.folder_widgets:
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
