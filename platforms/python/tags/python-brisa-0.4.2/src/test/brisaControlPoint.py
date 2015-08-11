# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import pygtk
pygtk.require('2.0')
import gtk

from twisted.internet import gtk2reactor
gtk2reactor.install()
from twisted.internet import reactor
from media_devices.control_point.control_point_manager \
    import ControlPointManager

class ControlPointGUI(object):

    def delete_event(self, widget, event, data=None):
        print "delete event occurred"
        return False

    def destroy(self, widget, data=None):
        print "destroy signal occurred"
        gtk.main_quit()

    def browse(self, widget, id, udn):
        self.controlPointManager.searchDevices()
#        self.controlPointManager.getSearchCapabilites(udn)
        out = self.controlPointManager.browse(id, udn)
        for o in out:
            print o.title, o.id

    def __init__(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.connect("delete_event", self.delete_event)
        self.window.connect("destroy", self.destroy)
        self.window.set_border_width(10)
#        self.searchButton = gtk.Button("Get Search Capabilities")
#        self.searchButton.connect("clicked", self.searchDevice, \
#            'uuid:YZiZCWuAqnRcLnhLuMUh')
#        self.window.add(self.searchButton)
#        self.searchButton.show()
        self.browseButton = gtk.Button("Browser the server")
        self.browseButton.connect("clicked", self.browse, '5',\
          'uuid:TACqpTatlxAhJBvOceCs')
        self.window.add(self.browseButton)
        self.browseButton.show()

        # and the window
        self.window.show()

        self.controlPointManager = ControlPointManager(None)

    def main(self):
        # All PyGTK applications must have a gtk.main(). Control ends here
        # and waits for an event to occur (like a key press or mouse event).
        gtk.main()

# If the program is run directly or passed as an argument to the python
# interpreter then create a HelloWorld instance and show it
if __name__ == "__main__":
    gui = ControlPointGUI()
    gui.main()