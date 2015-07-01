:mod:`network` --- Network facilities
======================================

.. module: network
    :synposis: Provides network related functions, such as get_ip_address(),
    http_call(), parse_url() and others.

:ref:`python-brisa <python-brisa>` provides a network module with some functions
which will simplify networking related tasks.

Using the parse_url() function
-------------------------------
This little example shows a simple and generic use of the parse_url() function

::

    from brisa.core.network import parse_url
    url = 'http://brisa.garage.maemo.org'
    parse_url(url)

It will return a 6-tuple: (scheme, netloc, path, params, query, fragment)
which in the example above will look like this

::

    ('http', brisa.garage.maemo.org', '/', '', '', '')

Getting the ip of interfaces
--------------------------------------
When you need to get the ip of one of your interfaces you can do as following

::

    from brisa.core.network import get_ip_address, get_active_ifaces
    for interfaces in get_active_ifaces():
        get_ip_address(interfaces)

This will output strings containing the active interfaces' IP address

