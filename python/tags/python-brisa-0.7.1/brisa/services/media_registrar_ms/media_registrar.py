# Licensed # Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa import log
from brisa.upnp.service import ServiceControl

log = log.getLogger('MS_MediaRegistrar')


class MS_MediaRegistrar(ServiceControl):
    namespace = ('u',
                 'urn:microsoft.com:service:X_MS_MediaReceiverRegistrar:1')

    def set_variables(self):
        self.service.set_variable(0, 'AuthorizationGrantedUpdateID', 0)
        self.service.set_variable(0, 'AuthorizationDeniedUpdateID', 0)
        self.service.set_variable(0, 'ValidationSucceededUpdateID', 0)
        self.service.set_variable(0, 'ValidationRevokedUpdateID', 0)

    def soap_IsAuthorized(self, *args, **kwargs):
        device_id = kwargs['DeviceID']
        log.info('IsAuthorized(%s)', device_id)
        return {'IsAuthorizedResponse': {'Result': 1}}

    def soap_IsValidated(self, *args, **kwargs):
        device_id = kwargs['DeviceID']
        log.info('IsValidated(%s)', device_id)
        return {'IsValidatedResponse': {'Result': 1}}

    def soap_RegisterDevice(self, *args, **kwargs):
        reg_msg = kwargs['RegistrationReqMsg']
        log.info('RegisterDevice(%s)', reg_msg)
        return {'RegisterDeviceResponse': {'RegistrationRespMsg': ' '}}
