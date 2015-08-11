# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
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

    av_uri = property(__getURI,__setURI)

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
            if (self.player.set_state(gst.STATE_PLAYING) == gst.STATE_CHANGE_FAILURE):
                log.msg( "error here")
            self.player_state = 1
        else:
            log.msg( "you need to set the uri first")

    def stop(self):

        if (self.player.set_state(gst.STATE_READY) == gst.STATE_CHANGE_FAILURE):
            log.msg( "error here: stopping")
        self.player_state = 0

    def pause(self):

        if (self.player.set_state(gst.STATE_PAUSED) == gst.STATE_CHANGE_FAILURE):
            log.msg( "error here : pausing")
        self.player_state = 2

    def seek(self):

        if unit == "ABS_TIME":
            target_time = self.convert_int(target)
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, target_time)

        if unit == "REL_TIME":
            target_time = self.convert_int(target)
            cur_pos = self.queryPosition()[1]
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, target_time+cur_pos)

        if unit == "ABS_COUNT":
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, target)

        if unit == "REL_COUNT":
            cur_pos = self.queryPosition()[1]
            self.player.seek_simple(self.time_format, gst.SEEK_FLAG_FLUSH, target + cur_pos)

    def setVolume(self,volume):

        self.player.set_property("volume",volume/10)

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
        self.typefinder.connect("have-type",self.__cb_typefound)
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
        self.hantrovideobin.add(self.hantrovideoqueue,self.hantrovideodec)

        self.mpegvideodec = gst.element_factory_make("ffdec_mpegvideo")
        self.mpegvideoqueue = gst.element_factory_make("queue")
        self.mpegvideobin = gst.Bin('mpegbin')
        self.mpegvideobin.add(self.mpegvideoqueue, self.mpegvideodec)

        self.aacaudiosink = gst.element_factory_make("dspaacsink")
        self.aacaudioqueue = gst.element_factory_make("queue")
        self.aacaudiobin = gst.Bin('aacbin')
        self.aacaudiobin.add(self.aacaudioqueue,self.aacaudiosink)

        gst.element_link_many(self.hantrovideoqueue, self.hantrovideodec)
        gst.element_link_many(self.mp3audioqueue, self.mp3audiosink)
        gst.element_link_many(self.mpegvideoqueue, self.mpegvideodec)
        gst.element_link_many(self.aacaudioqueue, self.aacaudiosink)

        self.hantrovideobin.add_pad(gst.GhostPad('sink',self.hantrovideoqueue.get_pad('sink')))
        self.hantrovideobin.add_pad(gst.GhostPad('src',self.hantrovideodec.get_pad('src')))

        self.mp3audiobin.add_pad(gst.GhostPad('sink',self.mp3audioqueue.get_pad('sink')))

        self.mpegvideobin.add_pad(gst.GhostPad('sink',self.mpegvideoqueue.get_pad('sink')))
        self.mpegvideobin.add_pad(gst.GhostPad('src',self.mpegvideodec.get_pad('src')))

        self.aacaudiobin.add_pad(gst.GhostPad('sink',self.aacaudioqueue.get_pad('sink')))

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

