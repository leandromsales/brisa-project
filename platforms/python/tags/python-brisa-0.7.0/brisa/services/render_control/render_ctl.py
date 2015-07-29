# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa import log
from brisa.upnp.service import ServiceControl


class RenderingControl(ServiceControl):

    namespace = ('u', 'urn:schemas-upnp-org:service:RenderingControl:1')

    def __init__(self, gst_player):
        ServiceControl.__init__(self)
        self.gst_player = gst_player

    def soap_ListPresets(self, *args, **kwargs):
        """Return List of currently defined.

        This action returns a list of the currently defined presets"""

        log.info('ListPresets')

        return {'ListPresetsResponse': {'CurrentPresetNameList': ''}}

    def soap_SelectPreset(self, *args, **kwargs):
        """Select Present state variables.

        This action restores (a subset) of the state variables to
        the values associated with the specified preset."""

        log.info('SelectPreset')

    def soap_GetVolume(self, *args, **kwargs):
        """Return the current volume state.

        This action retrieves the current value of the Volume state
        variable of the specified channel for the specified instance of this
        service"""

        (instance_id, channel) = args

        log.info('GetVolume()')

        volume = 100

        volume = int(self.gst_player.get_volume())

        return {'GetVolumeResponse': {'CurrentVolume': volume}}

    def soap_SetVolume(self, *args, **kwargs):
        """Set volume of instance and chanel.

        This action sets the Volume state variable of the specified
        instance and channel to the specified value."""
        (instance_id, channel, desired_volume) = args
        self.gst_player.set_volume(int(desired_volume))
        log.info('SetVolume()')

        return {'SetVolumeResponse': {}}
