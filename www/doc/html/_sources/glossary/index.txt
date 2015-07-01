.. _glossary-index:

##########
 Glossary
##########

This is a basic glossary recommended reading before starting reading the
documentation itself.

.. glossary::

    BRisa Project
        project under development at `Laboratory of Embedded Systems and
        Pervasive Computing <http://embedded.ufcg.edu.br>`_, financed by `INdT
        <http://www.indt.org.br>`_.

    .. _python-brisa:

    python-brisa
        a python upnp framework developed by the BRisa Project.

    brisa-applications or brisa-apps
        applications developed by the BRisa Project that uses python-brisa.

    upnp
        also known as UPnP, short for universal plug and play, protocol defined
        by the UPnP Forum. Documents can be found at `http://www.upnp.org
        <http://www.upnp.org>`_.

    device
        for our purposes, when refering to a device, we will be refering to a UPnP
        device, that is, an entity capable of joining the UPnP network (thus
        capable of performing its role on the `UPnP Architecture
        <http://upnp.org/specs/arch/UPnP-arch-DeviceArchitecture-v1.0.pdf>`_.

    service
        for our purposes, a service should be considered an UPnP service, that
        is, a service provider of actions. For example, a Content Directory
        service provides a Browse action that enables others to browse through a
        remote directory on the service location.

    control point
        an entity of the UPnP network capable of controlling devices

    reactor
        the gears or engine that powers the framework. This can be simply put analogous as
        a car engine - you can replace it by another one that provides the basic
        functionalities your car requires. python-brisa provides some engine options
        integrated with different frameworks, such as Gtk, Gobject, Qt, and so
        on.
