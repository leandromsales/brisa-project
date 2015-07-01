# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Extends the base control point and adds basic Audio/Video functionality.
"""

from brisa import log
from brisa.control_point.control_point import ControlPoint
from brisa.control_point.control_point import get_service_control_url
from brisa.upnp.didl.didl_lite import Element
from brisa.upnp.soap import SOAPProxy
from brisa.utils.network import parse_url
from brisa.utils import properties


class ControlPointAV(ControlPoint):
    """This class extends ControlPoint and add basic AV functionality.
    """
    cds_ns = ('u', 'urn:schemas-upnp-org:service:ContentDirectory:1')
    avt_ns = ('u', 'urn:schemas-upnp-org:service:AVTransport:1')
    version_media_server = 'urn:schemas-upnp-org:device:MediaServer:'
    version_media_renderer = 'urn:schemas-upnp-org:device:MediaRenderer:'
    encoding = 'UTF-8'

    # Start of Properties Methods

    def _set_current_server_device(self, device):
        self._prop_current_server = device
    current_server = properties.\
                     gen_property_with_default("current_server", fset =
                                               _set_current_server_device)

    def _set_current_renderer_device(self, device):
        self._prop_current_renderer = device
    current_renderer = properties.\
                       gen_property_with_default("current_renderer", fset =
                                                 _set_current_renderer_device)

    # End of Properties Methods

    def _new_device_event_impl(self, cargo, device_object):
        # MediaServer and MediaRenderer
        url_info = parse_url(device_object.location)
        root_device = device_object.device
        service_name = None

        if self.version_media_server in root_device.device_type:
            namespace = self.cds_ns
            service_name = 'urn:schemas-upnp-org:service:ContentDirectory'
        elif self.version_media_renderer in root_device.device_type:
            namespace = self.avt_ns
            service_name = 'urn:schemas-upnp-org:service:AVTransport'

        # Create device's SOAPService if the service name is fine
        if service_name is not None and hasattr(root_device, "services"):
            control_url = get_service_control_url(root_device.services,
                                                  service_name)
            control_url = "%s://%s%s" % (url_info[0], url_info[1], \
                                        control_url)
            root_device.SOAPService = SOAPProxy(control_url, namespace)

        self.callback("new_device_event", root_device)

    # Start of Media Server Methods

    def browse(self, object_id, browse_flag, filter, starting_index,
               requested_count, sort_criteria="dc:title"):
        """Make browses in Media Server.

        This method browser the media server with udn id. The browse_flag
        must be BrowseDirectChildren or BrowseMetadata"""
        object_id = str(object_id)
        log.debug('CP browsing id %s' % object_id)

        try:
            ss = self.current_server.SOAPService
            ss.soapaction = '%s#Browse' % self.cds_ns[1]

            browse_response = ss.Browse(ObjectID=object_id,
                                        BrowseFlag=browse_flag,
                                        Filter=filter,
                                        StartingIndex=starting_index,
                                        RequestedCount=requested_count,
                                        SortCriteria=sort_criteria)
            elt = Element.from_string(browse_response['Result'])
            browse_response['Result'] = elt.get_items()
            log.debug('browse response %s' % str(browse_response['Result']))
            return browse_response
        except Exception, e:
            if not self.current_server:
                raise Exception("Media Server not selected. Select it " \
                                "setting current_server to the device " \
                                "you want.")
            else:
                raise Exception("Error during remote browse call: %s" % \
                                e.message)

    def search(self, container_id, search_criteria, filter, starting_index,
               requested_count, sort_criteria):
        """Search items in Media Server.

        This method search items with search_criteria key in the container_id
        of current media server."""
        ss = self.current_server.SOAPService
        ss.soapaction = '%s#Search' % self.cds_ns[1]

        try:
            search_response = ss.Search(ContainerID=container_id,
                                    SearchCriteria=search_criteria,
                                    Filter=filter,
                                    StartingIndex=starting_index,
                                    RequestedCount=requested_count,
                                    SortCriteria=sort_criteria)

            elt = Element.from_string(search_response['Result'])
            log.debug('search response %s' % str(browse_response['Result']))
            return elt.get_items()
        except Exception, e:
            if not self.current_server:
                raise Exception("Media Server not selected. Select it " \
                                "setting current_server to the device " \
                                "you want.")
            else:
                raise Exception("Error during search: %s" % e.message)

    def get_search_capabilites(self):
        """Return the fields which the server support search.

        This method returns the fields which the server support searching."""
        try:
            ss = self.current_server.SOAPService
            ss.soapaction = '%s#GetSearchCapabilities' % self.cds_ns[1]
            return ss.GetSearchCapabilities()
        except Exception, e:
            if not self.current_server:
                raise Exception("Media Server not selected. Select it " \
                                "setting current_server to the device " \
                                "you want.")
            else:
                raise Exception("Error during get_search_capabilities: %s" % \
                                e.message)

    def get_sort_capabilities(self):
        """Returns a list of fields which the server supports sorting.

        This method returns the fields which the server support sort."""
        try:
            ss = self.current_server.SOAPService
            ss.soapaction = '%s#GetSortCapabilities' % self.cds_ns[1]
            return ss.GetSortCapabilities()
        except Exception, e:
            if not self.current_server:
                raise Exception("Media Server not selected. Select it " \
                                "setting current_server to the device " \
                                "you want.")
            else:
                raise Exception("Error during get_sort_capabilities: %s" % \
                                e.message)

    # End of Media Server Methods

    # Start of Media Renderer Methods

    def play(self, id):
        """Play item.

        Render media_url in the Renderer which udn is stored in
        self.renderer_udn."""
        try:
            ss = self.current_renderer.SOAPService
            ss.soapaction = '%s#SetAVTransportURI' % self.avt_ns[1]
            item = self.browse(id, 'BrowseMetadata', '*', 0, 1, '')
            uri = item['Result'][0].resources[0].value
            ss.SetAVTransportURI(InstanceID=0,
                                 CurrentURI=uri,
                                 CurrentURIMetaData='')
            ss.soapaction = '%s#Play' % self.avt_ns[1]
            ss.Play()
        except Exception, e:
            if not self.current_renderer:
                raise Exception("Media renderer not selected. Select it " \
                                "setting current_renderer to the device " \
                                "you want.")
            else:
                raise Exception("Error during remote play call: %s. Maybe " \
                                "the selected renderer is not online " \
                                "anymore." % e.message)

    def stop(self):
        """Stop render.

        Stop the rendering according to the current selected renderer udn."""
        try:
            ss = self.current_renderer.SOAPService
            ss.soapaction = '%s#Stop' % self.avt_ns[1]
            ss.Stop()
        except:
            if not self.current_renderer:
                raise Exception("Media Renderer not selected. Select it " \
                                "setting current_renderer to the device " \
                                "you want.")
            else:
                raise Exception("Error occured during remote stop call")

    def pause(self):
        """Pause render.

        Pause the Renderer which udn is stored in self.renderer_udn."""
        try:
            ss = self.current_renderer.SOAPService
            ss.soapaction = '%s#Pause' % self.avt_ns[1]
            ss.Pause()
        except:
            if not self.current_renderer:
                raise Exception("Media Renderer not selected. Select it " \
                                "setting current_renderer to the device " \
                                "you want.")
            else:
                raise Exception("Error occured during remote pause call")

    def next(self):
        """Switch play next track
        """
        pass

    def previous(self):
        """Switch play to previous track
        """
        pass

    # End of Media Renderer Methods
