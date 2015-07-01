# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import uuid

from brisa import log
from brisa.upnp.service import ServiceControl
from brisa_userprofile_server.services.up_profile.persistence.database import \
User


class UPAuthentication(ServiceControl):

    namespace = ('u', 'urn:schemas-upnp-org:service:UPAuthentication:1')

    def start(self):
        pass

    def soap_Auth(self, *args, **kwargs):
        UUID = ''
        login = kwargs['Login']
        password = kwargs['Password']
        try:
            user = User.select(User.q.username == login)[0]
        except IndexError:
            # FIXME since we don't have a database yet, and a webserver profile
            # creator, the Auth method will be sort like an auto-acc
            user = User(username = login, password = password, \
                        uuid = str(uuid.uuid4()), firstName = '', \
                        lastName = '', email = '')
        if user:
            if (password == user.password):
                UUID = user.uuid
        return {'AuthResponse': {'UUID': UUID}}

    def soap_RenewSession(self, *args, **kwargs):
        return {'RenewSessionResponse': {}}

    def soap_Logout(self):
        return {'LogoutResponse': {}}
