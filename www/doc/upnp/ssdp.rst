:mod:`ssdp` --- SSDP Server implementation
===========================================

.. module: ssdp
    :synopsis: SSDP Server implementation which listens for devices messages and
    searches.

:ref:`python-brisa <python-brisa>` provides a SSDP (Simple Service Discovery
Protocol) module which can be used on
the control point's side or on the device's.

* Device's side - Used for annoucing the device, its embedded devices and all services.
* Control Point's side - Used for keeping record of known devices.

There's no need to use the SSDP Server directly when using the control point or
device API's.

SSDPServer class
-----------------
Implementation of a SSDP Server.

Attributes
^^^^^^^^^^^
The attribute of the SSDPServer class are:

* server_name - name of the server.
* xml_description_filename - XML description filename.
* max_age - max age parameter, default is 1800.
* receive_notify - if False, ignores notify messages.
* known_devices - dict of the known devices.
* advertised -  dict of the advertised devices.
* _callbacks - dict of the callbacks.

Methods
^^^^^^^^

.. function:: is_running()

    Returns True if the SSDPServer is running, False otherwise.

.. function:: start()

    Starts the SSDPServer.

.. function:: stop()

    Sends bye bye notifications and stops the SSDPServer.

.. function:: destroy()

    Destroys the SSDPServer.

.. function:: clear_device_list()

    Clears the device list.

.. function:: discovered_device_failed(dev)

    Device could not be fully built, so forget it.

.. function:: is_known_device(usn)

    Returns if the device with the passed usn is already known.

.. function:: subscribe(name, callback)

    Subscribes a callback for an given name event.

.. function:: unsubscribe(name, callback)

    Unsubscribes a call back for an event.

.. function:: announce_device()

    Announces the device.

.. function:: register_device(device)

    Registers a device on the SSDP Server.

.. function:: _datagram_received(data, (host, pot))

    Handles a received multicast datagram.

.. function:: _discovery_request(headers, (host, port))

    Processes the discovery requests and responds accordingly.

.. function:: _notify_received(headers, (host, port))

    Processes a presence announcement.

.. function:: _register(usn, st, location, server, cache_control, where='remote')

    Registers a service or device.

.. function:: _local_register(usn, st, location, server, cache_control)

    Registers locally a new service or device.

.. function:: _register_device(device)

    Registers a device.

.. function:: _renew_notifications()

    Renew notifications (sends a notify).

.. function:: _unregister(usn)

    Unregisters a device.

.. function:: _do_notify(usn)

    Do a notification for the usn specified.

.. function:: _do_byebye(usn)

    Do byebye notification for the usn specified.

.. function:: _callback(name, *args)

    Performs callbacks for events.

.. function:: _cleanup()

    Cleans the SSDPServer by removing known devices and internal cache.





