# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

__all__ = ('FolderManagerWidget', 'CheckWidget', 'EntryWidget', 'ListWidget')

import gtk

from brisa.core import config



# Folder widget

class Folder(object):
    """ Represents a folder, internal use only.
    """

    def __init__(self, path):
        self.path = path

    def get_list_item(self):
        return [self, self.path]



class FolderManagerWidget(object):

    def __init__(self, section, param):
        self.section = section
        self.param = param

        # Build widget
        self.hbox = gtk.HBox(False, 5)
        self.folders = gtk.ListStore(object, str)
        self.tree = gtk.TreeView(model=self.folders)
        fold_column = gtk.TreeViewColumn('Folder',
                                         gtk.CellRendererText(),
                                         text=1)
        self.buttons = gtk.VButtonBox()
        self.add = gtk.Button(stock=gtk.STOCK_ADD)
        self.rem = gtk.Button(stock=gtk.STOCK_REMOVE)

        self.buttons.pack_start(self.add, False, False, 2)
        self.buttons.pack_start(self.rem, False, False, 2)

        self.tree.append_column(fold_column)

        self.hbox.pack_start(self.tree, True, True, 2)
        self.hbox.pack_start(self.buttons, True, True, 2)

        self.tree.show()
        self.hbox.show()
        self.buttons.show()
        self.add.show()
        self.rem.show()

        self.add.connect('clicked', self.add_folder)
        self.rem.connect('clicked', self.rem_folder)

        # Load already cfg'ed folders
        self.load_folders_list(config.get_parameter_as_list(self.section,
                                                            self.param,
                                                            ':'))

    def load_folders_list(self, folders):
        self.folders.clear()

        for f in folders:
            self.folders.append(Folder(f).get_list_item())

    def add_folder(self, widget, data=None):
        # Open dialog and add the folder to self.folders
        dialog = gtk.FileChooserDialog('Choose a folder',
                            action=gtk.FILE_CHOOSER_ACTION_SELECT_FOLDER,
                            buttons=(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                                     gtk.STOCK_OPEN, gtk.RESPONSE_ACCEPT))
        res = dialog.run()

        if res == gtk.RESPONSE_ACCEPT:
            self._add_folder(dialog.get_uri().split('://')[1])

        dialog.destroy()

    def _add_folder(self, folder_path):
        for f in self.folders:
            if f.model.get_value(f.iter, 1) == folder_path:
                return

        self.folders.append(Folder(folder_path).get_list_item())

    def rem_folder(self, widget, data=None):
        # Remove selected folder from self.folders
        model, iter = self.tree.get_selection().get_selected()
        self.folders.remove(iter)

    def get_folder_list(self):
        # Get the "human" list of the folders present on the view
        return [f[1] for f in self.folders]

    def save_folders_list(self):
        config.manager.set_parameter(self.section,
                                     self.param,
                                     ':'.join(self.get_folder_list()))

    def revert(self):
        self.load_folders_list(config.get_parameter_as_list(self.section,
                                                            self.param,
                                                            ':'))

    def get_widget(self):
        # Returns the widget to be added somewhere
        return self.hbox


# Checkbox Widget


class CheckWidget(object):

    def __init__(self, section, param, label):
        self.section = section
        self.param = param
        self.cbox = gtk.CheckButton(label=label)
        self.cbox.show()

        # Get initial value from database.
        self.revert()

    def save(self):
        config.manager.set_parameter(self.section,
                                     self.param,
                                     self.cbox.get_active())

    def revert(self):
        v = bool(config.manager.get_parameter(self.section, self.param))
        self.cbox.set_active(v)

    def get_widget(self):
        return self.cbox


# Entry Widget


class EntryWidget(object):

    def __init__(self, section, param, label):
        self.section = section
        self.param = param
        self.hbox = gtk.HBox(spacing=5)
        self.label = gtk.Label(label)
        self.entry = gtk.Entry()
        self.hbox.pack_start(self.label, False, False)
        self.hbox.pack_start(self.entry)

        self.hbox.show()
        self.label.show()
        self.entry.show()

        # Get initial text from config
        self.revert()

    def save(self):
        config.manager.set_parameter(self.section,
                                     self.param,
                                     self.entry.get_text())

    def revert(self):
        self.entry.set_text(config.manager.get_parameter(self.section,
                                                         self.param))

    def get_widget(self):
        return self.hbox


# List Widget


class List(object):
    """ Represents a list item. Internal use only.
    """

    def __init__(self, value):
        self.value = value

    def get_list_item(self):
        return [self, self.value]



class ListWidget(object):

    def __init__(self, section, param, label):
        self.section = section
        self.param = param

        # Build widget
        self.hbox = gtk.HBox(False, 5)
        self.lists = gtk.ListStore(object, str)
        self.tree = gtk.TreeView(model=self.lists)
        fold_column = gtk.TreeViewColumn(label,
                                         gtk.CellRendererText(),
                                         text=1)
        self.buttons = gtk.VButtonBox()
        self.add = gtk.Button(stock=gtk.STOCK_ADD)
        self.rem = gtk.Button(stock=gtk.STOCK_REMOVE)

        self.buttons.pack_start(self.add, False, False, 2)
        self.buttons.pack_start(self.rem, False, False, 2)

        self.tree.append_column(fold_column)

        self.hbox.pack_start(self.tree, True, True, 2)
        self.hbox.pack_start(self.buttons, True, True, 2)

        self.tree.show()
        self.hbox.show()
        self.buttons.show()
        self.add.show()
        self.rem.show()

        self.add.connect('clicked', self.add_to_list)
        self.rem.connect('clicked', self.rem_from_list)

        # Load already cfg'ed lists
        self.load_list(config.get_parameter_as_list(self.section,
                                                    self.param,
                                                    ':'))

    def load_list(self, lists):
        self.lists.clear()

        for f in lists:
            self.lists.append(List(f).get_list_item())

    def add_to_list(self, widget, data=None):
        # Open dialog and add the list to self.lists
        dialog = gtk.Dialog()
        dialog.add_buttons(gtk.STOCK_OK, gtk.RESPONSE_OK,
                           gtk.STOCK_CLOSE, gtk.RESPONSE_CLOSE)

        hbox = gtk.HBox(spacing=5)
        entry = gtk.Entry()
        dialog.vbox.pack_start(hbox)

        hbox.pack_start(gtk.Label('Port number to add:'), False, False)
        hbox.pack_start(entry, True, True)

        hbox.show()
        entry.show()

        if dialog.run() == gtk.RESPONSE_OK:
            try:
                # Test if it is integer.
                int(entry.get_text())
                self._add_to_list(entry.get_text())
            except ValueError:
                log.warning('Value entered for port is not an integer')

        dialog.destroy()

    def _add_to_list(self, list_value):
        for f in self.lists:
            if f.model.get_value(f.iter, 1) == list_value:
                log.debug('Port already exists')
                return

        self.lists.append(List(list_value).get_list_item())

    def rem_from_list(self, widget, data=None):
        # Remove selected list from self.lists
        model, iter = self.tree.get_selection().get_selected()
        self.lists.remove(iter)

    def get_list(self):
        # Get the "human" list
        return [f[1] for f in self.lists]

    def save(self):
        config.manager.set_parameter(self.section,
                                     self.param,
                                     ':'.join(self.get_list()))

    def revert(self):
        self.load_list(config.get_parameter_as_list(self.section,
                                                    self.param,
                                                    ':'))

    def get_widget(self):
        # Returns the widget to be added somewhere
        return self.hbox
