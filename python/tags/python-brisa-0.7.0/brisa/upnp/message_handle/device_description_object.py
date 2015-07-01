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


class DeviceDescriptionObject(object):

    def __init__(self, location, filename=None):
        self.location = location
        self.filename = filename

    def to_object(self):
        if self.filename is None:
            filecontent = url_fetch(self.location)
            if not filecontent:
                return
            tree = ElementTree(file=filecontent).getroot()
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
            self.to_object_async_gotdata(self, open(self.filename).read())

    def to_object_async_error(self, cargo, error):
        log.debug("Error fetching %s - Error: %s" % (self.location,
                                                     str(error)))
        return True

    def to_object_async_gotdata(self, fd, cargo=None):
        try:
            log.debug('to object async got data getting tree')
            tree = ElementTree(file=fd).getroot()
        except Exception, e:
            log.debug("Bad device XML %s" % e.message)
            return
        devobj = DeviceObject(self.location, tree)
        devobj.create_object()
        self.to_object_async_gotdevice(devobj)

    def to_object_async_gotdevice(self, devobj):
        self.callback(self.cargo, devobj)
