# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" A higher level interface of DeviceObject that abstracts the user from using
DeviceObject directly.
"""

from xml.etree.ElementTree import ElementTree

from brisa.upnp.message_handle.device_object import DeviceObject
from brisa.utils.network import url_fetch
from brisa.threading import run_async_call
from brisa import log
import brisa


class DeviceDescriptionObject(object):

    def __init__(self, location, filename=None):
        self.location = location
        self.filename = filename

    def to_object(self):
        if self.filename is None:
            filecontent = url_fetch(self.location)
            if not filecontent:
                return
            data = filecontent.read()
            data = data[data.find("<"):data.rfind(">")+1]
            tree = ElementTree(data).getroot()
        else:
            from xml.etree.ElementTree import parse
            tree = parse(self.filename)

        return (DeviceObject(self.location, tree)).create_object()

    def to_object_async(self, callback, cargo):
        self.callback = callback
        self.cargo = cargo
        log.debug('self.location is %s' % self.location)

        if self.filename is None:
            run_async_call(url_fetch,
                           success_callback=self.to_object_async_gotdata,
                           error_callback=self.to_object_async_error, delay=0,
                           url=self.location)
        else:
            self.to_object_async_gotdata(self, open(self.filename))

    def to_object_async_error(self, cargo, error):
        log.debug("Error fetching %s - Error: %s" % (self.location,
                                                     str(error)))
        self.callback(self.cargo, None)
        return True

    def to_object_async_gotdata(self, fd, cargo=None):
        try:
            log.debug('to object async got data getting tree')
            tree = ElementTree(file=fd).getroot()
        except Exception, e:
            log.debug("Bad device XML %s" % e.message)
            self.callback(self.cargo, None)
            return

        self.devobj = DeviceObject(self.location, tree)
        self.devobj.create_object()
        if brisa.__skip_service_xml__:
            self.callback(self.cargo, self.devobj)
        else:
            log.debug("Fetching device services")
            self.pending_services = len(self.devobj.device.services)
            for service_name, service_object in \
                     self.devobj.device.services.items():
                service_object.build_async(self.service_has_been_built)

    def service_has_been_built(self, built_ok):
        log.debug("Service fetched, %d left, result %s", \
                self.pending_services - 1, built_ok)
        if not built_ok and not brisa.__tolerate_service_parse_failure__:
            log.debug("Device killed")
            self.devobj = None
        self.pending_services -= 1

        if self.pending_services <= 0:
            log.debug("All services fetched, sending device forward")
            self.callback(self.cargo, self.devobj)
