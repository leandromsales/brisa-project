# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# example textview-basic.py

import pygtk
pygtk.require('2.0')
import gtk

from os import path

class TextViewExample:

    def close_application(self, widget):
        gtk.main_quit()

    def save(self, widget):
        out =self.textbuffer.get_text(self.textbuffer.get_start_iter(),
                                  self.textbuffer.get_end_iter())

        file = path.join(path.expanduser('~'), '.brisa/brisa.conf')
        outfile = open(file, "w")
        outfile.write(out)
        pass

    def __init__(self):
        window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        window.set_resizable(True)
        window.connect("destroy", self.close_application)
        window.set_title("Brisa Configuration File")
        window.set_border_width(0)

        box1 = gtk.VBox(False, 0)
        window.add(box1)
        box1.show()

        box2 = gtk.VBox(False, 10)
        box2.set_border_width(10)
        box1.pack_start(box2, True, True, 0)
        box2.show()

        sw = gtk.ScrolledWindow()
        sw.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        textview = gtk.TextView()
        self.textbuffer = textview.get_buffer()
        sw.add(textview)
        sw.show()
        textview.show()

        box2.pack_start(sw)
        # Load the file textview-basic.py into the text window
        file_str = path.join(path.expanduser('~'), '.brisa/brisa.conf')
        infile = open(file_str, "rw")

        if infile:
            string = infile.read()
            infile.close()
            self.textbuffer.set_text(string)

        hbox = gtk.HButtonBox()
        box2.pack_start(hbox, False, False, 0)
        hbox.show()

        vbox = gtk.VBox()
        vbox.show()
        hbox.pack_start(vbox, False, False, 0)

        separator = gtk.HSeparator()
        box1.pack_start(separator, False, True, 0)
        separator.show()

        box2 = gtk.VBox(False, 10)
        box2.set_border_width(10)
        box1.pack_start(box2, False, True, 0)
        box2.show()

        button = gtk.Button("close")
        button.connect("clicked", self.close_application)
        box2.pack_start(button, True, True, 0)
        button.set_flags(gtk.CAN_DEFAULT)
        button.grab_default()
        button.show()

        button_save = gtk.Button("save")
        button_save.connect("clicked", self.save)
        box2.pack_start(button_save, True, True, 0)
        button_save.set_flags(gtk.CAN_DEFAULT)
        button_save.grab_default()
        button_save.show()

        window.show()


def main():
    gtk.main()
    return 0

if __name__ == "__main__":
    TextViewExample()
    main()