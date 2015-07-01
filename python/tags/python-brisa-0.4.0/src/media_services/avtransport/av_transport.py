# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

#TODO: Implement the required Actions

#REQUIRED ACTIONS:
#SetAVTransportURI()     R
#GetMediaInfo()          R
#GetMediaInfo_Ext()      R
#GetTransportInfo()      R
#GetPositionInfo()       R
#GetDeviceCapabilities() R
#GetTransportSettings()  R
#Stop()                  R
#Play()                  R
#Seek()                  R
#Next()                  R
#Previous()              R

#TODO: Handle errors (transport_staus)

from twisted.python import log

from upnp import ServiceControl
from upnp import errorCode
from media_services.gst_renderer import GSTRendererMaemo, GSTRenderer
import platform

class AVTransportControl(ServiceControl):


    def __init__(self):
        self.avTransportURI = None
        self.NumberOfTracks = 0

        if platform.uname()[4].find('arm') is not -1:
            self.gst_player = GSTRendererMaemo()
        elif platform.system() == 'Linux':
            self.gst_player = GSTRenderer()

        self.urilist = {}
        self.transport_state = 'NO_MEDIA_PRESENT'
        self.transport_status= 'OK'
        self.transport_speed = 1

    def soap_SetAVTransportURI(self, *args, **kwargs):
        """This action specifies the URI of the resource to be controlled by the specified
        AVTransport instance. It is RECOMMENDED that the AVTransport service checks the MIME-type
        of the specified resource when executing this action"""

        (InstanceID, CurrentURI, CurrentURIMetaData) = args

        self.avTransportURI = CurrentURI
        self.NumberOfTracks = 1
        self.gst_player.av_uri = CurrentURI
        self.transport_state = 'STOPPED'

        log.msg('SetAVTransportURI()')

    def soap_GetMediaInfo(self, *args, **kwargs):
        """This action returns information associated with the current media of the
        specified instance; it has no effect on state."""

        log.msg('GetMediaInfo()')

        return {'GetMediaInfoResponse': {'NrTracks': str(self.NumberOfTracks), 'MediaDuration': str(self.gst_player.queryDuration()[0]),
                                                     'CurrentURI': str(self.avTransportURI),
                                                     'CurrentURIMetaData': '',
                                                     'NextURI':'', 'NextURIMetaData':'',
                                                     'PlayMedium':'NETWORK',
                                                     'RecordMedium':'',
                                                     'WriteStatus': ''}}


    def soap_GetMediaInfo_Ext(self, *args, **kwargs):
        """This action returns information associated with the current media of the specified
        instance; it has no effect on state.The information returned is identical to the information
        returned by the GetMediaInfo() action, except for the additionally returned CurrentType
        argument """

        log.msg('GetMediaInfo_Ext()')

        return {'GetMediaInfo_ExtResponse': {'CurrentType' : 'TRACK_UNAWARE',
                                                     'NrTracks': str(self.NumberOfTracks),
                                                     'MediaDuration': str(self.gst_player.queryDuration()[0]),
                                                     'CurrentURI': str(self.avTransportURI),
                                                     'CurrentURIMetaData': '',
                                                     'NextURI':'', 'NextURIMetaData':'',
                                                     'PlayMedium':'NETWORK',
                                                     'RecordMedium':'',
                                                     'WriteStatus': ''}}


    def soap_GetTransportInfo(self, *args, **kwargs):
        """This action returns information associated with the current transport state
        of the specified instance; it has no effect on state."""

        #log.msg('GetTransportInfo()')

        self.transport_state = self.gst_player.getState()

        #print self.transport_state
        #print self.transport_status

        return {'GetTransportInfoResponse': {'CurrentTransportState': str(self.transport_state),
                                                     'CurrentTransportStatus': str(self.transport_status),
                                                     'CurrentSpeed': str(self.transport_speed)}}


    def soap_GetPositionInfo(self, *args, **kwargs):
        """This action returns information associated with the current position of
        the transport of the specified instance; it has no effect on state."""

        #log.msg('GetPositionInfo()')

        dur = self.gst_player.queryDuration()
        pos = self.gst_player.queryPosition()

        duration = dur[0]

        if dur[1] == -1:
            duration = pos[0]


        abs_pos = pos[1]
        rel_pos = pos[1]

        if dur[1] > 2147483646:
            abs_pos = pos[1]*2147483646/dur[1]

        if pos[1] == -1:
            abs_pos = 0
            rel_pos = 0

        return {'GetPositionInfoResponse':{'Track':'1',
                                           'TrackDuration':'"'+str(duration)+'"',
                                           'TrackMetaData':'',
                                           'TrackURI':str(self.avTransportURI),
                                           'RelTime':'"'+str(pos[0])+'"',
                                           'AbsTime':'"'+str(pos[0])+'"',
                                           'RelCount':rel_pos,
                                           'AbsCount':abs_pos}}

    def soap_GetDeviceCapabilities(self, *args, **kwargs):
        """This action returns information on device capabilities of the specified instance,
        such as the supported playback and recording formats, and the supported quality levels for
        recording. This action has no effect on state."""

        log.msg('GetDeviceCapabilities()')

    def soap_GetTransportSettings(self, *args, **kwargs):
        """This action returns information on various settings of the specified instance, such as
        the current play mode and the current recording quality mode. This action has no
        effect on state."""

        log.msg('GetDeviceCapabilities()')

    def soap_Stop(self, *args, **kwargs):
        """This action stops the progression of the current resource that is associated with the
        specified instance."""

        log.msg('Stop()')
        self.transport_state = 'TRANSITIONING'
        self.gst_player.stop()
        self.transport_state = 'STOPPED'

    def soap_Play(self, *args, **kwargs):
        """This action starts playing the resource of the specified instance, at the specified speed,
        starting at the current position, according to the current play mode."""

        log.msg('Play()')

        self.transport_state = 'TRANSITIONING'

        self.gst_player.play()

        self.transport_state = 'PLAYING'

    def soap_Pause(self, *args, **kwargs):
        """This action starts playing the resource of the specified instance, at the specified speed,
        starting at the current position, according to the current play mode."""

        log.msg('Pause()')

        self.transport_state = 'TRANSITIONING'

        self.gst_player.pause()

        self.transport_state = 'PAUSED_PLAYBACK'

    def soap_Seek(self, *args, **kwargs):
        """This action starts seeking through the resource controlled by the specified instance - as
        fast as possible - to the position, specified in the Target argument."""

        (InstanceID, Unit, Target) = args

        self.gst_player.seek(Unit, Target)

        log.msg('Seek()')

    def soap_Next(self, *args, **kwargs):
        """This is a convenient action to advance to the next track. This action is functionally equivalent
        to Seek(TRACK_NR, CurrentTrackNr+1). This action does not cycle back to the first track."""

        log.msg('Next()')

    def soap_Previous(self, *args, **kwargs):
        """This is a convenient action to advance to the previous track. This action is functi
