# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import uuid

from brisa import log
from brisa.upnp.service import ServiceControl
from brisa.services.up_profile.persistence.database import User


class UPAuthentication(ServiceControl):

    def __init__(self):
        ServiceControl.__init__(self)

    def start(self):
        pass

    def soap_Auth(self, *args, **kwargs):
        self.UUID = "Invalid User"

        login = kwargs['Login']
        password = kwargs['Password']

        count = 1

        user = User.select()
        user_number = user.count()
        while count <= user_number:
            if (login == User.get(count).username) \
               and (password == User.get(count).password):
                self.UUID = 'uuid:%s' % uuid.uuid4()
                User.get(count).set(uuid = self.UUID)
            count += 1

        return{'AuthResponse': {'UUID': self.UUID}}

    def soap_RenewSession(self, *args, **kwargs):
        return{'RenewSessionResponse': {}}

    def soap_Logout(self):
        return{'LogoutResponse': {}}
