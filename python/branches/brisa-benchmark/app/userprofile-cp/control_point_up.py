from brisa import log
from brisa.control_point.control_point import ControlPoint
from brisa.control_point.control_point import get_service_control_url
from brisa.upnp.soap import SOAPProxy
from brisa.upnp.didl.didl_lite import Element
from brisa.utils.network import parse_url
from brisa.upnp.didl.didl_lite import Element
from brisa.utils import properties


class ControlPointUP(ControlPoint):
    aut_ns = ('u', 'urn:schemas-upnp-org:service:UPAuthentication:1')
    upp_ns = ('u', 'urn:schemas-upnp-org:service:UPProfile:1')
    version_userprofile_server = ('urn:schemas-upnp-org:device:UPServer')
    encoding = 'UTF-8'
    count = 0

    def _set_current_server_device(self, device):
        self._prop_current_server = device
        self.current_server = properties.\
                         gen_property_with_default('current_server', fset =
                                                   _set_current_server_device)

    def _new_device_event_impl(self, cargo, device_object):
        if device_object == None:
            pass
        else:
            url_info = parse_url(device_object.location)
            root_device = device_object.device
            service_name = None
            if self.version_userprofile_server in root_device.device_type:
                for t in map(None, [self.aut_ns[1], self.upp_ns[1]], \
                ['urn:schemas-upnp-org:service:UPAuthentication', \
                'urn:schemas-upnp-org:service:UPProfile']):
                    if t is not None and hasattr(root_device, 'services'):
                        control_url =\
                        get_service_control_url(root_device.services, t[1])
                        control_url = '%s://%s%s' % (url_info[0], url_info[1],\
                                                     control_url)
                    root_device.services[t[0]].SOAPService =\
                    SOAPProxy(control_url, t[0])
                root_device.SOAPService = SOAPProxy(control_url, t[0])
            self.callback('new_device_event', root_device)

    def GetUserInfo(self, _UUID):
        try:
            ss = self.current_server.services[self.upp_ns[1]].SOAPService
            ss.soapaction = '%s#GetUserInfo' % self.upp_ns[1]
            info_response = ss.GetUserInfo(UUID = _UUID)
            return info_response['User']
        except Exception, e:
            if not self.current_server:
                raise Exception('UserProfile Server not selected. Select it' \
                                'setting current_server to the device you'\
                                'want.')
            else:
                raise Exception('Error trying to retrieve information: %s' %\
                                e.message)

    def Auth(self, login, password):
        """
        """
        try:
            ss = self.current_server.services[self.aut_ns[1]].SOAPService
            ss.soapaction = '%s#Auth' % self.aut_ns[1]
            auth_response = ss.Auth(Login = login,
                                    Password = password)
            return auth_response['UUID']
        except Exception, e:
            if not self.current_server:
                raise Exception('UserProfile Server not selected. Select it' \
                                'setting current_server to the device you'\
                                'want.')
            else:
                raise Exception('Error during remote authentication call: %s' %
                                e.message)

    def renewsession(self):
        try:
            ss = self.current_server.services[self.aut_ns[1]].SOAPService
            ss.soapaction = '%s#RenewSession' % self.aut_ns[1]
            rs_response = ss.RenewSession()
            elt = Element.from_string(rs_response['Result'])
            rs_response['Result'] = elt.get_items()
            return rs_response
        except Exception, e:
            if not self.current_server:
                raise Exception('UserProfile Server not selected. Select it' \
                                'setting current_server to the device you' \
                                'want.')
            else:
                raise Exception('Error during renew: %s' % e.message)

    def logout(self):
        try:
            ss = self.current_server.services[self.aut_ns[1]].SOAPService
            ss.soapaction = '%s#Logout' % self.aut_ns[1]
            lg_response = ss.Logout()
            elt = Element.from_string(lg_response['Result'])
            lg_response['Result'] = elt.get_items()
            return lg_response
        except Exception, e:
            if not self.current_server:
                raise Exception('UserProfile Server not selected. Select it' \
                                'setting current_server to the device you' \
                                'want.')
            else:
                raise Exception('Error during logout: %s' % e.message)
