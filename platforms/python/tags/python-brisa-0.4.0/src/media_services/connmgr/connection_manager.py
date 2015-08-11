# Licensed # Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>
#
# This module implements the Connection Manager service
# type as documented in the ConnectionManager:1 Service Template
# Version 1.01
# It is implemented the features of MediaServer and only the
# required actions
#

from twisted.python import log

from upnp import ServiceControl
from upnp import errorCode

class ConnectionManagerControl(ServiceControl):

    def __init__(self, *args, **kwargs):
        # Nothing to be done yet
        pass


    def soap_GetProtocolInfo(self, *args, **kwargs):
        """Required: Returns the protocol-related info that this ConnectionManager
         supports in its current state"""

        log.msg('GetProtocolInfo()')
        return { 'GetProtocolInfoResponse': { 'Source': 'http-get:*:*:*', 'Sink': '' }}


    def soap_GetCurrentConnectionIDs(self, *args, **kwargs):
        """Required: Returns a comma-separated list of ConnectionIDs of
        currently ongoing Connections."""

        log.msg('GetCurrentConnectionIDs()')
        #If optional action PrepareForConnection is not implemented
        #this state variable should be set to 0.
        return {'GetCurrentConnectionIDsResponse': {'ConnectionIDs': '0'}}

    def soap_GetCurrentConnectionInfo(self, *args, **kwargs):
        """Required: Returns associated information of the connection referred to by
        the ConnectionID parameter."""

        log.msg('GetCurrentConnectionInfo()')
        #connection information can be retrieved for ConnectionID 0
        return {'GetCurrentConnectionInfoResponse': {'RcsID': '-1', 'AVTransportID': '-1',
                                                     'ProtocolInfo':'', 'PeerConnectionManager':'',
                                                     'PeerConnectionID':'-1', 'Direction':'Output',
                                                     'Status':'OK'}}
    def get_actionDict(self):

        return None

class ConnectionManagerRendererControl(ServiceControl):

    def __init__(self):
        #ConnectionManagerControl.__init__(self)
        pass

    def soap_GetProtocolInfo(self, *args, **kwargs):
        """Required: Returns the protocol-related info that this ConnectionManager
         supports in its current state - Specific for MediaRenderer Devices"""

        log.msg('GetProtocolInfo()')
        return { 'GetProtocolInfoResponse': {'Source' : '', 'Sink': 'http-get:*:audio/mpeg:*'}}

    def soap_GetCurrentConnectionInfo(self, *args, **kwargs):
        """Required: Returns associated information of the connection referred to by
        the ConnectionID parameter."""

        log.msg('GetCurrentConnectionInfo()')
        #connection information can be retrieved for ConnectionID 0
        return {'GetCurrentConnectionInfoResponse': {'RcsID': '0', 'AVTransportID': '0',
                                                     'ProtocolInfo':'', 'PeerConnectionManager':'',
                                                     'PeerConnectionID':'-1', 'Direction':'Input',
                                                     'Status':'OK'}}

    def soap_GetCurrentConnectionIDs(self, *args, **kwargs):
        """Required: Returns a comma-separated list of ConnectionIDs of
        currently ongoing Connections."""

        log.msg('GetCurrentConnectionIDs()')
        #If optional action PrepareForConnection is not implemented
        #this state variable should be set to 0.
        return {'GetCurrentConnectionIDsResponse': {'ConnectionIDs': '0'}}

    def get_actionDict(self):

        return None
