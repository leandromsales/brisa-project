# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import os
import mimetypes

import pygst
import time
pygst.require('0.10')
import gst
import gobject
import time
from twisted.python import log
from twisted.internet import reactor

loop = gobject.MainLoop()


class GSTRenderer(object):

    def __init__(self):

        self.buildPipeline()
        self.__av_uri = None
        self.time_format = gst.Format(gst.FORMAT_TIME)
        self.player_state = 0

        #  0-Stopped, 1- Playing, 2-Paused
    def getState(self):

        if self.player_state == 0:
            return 'STOPPED'
        if self.player_state == 1:
            return 'PLAYING'
        if self.player_state == 2:
            return 'PAUSED_PLAYBACK'

    def __setURI(self, uri):

        self.player.set_property('uri', uri)
        self.__av_uri = uri

    def __getURI(self):

        return self.__av_uri

    av_uri = property(__getURI, __setURI)

    def buildPipeline(self):

        self.player = gst.element_factory_make("playbin", "player")

        bus = self.player.get_bus()
        bus.add_signal_watch()
        bus.connect('message', self.onMessage)

        self.player.set_state(gst.STATE_READY)

    def onMessage(self, bus, message):
        t = message.type
        if t == gst.MESSAGE_EOS:
            self.player.set_state(gst.STATE_NULL)
            self.player_state = 0

        elif t == gst.MESSAGE_ERROR:
            self.player.set_state(gst.STATE_NULL)
            self.player_state = 0

    def play(self):

        if self.av_uri is not None:
            if (self.player.set_state(gst.STATE_PLAYING) == \
                gst.STATE_CHANGE_FAILURE):
                log.msg("error here")
            self.player_state = 1
        else:
            log.msg("you need to set the uri first")

    def stop(self):

        if (self.player.set_state(gst.STATE_READY) == \
            gst.STATE_CHANGE_FAILURE):
            log.msg("error here: stopping")
        self.player_state = 0

    def pause(self):

        if (self.player.set_state(gst.STATE_PAUSED) == \
            gst.STATE_CHANGE_FAILURE):
            log.msg("error here : pausing")
        self.player_state = 2

    def seek(self):

        if unit == "ABS_TIME":
            target_time = self.convert_int(target)
            self.player.seek_simple(self.time_format, \
                                    gst.SEEK_FLAG_FLUSH, target_time)

        if unit == "REL_TIME":
            target_time = self.convert_int(target)
            cur_pos = self.queryPosition()[1]
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, \
                                    target_time+cur_pos)

        if unit == "ABS_COUNT":
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, \
                                    target)

        if unit == "REL_COUNT":
            cur_pos = self.queryPosition()[1]
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, \
                                    target + cur_pos)

    def setVolume(self, volume):

        self.player.set_property("volume", volume/10)

    def getVolume(self):

        return int(self.player.get_property("volume")*10)

    def queryDuration(self):

        time.sleep(0.3)
        try:
            dur_int = self.player.query_duration(self.time_format, None)[0]
            dur_str = self.convert_ns(dur_int)
        except:
            dur_int = -1
            dur_str = ''

        return dur_str, dur_int

    def queryPosition(self):

        try:
            pos_int = self.player.query_position(self.time_format, None)[0]
            pos_str = self.convert_ns(pos_int)
        except:
            pos_int = -1
            pos_str = ''

        return pos_str, pos_int

    def convert_ns(self, time_int):
        time_int = time_int / 1000000000
        time_str = ""
        _hours = "00"
        if time_int >= 3600:
            _hours = time_int / 3600
            time_int = time_int - (_hours * 3600)
        time_str = str(_hours) + ":"
        if time_int >= 600:
            _mins = time_int / 60
            time_int = time_int - (_mins * 60)
            time_str = time_str + str(_mins) + ":"
        elif time_int >= 60:
            _mins = time_int / 60
            time_int = time_int - (_mins * 60)
            time_str = time_str + "0" + str(_mins) + ":"
        else:
            time_str = time_str + "00:"
        if time_int > 9:
            time_str = time_str + str(time_int)
        else:
            time_str = time_str + "0" + str(time_int)

        if time_str == "00:00:0-1":
            time_str=""

        return time_str

    def convert_int(self, time_str):

        time_str = time_str.strip('")( ')

        (hours, min, sec) = time_str.split(":")

        time_int = int(hours)*3600 + int(min)*60 + int(sec)

        time_int = time_int*1000000000
        return time_int

class GSTRendererMaemo(object):

    def __init__(self):

        self.buildElements()
        self.__av_uri = None
        self.time_format = gst.Format(gst.FORMAT_TIME)
        self.player_state = 0
        self.current_volume = 0

        #  0-Stopped, 1- Playing, 2-Paused
    def buildElements(self):

        self.player = gst.Pipeline()
        self.filesrc = gst.element_factory_make("gnomevfssrc", "filesrc")
        self.typefinder= gst.element_factory_make("typefind", "typefinder")
        self.typefinder.connect("have-type", self.__cb_typefound)
        self.player.add(self.filesrc, self.typefinder)

        gst.element_link_many(self.filesrc, self.typefinder)

        bus = self.player.get_bus()
        bus.add_signal_watch()
        bus.connect('message', self.onMessage)

        self.mp3audiosink = gst.element_factory_make("dspmp3sink")
        self.mp3audioqueue = gst.element_factory_make("queue")
        self.mp3audiobin = gst.Bin('mp3bin')
        self.mp3audiobin.add(self.mp3audioqueue, self.mp3audiosink)

        self.videosink = gst.element_factory_make("xvimagesink", 'videosink')

        self.hantrovideodec = gst.element_factory_make("hantro4100dec")
        self.hantrovideoqueue = gst.element_factory_make("queue")
        self.hantrovideobin = gst.Bin('hantrobin')
        self.hantrovideobin.add(self.hantrovideoqueue, self.hantrovideodec)

        self.mpegvideodec = gst.element_factory_make("ffdec_mpegvideo")
        self.mpegvideoqueue = gst.element_factory_make("queue")
        self.mpegvideobin = gst.Bin('mpegbin')
        self.mpegvideobin.add(self.mpegvideoqueue, self.mpegvideodec)

        self.aacaudiosink = gst.element_factory_make("dspaacsink")
        self.aacaudioqueue = gst.element_factory_make("queue")
        self.aacaudiobin = gst.Bin('aacbin')
        self.aacaudiobin.add(self.aacaudioqueue, self.aacaudiosink)

        gst.element_link_many(self.hantrovideoqueue, self.hantrovideodec)
        gst.element_link_many(self.mp3audioqueue, self.mp3audiosink)
        gst.element_link_many(self.mpegvideoqueue, self.mpegvideodec)
        gst.element_link_many(self.aacaudioqueue, self.aacaudiosink)

        self.hantrovideobin.add_pad(gst.GhostPad('sink', \
                                    self.hantrovideoqueue.get_pad('sink')))
        self.hantrovideobin.add_pad(gst.GhostPad('src',
                                    self.hantrovideodec.get_pad('src')))

        self.mp3audiobin.add_pad(gst.GhostPad('sink', \
                                    self.mp3audioqueue.get_pad('sink')))

        self.mpegvideobin.add_pad(gst.GhostPad('sink', \
                                    self.mpegvideoqueue.get_pad('sink')))
        self.mpegvideobin.add_pad(gst.GhostPad('src', \
                                    self.mpegvideodec.get_pad('src')))

        self.aacaudiobin.add_pad(gst.GhostPad('sink', \
                                    self.aacaudioqueue.get_pad('sink')))

    def resetElements(self):

        if self.player.get_by_name('mp3bin') is not None:
            self.mp3audiobin.unparent()
            self.player.remove(self.mp3audiobin)
        if self.player.get_by_name('hantrobin') is not None:
            self.hantrovideobin.unparent()
            self.player.remove(self.hantrovideobin)
        if self.player.get_by_name('mpegbin') is not None:
            self.mpegvideobin.unparent()
            self.player.remove(self.mpegvideobin)
        if self.player.get_by_name('videosink') is not None:
            self.videosink.unparent()
            self.player.remove(self.videosink)

        self.player.remove(self.filesrc)
        self.player.remove(self.typefinder)
        self.player.add(self.filesrc, self.typefinder)

        gst.element_link_many(self.filesrc, self.typefinder)

    def getState(self):

        if self.player_state == 0:
            return 'STOPPED'
        if self.player_state == 1:
            return 'PLAYING'
        if self.player_state == 2:
            return 'PAUSED_PLAYBACK'

    def __setURI(self, uri):

        self.player.set_state(gst.STATE_NULL)

        #self.buildPipeline()
        self.resetElements()
        self.filesrc.set_property('location', uri)
        self.filesrc.set_property('iradio-mode', True)
        self.filesrc.set_property('typefind', True)
        self.__av_uri = uri
        self.setVolume(self.current_volume)
        self.player.set_state(gst.STATE_READY)

    def __getURI(self):

        return self.__av_uri

    av_uri = property(__getURI, __setURI)

    def on_pad_added_mp3(self, element, pad):

        caps = pad.get_caps()
        name = caps[0].get_name()
        aqpad = self.mp3audiobin.get_pad('sink')
#       print '\non_pad_added:', name
        if name == 'audio/mpeg':
            if not aqpad.is_linked(): # Only link once
                pad.link(aqpad)
            self.mp3audiobin.set_state(gst.STATE_PLAYING)

    def __cb_typefound(self, element, prob, caps):

        if str(caps).find("audio/mpeg") is not -1:
            if self.mp3audiobin.get_parent() is None:
                self.player.add(self.mp3audiobin)
                gst.element_link_many(self.typefinder, self.mp3audiobin)
                self.mp3audiobin.set_state(gst.STATE_PLAYING)
            else:
                self.mp3audiobin.set_state(gst.STATE_READY)
                self.mp3audiobin.set_state(gst.STATE_PLAYING)

        elif str(caps).find("application/x-id3") is not -1:

            if self.mp3audiobin.get_parent() is None:
                id3lib = gst.element_factory_make("id3lib")
                self.player.add(id3lib)
                gst.element_link_many(self.typefinder, id3lib)
                self.player.add(self.mp3audiobin)
                id3lib.connect('pad-added', self.on_pad_added_mp3)

            else:
                self.mp3audiobin.set_state(gst.STATE_READY)
                self.mp3audiobin.set_state(gst.STATE_PLAYING)

        elif str(caps).find("video/x-msvideo") is not -1:
             # Create elements

            if self.hantrovideobin.get_parent() is None:
                demux = gst.element_factory_make("avidemux")
                self.player.add(demux)
                # Link source and demux elements
                gst.element_link_many(self.typefinder, demux)
                # Connect handler for 'pad-added' signal
                demux.connect('pad-added', self.on_pad_added)
                self.player.add(self.hantrovideobin, self.videosink)
                gst.element_link_many(self.hantrovideobin, self.videosink)

                demux.set_state(gst.STATE_PLAYING)


        elif str(caps).find("video/mpeg") is not -1:
            # Create elements
            #print "video/mpeg"
            if self.mpegvideobin.get_parent() is None:
                demux = gst.element_factory_make("mpegdemux")
                self.player.add(demux)
                # Link source and demux elements
                gst.element_link_many(self.typefinder, demux)
                # Connect handler for 'pad-added' signal
                demux.connect('pad-added', self.on_pad_added)
                self.player.add(self.mpegvideobin, self.videosink)
                gst.element_link_many(self.mpegvideobin, self.videosink)

                demux.set_state(gst.STATE_PLAYING)

        elif caps == "adts_mpeg_stream" or caps == "audio/x-ac3" or \
                                        caps == "audio/x-dts":
            if self.aacaudiobin.get_parent() is None:
                self.player.add(self.aacaudiobin)
                gst.element_link_many(self.typefinder, self.aacaudiobin)
                self.aacaudiobin.set_state(gst.STATE_PLAYING)
            else:
                self.aacaudiobin.set_state(gst.STATE_READY)
                self.aacaudiobin.set_state(gst.STATE_PLAYING)

        elif caps == "video/quicktime":
            pass

        else:
            log.msg("format not support"+ str(caps))
            self.player.set_state(gst.STATE_NULL)
            self.__av_uri = None

    def on_pad_added(self, element, pad):
        caps = pad.get_caps()
        name = caps[0].get_name()
        #print '\non_pad_added:', name
        if name == 'audio/mpeg':
            if self.mp3audiobin.get_parent() is None:
                self.player.add(self.mp3audiobin)
            aqpad = self.mp3audiobin.get_pad('sink')
            if not aqpad.is_linked(): # Only link once
                pad.link(aqpad)
            self.mp3audiobin.set_state(gst.STATE_PLAYING)
        elif name == 'audio/x-ac3' or name == 'audio/x-dts':
            if self.aacaudiobin.get_parent() is None:
                self.player.add(self.aacaudiobin)
            aqpad = self.aacaudiobin.get_pad('sink')
            if not aqpad.is_linked(): # Only link once
                pad.link(aqpad)
                self.mp3audiobin.set_state(gst.STATE_PLAYING)
        elif name == 'video/mpeg':
            vqpad = self.mpegvideobin.get_pad('sink')
            if not vqpad.is_linked(): # Only link once
                pad.link(vqpad)
                self.videosink.set_state(gst.STATE_PLAYING)
                self.mpegvideobin.set_state(gst.STATE_PLAYING)
        else:
            vqpad = self.hantrovideobin.get_pad('sink')
            if not vqpad.is_linked(): # Only link once
                pad.link(vqpad)
                self.videosink.set_state(gst.STATE_PLAYING)
                self.hantrovideobin.set_state(gst.STATE_PLAYING)

    def onMessage(self, bus, message):
        t = message.type
        if t == gst.MESSAGE_EOS:
            self.player.set_state(gst.STATE_NULL)
            self.player_state = 0
            self.player.set_state(gst.STATE_READY)

        elif t == gst.MESSAGE_ERROR:
            self.player.set_state(gst.STATE_NULL)
            log.msg(str(message))
            self.player_state = 0
            self.player.set_state(gst.STATE_READY)

    def play(self):

        if self.av_uri is not None:
            if (self.player.set_state(gst.STATE_PLAYING) == \
                gst.STATE_CHANGE_FAILURE):
                log.msg("gst_renderer error at playing")
                return
            self.player_state = 1
        else:
            log.msg("you need to set the uri first")
        return

    def stop(self):

        self.player.set_state(gst.STATE_NULL)
        if (self.player.set_state(gst.STATE_READY) == \
            gst.STATE_CHANGE_FAILURE):
            log.msg("error stopping")
            return
        self.player_state = 0

        self.av_uri = self.__av_uri

    def pause(self):

        if (self.player.set_state(gst.STATE_PAUSED) == \
            gst.STATE_CHANGE_FAILURE):
            log.msg("error pausing")
            return
        self.player_state = 2

    def seek(self, unit, target):

        if unit == "ABS_TIME":
            target_time = self.convert_int(target)
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, \
                                    target_time)

        if unit == "REL_TIME":
            target_time = self.convert_int(target)
            cur_pos = self.queryPosition()[1]
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, \
                                    target_time+cur_pos)

        if unit == "ABS_COUNT":
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, \
                                    target)

        if unit == "REL_COUNT":
            cur_pos = self.queryPosition()[1]
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, \
                                    target + cur_pos)

    def setVolume(self, volume):

        self.current_volume = volume

        self.mp3audiosink.set_property("volume", volume*65535/100)
        self.aacaudiosink.set_property("volume", volume*65535/100)

    def getVolume(self):

        self.current_volume = \
                self.mp3audiosink.get_property("volume")*100/65535
        return self.mp3audiosink.get_property("volume")*100/65535

    def queryDuration(self):

        time.sleep(0.3)
        try:
            dur_int = self.player.query_duration(self.time_format, None)[0]
            dur_str = self.convert_ns(dur_int)
        except:
            dur_int = -1
            dur_str = ''

        return dur_str, dur_int

    def queryPosition(self):

        try:
            pos_int = self.player.query_position(self.time_format, None)[0]
            pos_str = self.convert_ns(pos_int)
        except:
            pos_int = -1
            pos_str = ''

        return pos_str, pos_int

    def convert_ns(self, time_int):
        time_int = time_int / 1000000000
        time_str = ""
        _hours = "00"
        if time_int >= 3600:
            _hours = time_int / 3600
            time_int = time_int - (_hours * 3600)
        time_str = str(_hours) + ":"
        if time_int >= 600:
            _mins = time_int / 60
            time_int = time_int - (_mins * 60)
            time_str = time_str + str(_mins) + ":"
        elif time_int >= 60:
            _mins = time_int / 60
            time_int = time_int - (_mins * 60)
            time_str = time_str + "0" + str(_mins) + ":"
        else:
            time_str = time_str + "00:"
        if time_int > 9:
            time_str = time_str + str(time_int)
        else:
            time_str = time_str + "0" + str(time_int)

        if time_str == "00:00:0-1":
            time_str=""

        return time_str

    def convert_int(self, time_str):

        time_str = time_str.strip('")( ')

        (hours, min, sec) = time_str.split(":")

        time_int = int(hours)*3600 + int(min)*60 + int(sec)

        time_int = time_int*1000000000
        return time_int

if __name__ == "__main__":

    mygst = GSTRendererMaemo()

    mygst.av_uri = "/home/user/MyDocs/.videos/NokiaN93.avi"

    print mygst.av_uri

    mygst.play()

    loop.run()