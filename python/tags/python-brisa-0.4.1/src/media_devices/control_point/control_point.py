# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

#1. Discovery UPnP devices (MSs and MRs)
#2. get xmls descripting CDS e connmgr
#3. Create these services and devices instances

from upnp.msearch import MSearch
from upnp.ssdp import SSDPServer
from twisted.internet import reactor

class ControlPoint(object):

    msearch=None
    mySSDP=None
    ssdp_know={}
    services=[]

    def __init__(self, name="Brisa Control Point"):
        self.mySSDP = SSDPServer(False, "Brisa Media Control Point SSDP")
        self.msearch = MSearch(self.mySSDP)
        self.ssdp_know=self.mySSDP.known

    def add_service(self,service):
        #MOVE to device
        self.services.append(service)

    def discover_devices(self):
        self.msearch.discover()

    def discover_media_server_devices(self):
        self.msearch.discover("urn:schemas-upnp-org:device:MediaServer:1")

    def discover_media_renderer_devices(self):
        self.msearch.discover("urn:schemas-upnp-org:device:MediaRenderer:1")
