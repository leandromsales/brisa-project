**************
Code Examples
**************

These are some examples to show how simple it is to develop using BRisa.

-------------------------------
Creating a Binary Light Device
-------------------------------

This is the implementation of a Binary Light Device, which the specifications
you can find `here
<http://upnp.org/standardizeddcps/documents/BinaryLight1.0cc.pdf>`_.

The first thing you have to do is to choose a reactor. After that do the imports.
::

	from brisa.core.reactors import install_default_reactor
	reactor = install_default_reactor()
	
	import os
	
	from brisa.upnp.device import Device, Service

This is everything you need to implement the device. Now, you should implement your
Service class doing the following::

	class SwitchPower(Service):
	
	    def __init__(self):
	        Service.__init__(self, 'SwitchPower',
	                         'urn:schemas-upnp-org:service:SwitchPower:1',
	                         '',
	                         os.getcwd() + '/SwitchPower-scpd.xml')
	        self.target = False
	        self.status = False
	

	    def soap_SetTarget(self, *args, **kwargs):
	        self.target = kwargs['newTargetValue']
	        print 'Light switched ', {'1': 'on', '0': 'off'}.get(self.target, None)
	        # Status here is the target because our device is "perfect" and
	        # no errors could occur (this is a simulation)
	        self.status = self.target
	        return {}

	
	    def soap_GetTarget(self, *args, **kwargs):
	        return {'RetTargetValue': self.target}

	
	    def soap_GetStatus(self, *args, **kwargs):
	        return {'ResultStatus': self.status}

The Device will provide services to the Control Point, so, if the services
that the Device will provide are already created, you can create the Device class
::

	class BinaryLight(object):
	
	    def __init__(self):
	        self.server_name = 'Binary Light Device'
	        self.device = None
	
	    def _create_device(self):
	        project_page = 'https://garage.maemo.org/projects/brisa'
	        self.device = Device('urn:schemas-upnp-org:device:BinaryLight:1',
	                             self.server_name,
	                             manufacturer='Brisa Team. Embedded Laboratory '\
	                                          'and INdT Brazil',
	                             manufacturer_url=project_page,
	                             model_name='Binary Light Device',
	                             model_description='A UPnP Binary Light Device',
	                             model_number='1.0',
	                             model_url=project_page)
	                             
This is the method where you're going to specify the services
:: 

   def _add_services(self):
       switch = SwitchPower()
       self.device.add_service(switch)
	        
After doing this, the start method and the main are created. The start method is going 
to be called in the main method and initialize the Device appending the services and 
leaving it ready to use.
::
	
  def start(self):
      self._create_device()
      self._add_services()
      self.device.start()
      reactor.add_after_stop_func(self.device.stop)
      reactor.main()

  if __name__ == '__main__':
    device = BinaryLight()
    device.start()

Thats all it takes to implement the Binary Light Device.

You could follow these steps to implement any service/device, but remember that for
now you'll have to write the service's xml yourself.

You can find this source code :download:`Here <example_code/binary_light.py>`
and the service's xml :download:`Here <example_code/SwitchPower-scpd.xml>`.

--------------------------------
Creating a simple Control Point
--------------------------------

This is an example of a simple Control Point, in this example we will implement
a Binary Light command line Control Point, but this may help you to create any
Control Point.
This Control Point has the following features:

* Searching for devices
* Listing devices
* Handling events (new device located, removed device)

Before everything you have to choose a reactor and then do the imports.
::

    from brisa.core.reactors import install_default_reactor
    reactor = install_default_reactor()

    from brisa.core.network import parse_url
    from brisa.core.threaded_call import run_async_function

    from brisa.upnp.control_point.control_point import ControlPoint

Done the importing, lets implement the Control Point. To do that you have to choose 
the service that you're going to use and create that callback functions that are 
going to be used.
::

    service = ('u', 'urn:schemas-upnp-org:service:SwitchPower:1')
    binary_light_type = 'urn:schemas-upnp-org:device:BinaryLight:1'


    def on_new_device(dev):
        if not dev:
            return
        print 'Got new device:', dev.udn
        print "Type 'list' to see the whole list"


    def on_removed_device(udn):
        print 'Device is gone:', udn


    def get_switch_service(device):
        return device.services[service[1]]

Now you can create the events that will call the callbacks, and create the main
method, that will initialize the Control Point.
::

    def create_control_point():
        c = ControlPoint()
        c.subscribe('new_device_event', on_new_device)
        c.subscribe('removed_device_event', on_removed_device)
        return c


    def main():
        c = create_control_point()
        c.start()
        run_async_function(_handle_cmds, (c, ))
        reactor.add_after_stop_func(c.stop)
        reactor.main()

Now the functions that are going to perform the actions by the command handler,
just like the command handler are implemented

.. literalinclude:: example_code/cmd_line_control_point.py
   :lines: 59-203

And, finally to allow you to run it, add to the end
::

    if __name__ == '__main__':
        main()

You can find this code :download:`here <example_code/cmd_line_control_point.py>`.


-------------------------------
Creating a Qt Gui Device
-------------------------------

This is very similar to create a simple device. You will need to create your
Qt Gui, create the device, create the service and use the QtReactor
::

	from brisa.core.reactors.qtreactor import QtReactor
	reactor = QtReactor()
	
	from PyQt4 import QtCore, QtGui
	from brisa.core import config
	from brisa.upnp.device import Device
	from brisa.upnp.device.service import Service, StateVariable

The constructor is going to be most part composed of GUI code, but these two lines(40-41) 
are going to be important on the Device creation.
::
  
  self.root_device = None
  self.upnp_urn = 'urn:schemas-upnp-org:device:MyDevice:1'

In the _add_root_device method you are going to initialize the root_device setting it 
from None to a Device object.
::
  
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

In the _add_services method you're going to create the Services that the Device will 
provide.
::	
  
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

The _load method is going to initialize the device and add the services.
::
  
  def _load(self):
      self._add_root_device()
      self._add_services()

These are the callbacks to the GUI commands, and will start or stop the root device.
::	
  
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

Now you can implement the main method and call it.
::
	
	def main():
	    qt_dev = QtDevice()
	    qt_dev.show()
	    reactor.main()
	
	if __name__ == '__main__':
	    main()

You can find this source code :download:`Here <example_code/simple_qt_device.py>`.


-------------------------------
Creating a Qt Gui Control Point
-------------------------------

This is very similar to create a simple control point. You will need to create your
Qt Gui, create the control point and use the QtReactor and do the correct Qt imports.
::

	from brisa.core.reactors.qtreactor import QtReactor
	reactor = QtReactor()
	
	from PyQt4 import QtCore, QtGui
	from brisa.upnp.control_point import ControlPoint

Here you're going to create the Control Point and the events (lines 16-21).
::

  self.cp = ControlPoint()
  self.cp.subscribe('new_device_event', self.on_new_device)
  self.cp.subscribe('removed_device_event', self.on_removed_device)
  self.devices = []

These methods are going to be the callback methods of the events that the Control Point 
will receive from the Device.
::

  def on_new_device(self, dev):
      if dev.udn not in [d.udn for d in self.devices]:
          self.devices.append(dev)
      self.list_devices()
	    
  def on_removed_device(self, udn):
      for dev in self.devices:
          if dev.udn == udn:
              self.devices.remove(dev)
      self.list_devices()

These methods are the methods that are going to be the callbacks to the GUI events.
::
    
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

Now you implement and call the main method.
::	

	def main():
	    qt_cp = QtControlPoint()
	    qt_cp.show()
	    reactor.main()
	
	if __name__ == '__main__':
	    main()

You can find this source code :download:`Here <example_code/simple_qt_control_point.py>`.
