# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team


# Implementation of a simple GTK2.0 Application that uses the UPnP Framework
# It is based on the Twisted Framework

import pygtk
pygtk.require('2.0')
import gtk
import random
import string
import sys
import time
import signal, os
from time import sleep

from twisted.internet import gtk2reactor
gtk2reactor.install()
from twisted.internet import reactor

from upnp import RootDevice, Service, DeviceHandler

from media_services.cds.FSStorage import FSDirectory
from media_services.cds import ContentDirectoryControl

from media_services.connmgr import ConnectionManagerControl

from twisted.python import log
from twisted.web import resource

class ServerGtk2(object):

    port = 0
    ipAddress = ''
    mediaPath = ''
    deviceHandler = DeviceHandler()
    isRunning = False

    def __init__(self):
        # create a new window
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_size_request(720, 396)
        self.window.set_title("Brisa UPnP Media Server v. 0.01")
        self.window.connect("delete_event", self.delete_event)
        self.window.connect("destroy", self.destroy)
        self.window.set_border_width(10)

        main_vbox = gtk.VBox(True, 5)
        main_vbox.set_border_width(10)
        self.window.add(main_vbox)
        main_vbox.show()

        self.makeMenu(main_vbox)
        self.makeMainInterface(main_vbox)

        self.window.show()


    def callMakeNewInterface(self, widget, data=None):
        main_vbox = self.window.get_focus()
        self.makeMainInterface(main_vbox)

    def callStartMediaServer(self, widget, data=None):
        self.port = self.entryPort.get_text()
        self.ipAddress = self.entryIP.get_text()
        self.mediaPath = self.entryMediaPath.get_text()
        self.isRunning = True
        self.createMediaServer()

    def callStop(self, widget, data=None):
        if self.isRunning:
            self.deviceHandler.stopDevice()
            self.isRunning = False

    def createMediaServer(self):

        listenAddr = self.ipAddress
        listenPort = self.port
        mediaPath = self.mediaPath
        logOn = True

        if mediaPath == "":
            mediaPath = "media"

        listenURL = "http://" + listenAddr + ":" + listenPort

        # Creating a random udn
        uuid = ''.join([ 'uuid:'] + map(lambda x: random.choice(string.letters), xrange(20)))

        # Creating a root device and setting some features
        myDevice = RootDevice("MediaServer:1", "Brisa MediaServer", uuid, listenURL)

        myDevice.set_manufacturer("Brisa Team");
        myDevice.set_manufacturer_url("http://www.percomp.org");
        myDevice.set_model_description("Our first MediaServer Implementation");
        myDevice.set_model_number("model 01");
        myDevice.set_serial_number("0000001");
        # Creating a Content Directory Service
        myService = Service(listenURL, "ContentDirectory/control", "ContentDirectory/event", "ContentDirectory/presentation", "ContentDirectory/scpd.xml")
        myService.set_service_id("11")
        myService.set_service_type_version("ContentDirectory:1")

        # Inserting a control resource to deal with the actions
        content = resource.Resource()
        myService.add_description('media_services/cds/content-directory-scpd.xml')
        sdl = [{'klass':FSDirectory, 'path':mediaPath,'title':'Media'}]
        control = ContentDirectoryControl('My Media Server',
                                      storageDirectoryList = sdl,
                                      urlbase = listenURL+"/content",
                                      webbase = content)
        myService.insertControl(control)

        # Creating a ConnectionManager Service
        myService2 = Service(listenURL, "ConnectionManager/control", "ConnectionManager/event", "ConnectionManager/presentation", "ConnectionManager/scpd.xml")
        myService2.set_service_id("22")
        myService2.set_service_type_version("ConnectionManager:1")


        # Inserting a control (empty) to deal with the actions
        myService2.add_description('media_services/connmgr/connection-manager-scpd.xml')
        myService2.insertControl(ConnectionManagerControl())


        # Adding the services into the device
        myDevice.add_service(myService)
        myDevice.add_service(myService2)

        # Inserting "extra" web resources to be "putted" in the root of devices webserver
        myDevice.add_webResource(('content',content))

        # Creating a handler for the Device
        #deviceHandler = DeviceHandler()

        # Starting the device with its own thread
        #deviceHandler.startDevice(myDevice, logOn)

        #or you can use the reactor from Twisted. You just need to configuring the device and then start the reactor
        self.deviceHandler.config_device(myDevice, logOn)
        reactor.run()


    # This is useful for popping up are you sure you want to quit?
    # type dialogs.
    def delete_event(self, widget, event, data=None):
        print "delete event occurred"
        return False

    # Another callback
    def destroy(self, widget, data=None):
        if self.isRunning:
            self.deviceHandler.stopDevice()
        gtk.main_quit()


    def makeMenu(self, vbox):
        menu = gtk.Menu()
        menu2 = gtk.Menu()

        # Create a new menu-item with a name...
        menu_exit = gtk.MenuItem("Exit")
        menu_help = gtk.MenuItem("Help")
        menu_about = gtk.MenuItem("About")
        # ...and add it to the menu.
        menu.append(menu_exit)
        menu2.append(menu_help)
        menu2.append(menu_about)

        # Do something interesting when the menuitem is selected
        menu_exit.connect_object("activate", self.destroy, "file.quit")

        # Show the widget
        menu_exit.show()
        menu_help.show()
        menu_about.show()

        file_menu = gtk.MenuItem("File")
        help_menu = gtk.MenuItem("Help")

        file_menu.show()
        help_menu.show()

        # Now we specify that we want our newly created "menu" to be the
        # menu for the "root menu"
        file_menu.set_submenu(menu)
        help_menu.set_submenu(menu2)

        # Create a menu-bar to hold the menus and add it to our main window
        menu_bar = gtk.MenuBar()
        vbox.pack_start(menu_bar, False, False, 2)
        menu_bar.show()

        menu_bar.append (file_menu)
        menu_bar.append (help_menu)

    def addEditEntry(self):
        pass

    def makeMainInterface(self, main_vbox):
        vbox = gtk.VBox(False, 5)
        vbox.set_border_width(5)
        main_vbox.pack_start(vbox, False, False, 0)

        frameCreate = gtk.Frame("Execution Parameters")
        vbox.pack_start(frameCreate, False, False, 0)

        vbox_intra_frame = gtk.VBox(False, 0)
        vbox_intra_frame.set_border_width(5)
        frameCreate.add(vbox_intra_frame)

        self.hbox_name = gtk.HBox(False, 0)
        vbox_intra_frame.pack_start(self.hbox_name, False, False, 5)

        self.label = gtk.Label(" Local IP: ")
        self.label.set_justify(gtk.JUSTIFY_LEFT)
        self.hbox_name.pack_start(self.label, False, False, 0)

        self.entryIP = gtk.Entry()
        self.entryIP.set_max_length(15)
        self.entryIP.connect("activate", self.callStartMediaServer, self.entryIP)
        self.hbox_name.pack_start(self.entryIP, False, False, 0)

        hbox_user_name = gtk.HBox(False, 0)
        vbox_intra_frame.pack_start(hbox_user_name, False, False, 5)

        #
        self.label = gtk.Label(" Port: ")
        self.label.set_justify(gtk.JUSTIFY_LEFT)
        self.hbox_name.pack_start(self.label, False, False, 0)

        self.entryPort = gtk.Entry()
        self.entryPort.set_max_length(7)
        self.entryPort.connect("activate", self.callStartMediaServer, self.entryPort)
        self.hbox_name.pack_start(self.entryPort, False, False, 0)

        #
        self.label = gtk.Label(" Media Path: ")
        self.label.set_justify(gtk.JUSTIFY_LEFT)
        self.hbox_name.pack_start(self.label, False, False, 0)

        self.entryMediaP
