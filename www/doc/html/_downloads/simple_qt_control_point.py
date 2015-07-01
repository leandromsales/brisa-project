from brisa.core.reactors.qtreactor import QtReactor
reactor = QtReactor()

from PyQt4 import QtCore, QtGui
from brisa.upnp.control_point import ControlPoint

class QtControlPoint(QtGui.QWidget):
    
    
    def __init__(self):
        """ Constructor for class QtControlPoint, which constructs the Qt Gui
        and the control point and starts it.
        """
        QtGui.QWidget.__init__(self)

        self.cp = ControlPoint()
        self.cp.subscribe('new_device_event', self.on_new_device)
        self.cp.subscribe('removed_device_event', self.on_removed_device)
        self.devices = []

        self.verticalLayout = QtGui.QVBoxLayout(self)
        self.title = QtGui.QLabel("Qt Simple Control Point")
        font = QtGui.QFont()
        font.setPointSize(15)
        self.title.setFont(font)
        self.title.setAlignment(QtCore.Qt.AlignCenter)

        self.verticalLayout.addWidget(self.title)
        
        self.search_btn = QtGui.QPushButton("Search", self)
        self.verticalLayout.addWidget(self.search_btn)
        QtCore.QObject.connect(self.search_btn, QtCore.SIGNAL("clicked()"), self.search)
    
        self.stop_btn = QtGui.QPushButton("Stop Search", self)
        self.verticalLayout.addWidget(self.stop_btn)
        QtCore.QObject.connect(self.stop_btn, QtCore.SIGNAL("clicked()"), self.stop_search)
                
        self.listView = QtGui.QListWidget(self)
        self.verticalLayout.addWidget(self.listView)

        self.cp.start()
        reactor.add_after_stop_func(self.cp.stop)

    def on_new_device(self, dev):
        if dev.udn not in [d.udn for d in self.devices]:
            self.devices.append(dev)
        self.list_devices()
    
    def on_removed_device(self, udn):
        for dev in self.devices:
            if dev.udn == udn:
                self.devices.remove(dev)
        self.list_devices()
    
    def list_devices(self):
        """ Lists the devices at list view.
        """
        self.listView.clear()
        for d in self.devices:
            self.listView.addItem(str(d.friendly_name))

    def search(self):
        """ Starts search for new devices.
        """        
        self.cp.start_search(600, 'upnp:rootdevice')

    def stop_search(self):
        """ Stops search for new devices.
        """
        self.cp.stop_search()

def main():
    qt_cp = QtControlPoint()
    qt_cp.show()
    reactor.main()

if __name__ == '__main__':
    main()
