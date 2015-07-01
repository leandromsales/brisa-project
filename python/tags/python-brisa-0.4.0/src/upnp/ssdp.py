# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.

# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006 John-Mark Gurney <gurney_j@resnet.uroegon.edu>
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

#
# Implementation of SSDP server under Twisted Python.
#
import random
import string
import sys
import time
import device
import service
from twisted.python import log
from twisted.internet.protocol import DatagramProtocol
from twisted.internet import reactor, error

SSDP_PORT = 1900
SSDP_ADDR = '239.255.255.250'


class SSDPServer(DatagramProtocol):
    """A class implementing a SSDP server.  The notifyReceived and
    searchReceived methods are called when the appropriate type of
    datagram is received by the server."""
    # not used yet
    stdheaders = [('Server', 'Twisted, UPnP/1.0, python-upnp'), ]
    elements = {}
    known = {}
    own = {}
    _callbacks = {}

    def __init__(self, enable_log, serverName):
        # Create SSDP server
        port = reactor.listenMulticast(SSDP_PORT, self, listenMultiple=True)
        self.server = serverName

        if enable_log:
            log.startLogging(sys.stdout)
        # don't get our own sends
        port.setLoopbackMode(0)
        port.joinGroup(SSDP_ADDR)

    def _failure(self, error):
        print error

    def datagramReceived(self, data, (host, port)):
        """Handle a received multicast datagram."""
        try:
            header, payload = data.split('\r\n\r\n')
        except ValueError, err:
            print err
            print 'Arggg,', data
            import pdb
            pdb.set_trace()

        lines = header.split('\r\n')
        cmd = string.split(lines[0], ' ')
        lines = map(lambda x: x.replace(': ', ':', 1), lines[1:])
        lines = filter(lambda x: len(x) > 0, lines)

        headers = [string.split(x, ':', 1) for x in lines]
        headers = dict(map(lambda x: (x[0].lower(), x[1]), headers))

        if cmd[0] == 'M-SEARCH' and cmd[1] == '*':
            # SSDP discovery
            self.discoveryRequest(headers, (host, port))
            #pass
        elif cmd[0] == 'NOTIFY' and cmd[1] == '*':
            # SSDP presence
            self.notifyReceived(headers, (host, port))
        else:
            log.msg('Unknown SSDP command %s ' % cmd)

    def register(self, usn, st, location, server, cache_control):
        """Register a service or device that this SSDP server will
        respond to."""
        log.msg('Registering %s (%s)' % (st, location))
        self.known[usn] = {}
        self.known[usn]['USN'] = usn
        self.known[usn]['LOCATION'] = location
        self.known[usn]['ST'] = st
        self.known[usn]['EXT'] = ''
        self.known[usn]['SERVER'] = server
        self.known[usn]['CACHE-CONTROL'] = cache_control

        if st == 'upnp:rootdevice':
            self.callback("new_device", st, self.known[usn])

    def localRegister(self, usn, st, location, server, cache_control):
        log.msg('Registering locally %s (%s)' % (st, location))
        self.own[usn] = {}
        self.own[usn]['USN'] = usn
        self.own[usn]['LOCATION'] = location
        self.own[usn]['ST'] = st
        self.own[usn]['EXT'] = ''
        self.own[usn]['SERVER'] = server
        self.own[usn]['CACHE-CONTROL'] = cache_control

        if st == 'upnp:rootdevice':
            self.callback("new_device", st, self.own[usn])

        self.doNotify(usn)

    def registerRootDevice(self,rootDevice):
        if (rootDevice.is_rootDevice()):
            #Dont have embedded devices
            deviceId = rootDevice.get_udn()
            deviceType = rootDevice.get_device_type_version()
            deviceServer = "Twisted, UPnP/1.0, " + self.server
            deviceLocation = rootDevice.get_location() + '/root-device.xml'
            self.localRegister('%s::upnp:rootdevice' % deviceId,
                               'upnp:rootdevice', deviceLocation,
                               deviceServer, 'max-age=1800')
            self.localRegister(deviceId, deviceId, deviceLocation,
                                   deviceServer, 'max-age=1800')
            self.localRegister(deviceId + "::" + deviceType, deviceType,
                               deviceLocation, deviceServer, 'max-age=1800')

            for service in rootDevice.get_services():
                serviceType = service.get_service_type_version()
                self.localRegister(deviceId + "::" + serviceType, serviceType,
                                   deviceLocation, deviceServer,
                                   'max-age=1800')

        else:
            #has embedded devices
            #TODO: implement-me
            pass
        #print self.own

    def announceDevice(self, device):
        #TODO: check renew annoucemments of upnp

        for usn in self.known:
            self.doNotify(usn)

    def unRegister(self, usn):
        log.msg("Un-registering %s" % usn)

        st = self.known[usn]['ST']
        if st == 'upnp:rootdevice':
            self.callback("removed_device", st, self.known[usn])

        del self.known[usn]

    def isKnown(self, usn):
        #return self.known.has_key(usn)
        return (usn in self.known)

    def notifyReceived(self, headers, (host, port)):
        """Process a presence announcement.  We just remember the
        details of the SSDP service announced."""

        if headers['nts'] == 'ssdp:alive':
            if not self.isKnown(headers['usn']):
                self.register(headers['usn'],
                              headers['nt'],
                              headers['location'],
                              headers['server'],
                              headers['cache-control'])
        elif headers['nts'] == 'ssdp:byebye':
            if self.isKnown(headers['usn']):
                self.unRegister(headers['usn'])
        else:
            log.msg('Unknown subtype %s for notification type %s' % \
                    (headers['nts'], headers['nt']))

    def subscribe(self, name, callback):
        self._callbacks.setdefault(name,[]).append(callback)

    def unsubscribe(self, name, callback):
        callbacks = self._callbacks.get(name,[])
        if callback in callbacks:
            callbacks.remove(callback)
        self._callbacks[name] = callbacks

    def callback(self, name, *args):
        for callback in self._callbacks.get(name,[]):
            callback(*args)

    def doNotify(self, st):
        """Do notification"""
        log.msg('Sending alive notification for %s' % st)
        resp = ['NOTIFY * HTTP/1.1',
            'Host: %s:%d' % (SSDP_ADDR, SSDP_PORT),
            'NTS: ssdp:alive', ]
        stcpy = dict(self.own[st].iteritems())
        stcpy['NT'] = stcpy['ST']
        del stcpy['ST']
        resp.extend(map(lambda x: ': '.join(x), stcpy.iteritems()))
        resp.extend(('', ''))
        self.transport.write('\r\n'.join(resp), (SSDP_ADDR, SSDP_PORT))

    def discoveryRequest(self, headers, (host, port)):
        """Process a discovery request.  The response must be sent to
        the address specified by (host, port)."""

        log.msg('Discovery request for %s' % headers['st'])
        rightKey = 0

        # Do we know about this service?
        if headers['st'] == 'ssdp:all':
            for i in self.own.keys():
                hcopy = dict(headers.iteritems())
                hcopy['st'] = self.own[i]['ST']
                self.discoveryRequest(hcopy, (host, port))
            return None

        for key in self.own.keys():
            if self.own[key]['ST'].split()[0] == (headers['st']).split()[0]:
                rightKey = key
                break
        else:
            log.msg('Discovery request ST %s not found' % headers['st'])
            return

        if rightKey == 0:
            log.msg('Unknown Error in Discovery request for %s' % \
                        headers['st'])
            return

        # Generate a response
        response = []
        response.append('HTTP/1.1 200 OK')

        for k, v in self.own[rightKey].items():
            response.append('%s: %s' % (k, v))

        response.exten
