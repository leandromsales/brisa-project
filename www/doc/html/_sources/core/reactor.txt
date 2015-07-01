:mod:`reactor` --- Reactor facilities
=====================================

.. module: reactor
    :synopsis: Reactor Facility, gears of python-brisa


Choosing a Reactor
------------------

**The first thing that may be done on a python-brisa application is to setup the
framework gears, that is, choose a reactor.**

The choice of a reactor is very simple and depends mainly on your application.
If your application requires somehow Gtk, Qt, ecore or even gobject, you should
choose the reactor that attends that demand. Besides having a **default**
python-pure reactor, :ref:`python-brisa <python-brisa>` implements:

* glib/gobject
* gtk2
* ecore
* Qt

If your application do not demand any of these libraries/frameworks, then you
should probably use the default reactor. The next section explains how to
install the default reactor and subsequent sections explains how to use the
reactor.

Default Reactor
---------------

Installing the default reactor::

    from brisa.core.reactors import install_default_reactor
    reactor = install_default_reactor()

Once installed, the reactor cannot be replaced during the same runtime.

Retrieving a Installed Reactor
------------------------------

Once installed, the reactor can be retrieved the same way it was installed (by
calling install_default_reactor() or e.g. Gtk2Reactor()). The reactor will be installed on 
module *brisa.core.reactor* and can also be retrieved with:

``from brisa.core import reactor``


Gtk2Reactor Example
-------------------

For instance, if your application contains a Gtk GUI, you must install the Gtk2Reactor,
action illustrated below::

    from brisa.core.reactors import Gtk2Reactor
    reactor = Gtk2Reactor()

The most important thing here is to notice that once installed, this reactor
will use Gtk internally, so your old calls to ``gtk.main()``, ``gtk.main_quit()`` will
be equal to ``reactor.main()``, ``reactor.main_quit()``, respectivelly.

For the sake of organization, we recommend you to use the reactor interface
(e.g. use ``reactor.main()`` instead of ``gtk.main()``). It's possible the reactor
doesn't work properly if you don't use the interface.


QtReactor Example
-----------------

If your application contains a Qt GUI or a Qt Core, you must install the QtReactor,
action illustrated below::

    from brisa.core.reactors.qtreactor import QtReactor
    reactor = QtReactor()

This reactor will use Qt internally, so remember to call ``reactor.main()`` and 
``reactor.main_quit()`` as we did with Gtk2Reactor. Take a look at 
**code examples** to see a Qt device and a Qt control point example.

The concepts illustrated above are the same for the other reactors.

Advanced Reactor Usage
----------------------

The reactor interface defines that reactors must support file descriptors
event handling and timers.

File descriptors event handlers and timers can be added before starting the
reactor (e.g. ``reactor.main()``) and during runtime. Though, they will not
work if the reactor hasn't been started.

Creating a Timer
----------------

For creating a timer, use the reactor's method ``add_timer()``. By providing the
interval to wait between calls to the function, you will receive an unique id
for the timer. Use it later for removing it from the reactor::

    from brisa.core import reactor

    # Adds a timer
    timer_id = reactor.add_timer(interval, my_function)
    (...)

    # Removes the timer given it's id
    reactor.rem_timer(timer_id)
    (...)

Creating an Event Handler
-------------------------

For creating an event handler, use the reactor's method ``add_fd()``. By providing
a fd file object, a callback for the event and a type for watch on the event, you
will get an unique handler. Use this handler later for removing the event
handler from the reactor with the ``rem_fd()`` method.

Whenever an event of the type specified (read, write or exception) happens on
the file, your callback will be called with the following format: ``callback(file, event_type)``.

The available *type* flags are:

* EVENT_TYPE_READ
* EVENT_TYPE_WRITE
* EVENT_TYPE_EXCEPTION

Multiple events can be watched by passing an or combination of event types on
the *type* parameter (e.g. ``type=EVENT_TYPE_READ|EVENT_TYPE_WRITE`` for watching
both read and write events)::

    from brisa.core import reactor
    from brisa.core.ireactor import EVENT_TYPE_READ

    def on_ready_to_read(fd, type):
        print 'I read: ', fd.read()

    # Add my fd
    fd_handler = reactor.add_fd(myfd, on_ready_to_read, EVENT_TYPE_READ);

    (...)

    reactor.rem_fd(fd_handler)
