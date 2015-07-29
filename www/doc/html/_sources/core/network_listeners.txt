:mod:`network_listeners` --- Network listening facilities
==========================================================

.. module: network_listeners
    :synopsis: Provides a simple API and observer model for listening over UDP

:ref:`python-brisa <python-brisa>` provides a network listening module which
will make it easy to implement, for example, a udp server.

Implementing a simple udp server.
----------------------------------
Look how simple it's to implement a udp server

::

    from brisa.core.reactors import install_default_reactor
    reactor = install_default_reactor()

    from brisa.core.network_listeners import UDPListeners

    def forward_data(data, addr=''):
        # This is the data handler function, in this example it will simply
        # print the received data and the address from where it came
        print data, 'from ', addr

    server = UDPListener('239.255.255.250',
                         port,
                         'interface',
                         data_callback = foward_data)

    # Be sure to register the UDPListener's stop function before using the
    # start method, or the program doesn't end.
    server.start()
    reactor.add_after_stop_func(server.stop)
    reactor.main()

This is all it takes to implement a simple udp server
