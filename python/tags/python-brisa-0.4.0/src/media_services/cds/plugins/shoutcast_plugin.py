# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import logging
from urllib import URLopener

from plugin import Plugin
from feeds import ShoutcastFeed
from persistence import Facade, Persistence
import brisa_config

class ShoutCastPlugin(Plugin):
    '''
    ShoutCast plugin
    Obtains the urls of the shoutcast stations from www.shoutcast.com
    '''
    name = 'shoutcast'
    genre_list = ['Country', 'Dance', 'Jazz', 'Metal', 'Oldies', 'Progressive', 'Rock']
    use = brisa_config.get_parameter('shoutcast','usage')

    def __init__(self, *args, **kwargs):
        Plugin.__init__(self, *args, **kwargs)

    def run(self):
        audioBroadcastContainer = Facade.selectContainerByTitle("Audio Broadcast")[0]
        audioShoutCastContainer = Facade.addContainer(title="Shoutcast stations", container= audioBroadcastContainer)

        for genre in self.genre_list:
            Container = Facade.addContainer(title=genre, container= audioShoutCastContainer)
            shout = ShoutcastFeed(genre, limit=8)
            try:
                shout.fetch_stations()
            except IOError:
                return

            station_list = shout.parse_stations()

            for station in station_list:
                station_name = station['Name'].decode('utf-8','replace').encode('ascii','ignore')
                station_bitrate = station['Bitrate']
                try:
                    station_url = self.get_shoutcast_url(station['PLS_URL'])
                except IOError:
                    return
                if station_url == 'Error':
                    continue
                audioBroadcast = Facade.addAudioBroadcast(bitRate = station_bitrate,
                                                          title = station_name,
                                                          local_path = station_url,
                                                          date = "1993-01-01",
                                                          mediaType="audio/broadcast",
                                                          protocolInfo="http-get:*:audio/mpegurl:*"
                                                          )
                Container.addItem(audioBroadcast)
        return


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


    def get_genre_list(self,genre):
        return self.genre_list

    def load(self):
        logging.info("Loading Shoutcast Plugin")
        self.run()

if __name__=='__main__':
    st = ShoutCastPlugin()
    st.create_db()
