:mod:`config` --- Configuration facilities
==========================================

.. module: config
    :synopsis: Configuration Facility

:ref:`python-brisa <python-brisa>` provides a very simple configuration API and
a built-in configuration manager.

Using the Built-In Configuration Manager
----------------------------------------
::

    from brisa.core.config import manager

    # Retrieving the value of some_section.some_parameter_name
    print manager.get_parameter('some_section', 'some_parameter_name')

    # Setting some_section.some_parameter_name = 1
    manager.set_parameter('some_section', 'some_parameter_name', 1)

The default manager contains a default section for framework settings. It is
called *brisa* and you can play with the manager's methods on it (e.g.
``manager.contains()``, ``manager.items()``, and so on).

Direct Access Option
--------------------

When activated, the direct access option makes the manager perform changes
(e.g. ``manager.set_parameter()``) directly on the configuration static storage. If
not activated, all changes made during runtime will be lost if you don't
explicitly call ``manager.save()``.

Direct Access can be activated with ``manager.set_direct_access(True)`` and
deactivated the same way, passing False as argument.


Configuration Tool
------------------

There's a command-line configuration tool called ``brisa-conf``. It can list
sections, add/remove sections, set parameters, clean up configuration.

For help on how to use it, try ``brisa-conf --help``. It will give you detailed
information on how to use it.

We also have a few examples illustrated below.

Listing all sections::

        brisa-conf -l

Listing items of section ``brisa``: ::

        brisa-conf -i brisa

Setting logging output to stdout::

        brisa-conf -s brisa -p logging_output stdout

Setting logging level to DEBUG::

        brisa-conf -s brisa -p logging DEBUG
