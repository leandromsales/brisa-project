# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import uuid

from brisa import log
from brisa.upnp.service import ServiceControl
from brisa.upnp.message_handle.user_profile_description_message import \
    UserProfileDescriptionMessage
from brisa.upnp.message_handle.user_preference_description_message import \
    UserPreferenceDescriptionMessage
from brisa_userprofile_server.services.up_profile.persistence.database import\
User


class UPProfile(ServiceControl):

    def __init__(self):
        ServiceControl.__init__(self)

    def start(self):
        pass

    def soap_GetUserInfo(self, *args, **kwargs):
        UUID = kwargs['UUID']
        xml_profile = "Invalid UUID"
        user = User.select(User.q.uuid == UUID)[0]
        if UUID == user.uuid:
            username = user.username
            password = user.password
            profile = UserProfileDescriptionMessage(username, password)
            xml_profile = profile.dump_xml()
        return{'GetUserInfoResponse': {'User': xml_profile}}

    def soap_GetProfile(self, *args, **kwargs):
        UUID = kwargs['UUID']
        categoryID = kwargs['CategoryID']
        xml_preference = "Invalid UUID"
        user = User.select(User.q.username == username)[0]
        if UUID == user.uuid:
            username = user.username
            password = user.password
            preference = UserPreferenceDescriptionMessage(username,
                                                          password)
            xml_preference = preference.dump_xml()

        return{'GetProfileResponse': {'Preference': xml_preference}}
