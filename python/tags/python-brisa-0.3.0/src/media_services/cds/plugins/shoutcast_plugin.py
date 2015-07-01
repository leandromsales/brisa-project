# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team

from plugin import Plugin
from feeds import ShoutcastFeed
from urllib import URLopener
from persistence import Facade, Persistence

class ShoutCastPlugin(Plugin):
    '''
    ShoutCast plugin
    Obtains the urls of the shoutcast stations from www.shoutcast.com
    '''
    name = 'shoutcast'
    genre_list = ['Country', 'Dance', 'Jazz', 'Metal', 'Oldies', 'Progressive', 'Rock']
    #genre_list = ['Pop']

    def __init__(self, *args, **kwargs):
    	pass

    def create_db(self):
        audioBroadcastContainer = Facade.selectContainerByTitle("Audio Broadcast")
        audioBroadcastContainer = audioBroadcastContainer[0]
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
                                                          path = station_url,
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
        print "Loading Shoutcast Plugin"
        self.create_db()

if __name__=='__main__':
    st = ShoutCastPlugin()
    st.create_db()
