from brisa.core.reactors.qtreactor import QtReactor
reactor = QtReactor()

from PyQt4 import QtCore, QtGui
from brisa.core import config
from brisa.upnp.device import Device
from brisa.upnp.device.service import Service
from brisa.upnp.device.service import StateVariable


class QtDevice(QtGui.QWidget):
    
    
    def __init__(self):
        """ Constructor for class QtDevice, which constructs the Qt Gui.
        """
        QtGui.QWidget.__init__(self)

        self.verticalLayout = QtGui.QVBoxLayout(self)
        self.title = QtGui.QLabel("Qt Simple Device")
        font = QtGui.QFont()
        font.setPointSize(15)
        self.title.setFont(font)
        self.title.setAlignment(QtCore.Qt.AlignCenter)

        self.verticalLayout.addWidget(self.title)
        
        self.lineEdit = QtGui.QLineEdit(self)
        self.verticalLayout.addWidget(self.lineEdit)

        self.search_btn = QtGui.QPushButton("Start Device", self)
        self.verticalLayout.addWidget(self.search_btn)
        QtCore.QObject.connect(self.search_btn, QtCore.SIGNAL("clicked()"), self.start)
    
        self.stop_btn = QtGui.QPushButton("Stop Device", self)
        self.verticalLayout.addWidget(self.stop_btn)
        QtCore.QObject.connect(self.stop_btn, QtCore.SIGNAL("clicked()"), self.stop)

        self.lineEdit.setText('My Generic Device Name')
        self.root_device = None
        self.upnp_urn = 'urn:schemas-upnp-org:device:MyDevice:1'

    def _add_root_device(self):
        """ Creates the root device object which will represent the device
        description.
        """
        project_page = 'http://brisa.garage.maemo.org'
        serial_no = config.manager.brisa_version.replace('.', '').rjust(7, '0')
        self.root_device = Device(self.upnp_urn,
                             str(self.lineEdit.text()),
                             manufacturer='BRisa Team. Embedded '\
                                          'Laboratory and INdT Brazil',
                             manufacturer_url=project_page,
                             model_description='An Open Source UPnP generic '\
                                               'Device',
                             model_name='Generic Device Example',
                             model_number=config.manager.brisa_version,
                             model_url=project_page,
                             serial_number=serial_no)

    def _add_services(self):
        """ Creates the root device service.
        """
        # Creating the example Service
        service_name = 'MyService'
        service_type = 'urn:schemas-upnp-org:service:MyService:1'

        myservice = Service(service_name, service_type, '')
        var = StateVariable(self, "A_ARG_TYPE_Variable",
                              True, False, "string")
        myservice.add_state_variable(var)

        # Inserting a service into the root device
        self.root_device.add_service(myservice)

    def _load(self):
        self._add_root_device()
        self._add_services()

    def start(self):
        """ Starts the root device.
        """
        self.stop()
        self._load()
        self.root_device.start()
        reactor.add_after_stop_func(self.root_device.stop)

    def stop(self):
        """ Stops the root device.
        """
        if self.root_device:
            self.root_device.stop()
            self.root_device = None


def main():
    qt_dev = QtDevice()
    qt_dev.show()
    reactor.main()

if __name__ == '__main__':
    main()
