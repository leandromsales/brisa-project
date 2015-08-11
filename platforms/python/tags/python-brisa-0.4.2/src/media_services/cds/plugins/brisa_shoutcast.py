# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from urllib import urlopen
from urllib import urlencode
from xml.etree import ElementTree
import time
import ConfigParser
import brisa_logging
import logging
from os import path

class BrisaShoutcast(object):

    cache = ConfigParser.SafeConfigParser()
    cache_file = path.join(path.expanduser('~'),
                           '.brisa',
                           'cached_stations')

    def _get_cached_stream(self, id):
        f=open(self.cache_file)
        self.cache.read(f)
        return self.cache.get(id, 'url')

    def _add_to_cache(self, id, stream):
        try:
            f=open(self.cache_file, 'r+')
        except:
            f=open(self.cache_file, 'w')
            logging.debug('Creating shoutcast station cache')
        self.cache.add_section(id)
        self.cache.set(id, 'url', stream)
        self.cache.write(f)

    def _id_is_cached(self, id):
        try:
            f=open(self.cache_file)
            self.cache.readfp(f)
            cached = self.cache.has_section(id)
            return cached
        except:
            return False

    def _alternative_station_list(self):
            query = {'genre': 'top',
                     'mt': 'audio/mpeg',
                     'br': '128'}
            params = urlencode(query)
            url = 'http://www.shoutcast.com/sbin/newxml.phtml?%s'%(params)
            logging.debug('brisa_shoutcast>> openning alternative url: %s'%\
                          (url))
            response = urlopen(url)
            return ElementTree.parse(response).getroot()

    def station_url(self, id):
        cached = self._id_is_cached(id)
        if cached:
            return self._get_cached_stream(id)
        query = {'id': id}
        params = urlencode(query)
        url = 'http://www.shoutcast.com/sbin/tunein-station.pls?%s'%(params)
        playlist_file = urlopen(url+params)
        config = ConfigParser.SafeConfigParser()
        config.readfp(playlist_file)
        stream = config.get('playlist', 'File1')
        if not cached:
            self._add_to_cache(id, stream)
        return stream

    def station_list(self):
        query = {'genre': 'Top500',
                 'mt': 'audio/mpeg',
                 'br': '128'}
        params = urlencode(query)
        url = 'http://www.shoutcast.com/sbin/newxml.phtml?%s'%(params)
        logging.debug('brisa_shoutcast>> openning url: %s'% (url))
        response = urlopen(url)
        try:
            radios_et = ElementTree.parse(response).getroot()
        except:
            radios_et = self._alternative_station_list()
    #    logging.debug('brisa_shoutcast>> got response:\n%s'%\
    #                  (ElementTree.tostring(radios_et)))
        stations = []
        for station_et in radios_et:
            station={}
            station['bitrate']=station_et.get('br')
            station['name']=station_et.get('name')
            station['genre']=station_et.get('genre')
            station['id']=station_et.get('id')
            if station['id']:
                stations.append(station)
        return stations

if __name__ == '__main__':
#    sc = BrisaShoutcast()
#    print sc.station_url('1234')
    sc = BrisaShoutcast()
    tick = time.time()
    stations = sc.station_list()
    print len(stations)
    sts=[]
    for station in stations:
        sts.append(sc.station_url(station['id']))
#    print stations
#    print stream_url('8751')
    tack = time.time()
    print tack - tick

#    tick = time.time()
#    i=0
#    url = 'http://www.shoutcast.com/sbin/newxml.phtml?genre=Top500'
#    response = urlopen(url)
#    i = i+1
#    print 'request %i'% (i)
#    response_element = ElementTree.parse(response).getroot()
#    response.close()
#    print ElementTree.tostring(response_element)
#    tack = time.time()
#    print tack - tick

#    tick = time.time()
#    shout = feeds.ShoutcastFeed('Top500')
#    station_list = shout.parse_stations()
#    print station_list
#    tack = time.time()
#    print tack - tick