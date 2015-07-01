# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

#TODO: Implement the required Actions

#REQUIRED ACTIONS:
#ListPresets() R
#SelectPreset() R

#OPTIONAL ACTIONS:
#GetVolume()
#SetVolume()

from twisted.python import log
from upnp import ServiceControl
from upnp import errorCode

class RenderingControl(ServiceControl):

    def __init__(self, gst_player):
        self.gst_player = gst_player

    def soap_ListPresets(self, *args, **kwargs):
        """This action returns a list of the currently defined presets"""

        log.msg('ListPresets')

        return {'ListPresetsResponse': {'CurrentPresetNameList': ''}}

    def soap_SelectPreset(self, *args, **kwargs):
        """ This action restores (a subset) of the state variables to
         the values associated with the specified preset."""

        log.msg('SelectPreset')

    def soap_GetVolume(self, *args, **kwargs):
        """This action retrieves the current value of the Volume state
        variable of the specified channel for the specified instance of this
        service"""

        (InstanceID, Channel) = args

        log.msg('GetVolume()')

        volume = 100

        volume = int(self.gst_player.getVolume())

        return {'GetVolumeResponse': {'CurrentVolume': volume}}

    def soap_SetVolume(self, *args, **kwargs):
        """ This action sets the Volume state variable of the specified
        instance and channel to the specified value."""
        (InstanceID, Channel, DesiredVolume) = args
        self.gst_player.setVolume(int(DesiredVolume))
        log.msg('SetVolume()')

    def get_actionDict(self):

        return None