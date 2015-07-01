# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa import log
from brisa.upnp.service import ServiceControl
from brisa.upnp.message_handle.user_profile_description_message import \
    UserProfileDescriptionMessage
from brisa.upnp.message_handle.user_preference_description_message import \
    UserPreferenceDescriptionMessage
import uuid
from brisa.services.up_profile.persistence.database import User


class UPProfile(ServiceControl):

    def __init__(self):
        ServiceControl.__init__(self)

    def start(self):
        pass

    def soap_GetUserInfo(self, *args, **kwargs):
        UUID = kwargs['UUID']
        xml_profile = "Invalid UUID"

        count = 1

        user = User.select()
        user_number = user.count()

        while count <= user_number:
            if UUID == User.get(count).uuid:
                username = User.get(count).username
                password = User.get(count).password
                profile = UserProfileDescriptionMessage(username, password)
                xml_profile = profile.dump_xml()
            count += 1

        return{'GetUserInfoResponse': {'User': xml_profile}}

    def soap_GetProfile(self, *args, **kwargs):
        UUID = kwargs['UUID']
        categoryID = kwargs['CategoryID']
        xml_preference = "UUID Invalido"

        count = 1

        user = User.select()
        user_number = user.count()

        while count <= user_number:
            if UUID == User.get(count).uuid:
                username = User.get(count).username
                password = User.get(count).password
                preference = UserPreferenceDescriptionMessage(username,
                                                               password)
                xml_preference = preference.dump_xml()
            count += 1

        return{'GetProfileResponse': {'Preference': xml_preference}}
