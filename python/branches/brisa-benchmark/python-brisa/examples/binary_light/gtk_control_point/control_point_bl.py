import brisa
brisa.__skip_service_xml__ = False

from brisa import log
from brisa.control_point.control_point import ControlPoint
from brisa.control_point.control_point import get_service_control_url
from brisa.upnp.soap import SOAPProxy
from brisa.upnp.didl.didl_lite import Element
from brisa.utils.network import parse_url
from brisa.utils import properties


class ControlPointBL(ControlPoint):
    sp_ns = ('u', 'urn:schemas-upnp-org:service:SwitchPower:1')
    version_binarylight_server = ('urn:schemas-upnp-org:device:BinaryLight')
    encoding = 'UTF-8'
    current_server = None

    def _new_device_event_impl(self, cargo, device_object):
        url_info = parse_url(device_object.location)
        root_device = device_object.device
        service_name = None
        if self.version_binarylight_server in root_device.device_type:
            namespace = self.sp_ns
            service_name = 'urn:schemas-upnp-org:service:SwitchPower:1'
            # Create device's SOAPService if the service name is fine
            if hasattr(root_device, 'services'):
                control_url = get_service_control_url(root_device.services,
                                                      service_name)
                control_url = '%s://%s%s' % (url_info[0], url_info[1], \
                                             control_url)
                root_device.SOAPService = SOAPProxy(control_url, namespace)

        self.callback('new_device_event', root_device)

    def set_target(self, target):
        try:
            ss = self.current_server.SOAPService
            ss.soapaction = '%s#SetTarget' % self.sp_ns[1]
            ss.SetTarget(newTargetValue=target)
        except Exception, e:
            if not self.current_server:
                raise Exception('BinaryLight Server not selected. Select it' \
                                'setting current_server to the device you'\
                                'want.')
            else:
                raise Exception('Error trying to retrieve information: %s' %\
                                e.message)

    def get_target(self):
        try:
            ss = self.current_server.SOAPService
            ss.soapaction = '%s#GetTarget' % self.sp_ns[1]
            target_response = ss.GetTarget()
            return True if status_response['RetTargetValue'] == '1' else False
        except Exception, e:
            if not self.current_server:
                raise Exception('BinaryLight Server not selected. Select it' \
                                'setting current_server to the device you'\
                                'want.')
            else:
                raise Exception('Error during remote authentication call: %s' \
                                % e.message)

    def get_status(self):
        try:
            ss = self.current_server.SOAPService
            self.current_server.SOAPService
            ss.soapaction = '%s#GetStatus' % self.sp_ns[1]

            status_response = ss.GetStatus()
            return True if status_response['ResultStatus'] == '1' else False
        except Exception, e:
            if not self.current_server:
                raise Exception('BinaryLight Server not selected. Select it' \
                                'setting current_server to the device you' \
                                'want.')
            else:
                raise Exception('Error during renew: %s' % e.message)
