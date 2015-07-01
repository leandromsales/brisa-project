# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of the UPnP Control Point Gui

from twisted.internet import gtk2reactor
gtk2reactor.install()
from twisted.internet import reactor

import gtk
import gtk.glade
from threading import Thread

import logging

try:
    import hildon
    isHildon =True
except:
    isHildon = False

from control_point_manager import ControlPointManager
from media_services.cds.didl.DIDLLite import Container

class ControlPointGui(object):

    # Screen objects
    comboBox_server_devices = None
    comboBox_renderer_devices = None
    container_treestore = None
    container_tree_view = None

    # To control which devices the control point is controlling
    current_server_device = None
    current_renderer_device = None
    current_media_id = None

    # Control point manager used to manage the control point specific actions
    control_point_manager = None

    isHildon = True
    kill_app = False
    media_list = []

    def __init__(self):
        ##### gtk stuffs
        #Signals used
        signals={"gtk_main_quit": self._main_quit,
                 "on_quit_activate": self._main_quit,
                 "on_refresh_clicked": self._on_refresh_clicked,
                 "on_play_clicked": self._on_play_clicked,
                 "on_stop_clicked": self._on_stop_clicked}

        # Create ControlPointManager
        self.control_point_manager = ControlPointManager(self)

        # Glade file
        self.glade_xml = gtk.glade.XML("media_devices/control_point/\
control_point_gui.glade")

        # Create Screen Objects
        self.createAllScreenObjects()

        self.glade_xml.signal_autoconnect(signals)
        self.adjust_to_hildon()

        self.control_point_manager.start_search_device_loop()
        self.control_point_manager.search_device_loop.interval = 600

    ## Creation Objects Methods
    def createAllScreenObjects(self):
        self.create_server_combo_box()
        self.create_renderer_combo_box()
        self.create_container_tree_view()
        self.create_item_media_list()

    def create_server_combo_box(self):
        hbox = self.glade_xml.get_widget("hbox_servers")
        liststore = gtk.ListStore(str, str)
        self.comboBox_server_devices = gtk.ComboBox(liststore)
        #Connect to callback
        self.comboBox_server_devices.connect('changed', \
                                    self._changed_server_devices)

        cell = gtk.CellRendererText()
        self.comboBox_server_devices.pack_start(cell, True)
        self.comboBox_server_devices.add_attribute(cell, 'text', 0)

        hbox.add(self.comboBox_server_devices)
        self.comboBox_server_devices.show()

    def create_renderer_combo_box(self):
        hbox = self.glade_xml.get_widget("hbox_renders")
        #_changed_server_devices
        liststore = gtk.ListStore(str, str)
        self.comboBox_renderer_devices = gtk.ComboBox(liststore)
        #Connect to callback
        self.comboBox_renderer_devices.connect('changed', \
                                        self._changed_renderer_devices)

        cell = gtk.CellRendererText()
        self.comboBox_renderer_devices.pack_start(cell, True)
        self.comboBox_renderer_devices.add_attribute(cell, 'text', 0)

        hbox.add(self.comboBox_renderer_devices)
        self.comboBox_renderer_devices.show()

    def create_container_tree_view(self):
        self.container_treestore = gtk.TreeStore(str, str)

        # create the TreeView using treestore
        self.container_treeview = gtk.TreeView(self.container_treestore)
        self.container_treeview.connect("row_activated", \
self._on_container_treeview_activated, '')

        # create the TreeViewColumn to display the data
        tvcolumn = gtk.TreeViewColumn('Containers')
        # add tvcolumn to treeview
        self.container_treeview.append_column(tvcolumn)
        # create a CellRendererText to render the data
        cell = gtk.CellRendererText()
        # add the cell to the tvcolumn and allow it to expand
        tvcolumn.pack_start(cell, True)
        # set the cell "text" attribute to column 0 - retrieve text
        # from that column in treestore
        tvcolumn.add_attribute(cell, 'text', 0)
        tree_hbox = self.glade_xml.get_widget('tree_hbox')
        tree_hbox.add(self.container_treeview)
        self.container_treeview.show()

    def create_item_media_list(self):
        # create a ListStore with one string column to use as the model
        self.item_media_list_liststore = gtk.ListStore(str, str)
        # create the TreeView using treestore
        self.item_media_list_treeview = \
        gtk.TreeView(self.item_media_list_liststore)
        self.item_media_list_treeview.connect("cursor-changed", \
                                        self._on_media_item_listview_changed)
        self.item_media_list_treeview.connect("row_activated", \
                                              self._on_play_clicked)
        # create the TreeViewColumn to display the data
        tvcolumn = gtk.TreeViewColumn('Title')
        # add tvcolumn to treeview
        self.item_media_list_treeview.append_column(tvcolumn)
        # create a CellRendererText to render the data
        cell = gtk.CellRendererText()
        # add the cell to the tvcolumn and allow it to expand
        tvcolumn.pack_start(cell, True)
        # set the cell "text" attribute to column 0 - retrieve text
        # from that column in treestore
        tvcolumn.add_attribute(cell, 'text', 0)

        tree_hbox = self.glade_xml.get_widget('list_viewport')
        tree_hbox.add(self.item_media_list_treeview)
        self.item_media_list_treeview.show()

    def create_main_window(self):
        window = hildon.Window()
        window.set_title("Hildon Brisa")
        window.connect("destroy", self._main_quit)

        main_menu = self._create_main_menu()
        window.set_menu(main_menu)

        window.show_all()
        return window

    def _create_main_menu(self):
        # Create the menu items
        about_item = gtk.MenuItem("About")
        about_item.connect("activate", self._on_about_activated)
        quit_item = gtk.MenuItem("Quit")
        quit_item.connect("activate", self._main_quit)
        # Create submenus
        help_menu = gtk.Menu()
        help_menu.append(about_item)
        help_item = gtk.MenuItem("Help")
        help_item.set_submenu(help_menu)
        # Create the menu
        menu = gtk.Menu()
#        menu.append(help_item)
        menu.append(quit_item)
        menu.show()
        return menu

    ## End of Creation Objects Methods

    ## Actions
    def adjust_to_hildon(self):
        '''
        Adjust the GUI to be usable in maemo platform.
        '''
        if isHildon:
            self.app = hildon.Program()
            # Create the substiute hildon main window
            hildon_main_window = self.create_main_window()
            self.app.add_window(hildon_main_window)
            # Get the content of the gtk main window
            #and move to hildon_main_window
            gtk_main_vbox=self.glade_xml.get_widget('main_vbox')
            gtk_main_vbox.reparent(hildon_main_window)
            # Destroy the now useless menu
            main_menu=self.glade_xml.get_widget('main_menu')
            main_menu.destroy()
            # Destroy the now useless window
            gtk_main_window=self.glade_xml.get_widget('main_window')
            gtk_main_window.destroy()

    def refresh(self):
        logging.info('search device refresh event...')
        self.generate_server_list()
        self.generate_render_list()

    def generate_server_list(self):
        self._generate_combo('server')

    def generate_render_list(self):
        self._generate_combo('render')

    def _generate_combo(self, type):
        if type=='server':
            combo_box = self.comboBox_server_devices
            dict = self.control_point_manager.server_devices
        else:
            combo_box = self.comboBox_renderer_devices
            dict = self.control_point_manager.renderer_devices

        list_store = combo_box.get_model()
        list_store.clear()
        list_store.append(["", "none"])

        for key in dict:
            list_store.append([dict[key].get_friendly_name(), \
dict[key].get_udn()])

    def browse_media_server(self, id, iter=None):
        itens = self.control_point_manager.browse(id, \
                                            self.current_server_device)
        #self.container_treestore.clear()
        for item in itens:
            if isinstance(item, Container):
                self.container_treestore.append(iter, [item.title, item.id])
            else:
#                self.item_media_list_liststore.clear()
                self.item_media_list_liststore.append([item.title, item.id])

    def play(self):
        self.control_point_manager.play(self.current_server_device, \
                                        self.current_renderer_device, \
                                        self.current_media_id)

    def stop(self):
        self.control_point_manager.stop(self.current_server_device, \
                                        self.current_renderer_device)

    # End of Actions

    ## Handle events methods
    def _main_quit(self, window):
#        if self.isHildon:
#            if self.kill_app:
#                self.control_point_manager.stop()
#                gtk.main_quit()
#                self.kill_app = True
#        else:
        self.control_point_manager.stop_search_device_loop()
        gtk.main_quit()

    def _changed_server_devices(self, combobox):
        model = combobox.get_model()
        index = combobox.get_active()
        if index:
            self.container_treestore.clear()
            self.current_server_device = model[index][1]
            self.browse_media_server(0)
            self.item_media_list_liststore.clear()
            logging.debug('Media server controlled: " + model[index][0] + \
" UDN: " + model[index][1]')

    def _changed_renderer_devices(self, combobox):
        model = combobox.get_model()
        index = combobox.get_active()
        if index:
            self.current_renderer_device = model[index][1]
            self.stop()
            print "Media renderer controlled: " + model[index][0]

    def _on_device_list_update(self):
        self.refresh()

    def _on_refresh_clicked(self, button):
        rt = RefreshThread(self.control_point_manager.tick_search_device)
        rt.start()

    def _on_container_treeview_activated(self, treeview, path, row, data):
        (model, iter) = treeview.get_selection().get_selected()
        if not treeview.row_expanded(path):
            self.item_media_list_liststore.clear()
            self.browse_media_server(model.get_value(iter, 1), iter)
            treeview.expand_to_path(path)

    def _on_media_item_listview_changed(self, listview):
        (model, iter) = listview.get_selection().get_selected()
        self.current_media_id = model.get_value(iter, 1)

    def _on_play_clicked(self, play_button, *args, **kwargs):
        self.play()

    def _on_stop_clicked(self, stop_button, *args, **kwargs):
        self.stop()

    def _on_about_activated(self, widget):
        pass

    ## Destroy Objects Methods
    def destroy_server_combo_box(self):
        self.comboBox_server_devices.destroy()

    def destroy_renderer_combo_box(self):
        self.comboBox_renderer_devices.destroy()

class RefreshThread(Thread):

    handle = None

    def __init__(self, handle):
        Thread.__init__(self)
        self.handle = handle

    def run(self):
        self.handle()


def main():

    gui = ControlPointGui()
    gtk.main()

if __name__ == '__main__':
    main()