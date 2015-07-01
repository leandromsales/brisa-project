# feeds.py - Gets the current listings of Shoutcast stations
# $Id: /pyshout/newfeeds/python-shoutcast/py_shoutcast/feeds.py 7 2006-07-03T04:56:26.069531Z matt  $
# Copyright (C) 2005-2006 Matthew Schick <matt@excentral.org>

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

import cPickle
from urllib import FancyURLopener
from zlib import decompress
from xml.sax import make_parser,parseString
from xml.sax.handler import ContentHandler
from re import compile,search,I
from encodings.base64_codec import base64_decode
from os import stat,mkdir
from os.path import dirname,isdir,isfile
from time import time
from stat import *
from sys import exit

tmpxml='shout.xml'
DEBUG=0

def write_cache(cache_file, cache_data):
	"""
	Does a cPickle dump
	"""
	if not isdir( dirname(cache_file) ):
		try:
			mkdir( dirname(cache_file) )
		except OSError:
			print dirname(cache_file), 'is a file'
	fd = open(cache_file, 'w')
	cPickle.dump(cache_data, fd, -1)
	fd.close()

def valid_cache(cache_file, cache_ttl):
	"""
	See if the cache file exists and is still living
	"""
	try:
		mtime = stat(cache_file)[ST_MTIME]
	except:
		return 0
	curr_time = time()
	if (curr_time - mtime) > cache_ttl:
		return 0
	else:
		return 1

def load_cache(cache_file):
	"""
	Does a cPickle load
	"""
	fd = open(cache_file)
	cache_data = cPickle.load(fd)
	fd.close()
	return cache_data

class StationParser(ContentHandler):
	"""
	SAX handler for xml feed, not for public consumption
	"""
	def __init__(self,min_bitrate):
		self.isStationList = False
		self.isTuneIn = False
		self.isStation = False
		self.station_list = []
		self.min_bitrate = min_bitrate
		self.mimeType = ''
		self.Id = ''
		self.Name = ''
		self.Bitrate = ''
		self.nowPlaying = ''
		self.Listeners = ''
		self.stationUrl = ''
		self.Genre = ''
		self.count = 0
		self.shoutUrl = 'http://www.shoutcast.com'
	def startElement(self, name, attrs):
		if name == 'stationlist':
			self.isStationList = True
		if name == 'tunein':
			self.isTuneIn = True
			self.baseUrl = attrs.get('base', None)
		if name == 'station':
			self.isStation = True
			self.Name = attrs.get('name', None)
			self.mimeType = attrs.get('mt', None)
			self.Id = attrs.get('id', None)
			self.Bitrate = attrs.get('br', None)
			self.nowPlaying = attrs.get('ct', None)
			self.Listeners = attrs.get('lc', None)
			self.Genre = attrs.get('genre', None)
	def endElement(self,name):
		if name == 'station':
			self.isStation = False
		if name == 'tunein':
			self.isTuneIn = False
		if name == 'station':
			self.isStation = False
			if int(self.Bitrate) >= self.min_bitrate:
				self.stationUrl = self.shoutUrl + self.baseUrl + '?id=' + self.Id
				self.station_list.append({'Name':self.Name, 'PLS_URL':self.stationUrl, 'NowPlaying':self.nowPlaying, 'Listeners':self.Listeners, 'Bitrate':self.Bitrate, 'MimeType':self.mimeType, 'Genres': self.Genre})
				self.count += 1
		if name == 'stationlist':
			self.isStationList = False
			if DEBUG == 1:
				print 'Parsed ', self.count, ' stations'

class GenreParse(ContentHandler):
	def __init__( self ):
		self.isGenre = False
		self.isGenreList = False
		self.genreList = []
	def startElement( self, name, attrs ):
		if name == 'genrelist':
			self.isGenreList = True
		if name == 'genre':
			self.isGenre == True
			self.genre_name = attrs.get( 'name', None )
	def endElement( self, name ):
		if name == 'genre':
			self.isGenre = False
			self.genreList.append( self.genre_name )
		if name == 'genrelist':
			self.isGenreList = False

class GenreFeed(object):
	def __init__(self, cache_ttl=6000, cache_dir = '/tmp/pyshout_cache'):
		self.cache_ttl = cache_ttl
		self.cache_file = cache_dir + '/genres.cache'
	def fetch_genres(self):
		"""
		Grabs genres and returns tuple of genres
		"""
		self.genre_url = 'http://www.shoutcast.com/sbin/newxml.phtml'
		self.urlhandler = FancyURLopener()
		self.fd = self.urlhandler.open(self.genre_url)
		self.genre = self.fd.read()
		self.fd.close()
		return self.genre

	def parse_genres(self):
		self.inv_cache = 0
		self.vc = valid_cache(self.cache_file, self.cache_ttl)
		if self.cache_ttl > 0 and self.vc != 0:
			if DEBUG == 1:
				print 'Loading cache from ',self.cache_file
			try:
				self.genre_list = load_cache(self.cache_file)
			except:
				self.inv_cache = 1
		if self.cache_ttl == 0 or self.inv_cache == 1 or self.vc == 0:
			if DEBUG == 1:
				print 'Getting fresh feed'
			parseXML = GenreParse()
			self.genres = self.fetch_genres()
			parseString( self.genres, parseXML )
			self.genre_list = parseXML.genreList
			write_cache(self.cache_file, self.genre_list)
		return self.genre_list

class ShoutcastFeed(object):
	def __init__(self, genre, min_bitrate=128, cache_ttl=600, cache_dir='/tmp/pyshout_cache', limit=7):
		"""
		Parses the xml feed and spits out a list of dictionaries with the station info
		keyed by genre. Params are as follows:
		min_bitrate - 128 default, Minimum bitrate filter
		cache_ttl - 600 default, 0 disables, Seconds cache is considered valid
		cache_dir - /tmp/pyshout_cache default, Path to cache directory
		"""
		self.min_bitrate = min_bitrate
		self.cache_ttl = cache_ttl
		self.genre = genre
		self.cache_file = cache_dir + '/' + self.genre + '.pickle'
		self.station_list = []
		self.limit = limit

	def fetch_stations(self):
		"""
		Grabs the xml list of stations from the shoutcast server
		"""
		self.shout_url='http://www.shoutcast.com/sbin/newxml.phtml?genre=' + self.genre + '&limit=' + str(self.limit)
		self.urlhandler = FancyURLopener()
		self.fd = self.urlhandler.open(self.shout_url)
		self.stations = self.fd.read()
		self.fd.close()
		return self.stations

	def parse_stations(self):
		self.inv_cache = 0
		self.vc = valid_cache(self.cache_file, self.cache_ttl)
		if self.cache_ttl > 0 and self.vc != 0:
			if DEBUG == 1:
				print 'Loading cache from ', self.cache_file
			try:
				self.station_list = load_cache(self.cache_file)
			except:
				self.inv_cache = 1
		if self.cache_ttl == 0 or self.inv_cache == 1 or self.vc == 0:
			if DEBUG == 1:
				print 'Getting fresh feed'
			parseXML = StationParser(self.min_bitrate)
			self.stations = self.fetch_stations()
			parseString(self.stations, parseXML)
			self.station_list = parseXML.station_list
			write_cache(self.cache_file, self.station_list)
		return self.station_list

# vim:syntax=python
# vim:ts=4
# cursor: 4 del

