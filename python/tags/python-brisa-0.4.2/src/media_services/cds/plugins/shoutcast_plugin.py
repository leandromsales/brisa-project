# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import brisa_logging

import logging
from urllib import URLopener

from plugin import Plugin
from persistence import Facade, Persistence
import brisa_config
from brisa_shoutcast import BrisaShoutcast

class ShoutCastPlugin(Plugin):
    '''
    ShoutCast plugin
    Obtains the urls of the shoutcast stations from www.shoutcast.com
    '''
    name = 'shoutcast'
    genre_list = ['Country',
                  'Dance',
                  'Jazz',
                  'Metal',
                  'Oldies',
                  'Progressive',
                  'Rock']
    use = brisa_config.get_parameter('shoutcast', 'usage')
    shoutcast=BrisaShoutcast()

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)

    def load_stations(self):
        radiocount=0
        # create_containers
        broadcast_container=Facade.selectContainerByTitle("Audio Broadcast")[0]
        shoutcast_container=Facade.addContainer(title="Shoutcast stations",
                                                container=broadcast_container)
        genre_containers={}
        for genre in self.genre_list:
            genre_containers[genre]=Facade.addContainer(title=genre,
                                                container=shoutcast_container)
        stations=self.shoutcast.station_list()
        for station in stations:
            for genre in self.genre_list:
                if genre.lower() in station['genre'].lower():
                    try:
                        url=self.shoutcast.station_url(station['id'])
                    except Exception, e:
                        msg="Can't load station %s, ignoring it"%\
                        (station['name'])
                        brisa_logging.show_exception(msg, e)
                        break
                    audio_broadcast=\
                    Facade.addAudioBroadcast(\
                                    bitRate=station['bitrate'],
                                    title=station['name'],
                                    local_path=url,
                                    date="1993-01-01",
                                    mediaType="audio/broadcast",
                                    protocolInfo="http-get:*:audio/mpegurl:*")
                    genre_containers[genre].addItem(audio_broadcast)
                    radiocount=radiocount+1
                    break
        logging.debug('shoutcast_plugin %s stations loaded'%(radiocount))

    def get_shoutcast_url(self, playlist):
        urlhandler = URLopener()
        fd = urlhandler.open(playlist)
        data = fd.read()
        fd.close()

        for line in data.splitlines():
            result = line.find('File1=')
            if result != -1:
                return line.split('File1=')[1]
        return 'Error'

    def get_genre_list(self, genre):
        return self.genre_list

    def load(self):
        import time
        tick = time.time()
        logging.info("Loading Shoutcast Plugin")
        self.load_stations()
        tack = time.time()
        loadtime=tack-tick
        logging.debug('Shoutcast plugin loaded in %s seconds'%(loadtime))