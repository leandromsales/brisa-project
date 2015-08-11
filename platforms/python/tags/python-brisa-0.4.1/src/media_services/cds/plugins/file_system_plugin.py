# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import os
#import platform
#import time
import logging
import mimetypes
import traceback
import sys
import re

from tagger import ID3v2, ID3v1
import os
from stat import ST_SIZE

from persistence import Facade
from media_services.cds.plugins.plugin import Plugin
import brisa_config

class FSPlugin(object):

    def __init__(self, *args, **kwargs):
        '''
        Arguments:
        args - The configuration parameters to the loadded plugin.
        '''
        pass

    def _list_directory(self, dir_name):
        '''
        List the files contained into the directory.
        Returns:
        file_list - A list containing all files in dir_name.
        '''
        file_list = []
        for root, dirs, files in os.walk(dir_name):
            for file in files:
                root = root.rstrip("/")
                file_list.append(root+"/"+file)
        return file_list

    def load(self):
        '''Load the plugin media information into the database.'''
        #Read all files from all directories andd add each file to the database
        logging.info("Loading Plugin " + str(self.name))
        for directory in self.dirs:
            files = self._list_directory(directory)
            for file in files:
                self.add_item(file)

    def add_item(self, file):
        sizebytes = os.stat(file)[ST_SIZE]
        if not Facade.localItemExist(unicode(file, errors='replace').\
                                 encode('ascii', 'ignore'), sizebytes):
            media_info = self._get_media_info(file)
            self._real_add_item(media_info)

    def getUsage(self):
        '''
        Return a usage message containing the structure of the
        args argument of the construct
        '''
        usage = "\"name:path1:path2, ...\""
        return usage

class FSMp3Plugin(FSPlugin, Plugin):
    name = 'filemp3'
    use = brisa_config.get_parameter('audio', 'usage')
    watch = brisa_config.get_parameter('image', 'watch')
    dirs = brisa_config.get_parameter_as_list('audio', 'dirs')

    default_MusicTrack = {
                          'type': 'MusicTrack',
                          'composer': '',
                          'title': '',
                          'album': '',
                          'date': '1900-00-00',
                          'copyright': '',
                          'url': '',
                          'protocolInfo': '',
                          'description': '',
                          'longDescription': '',
                          'publisher': '',
                          'producer': '',
                          'mediaType': "audio/xxx",
                          'language': "",
                          'path': '',
                          'local_path': '',
                          'genre': '',
                          'artist': '',
                          'mediaLength': 0,
                          'originalTrackNumber': '0'}
    # Convert a field ID from http://www.id3.org/id3v2.3.0
    #to the database structure
    conv_fid= {
               'TRCK': 'originalTrackNumber',
               'TYER': 'year',
               'PRIV': 'private', # unused
               'TIT2': 'title',
               'TALB': 'album', #unused
               'TCON': 'genre',
               'WOAF': '', #unused oficial file webpage
               'WOAR': 'url',
               'WOAS': '', #unused
               'TPE1': 'artist',
               'TLEN': 'mediaLength',
               'COMM': 'description',
               'TENC': 'TENC',
               'TLAN': 'language',
               'TDRC': 'TDRC', # not appear in http://www.id3.org/id3v2.3.0
               'TPE2': 'TPE2',
               'MCDI': 'MCDI',
               'TCOM': 'composer',
               'APIC': 'APIC',
               'WXXX': 'WXXX',
               'TOPE': 'TOPE',
               'TSSE': 'TSSE',
               'TPUB': 'publisher'}

    def __init__(self, *args, **kwargs):
        FSPlugin.__init__(self, *args, **kwargs)
        Plugin.__init__(self, *args, **kwargs)

    def _is_audio_file(self, file):
        '''
        Discovery the tipe of media using the file name.
        '''
        audio_types=['.mp3']
        (root, ext) = os.path.splitext(file)
        return audio_types.__contains__(ext.lower())

    def _get_media_info(self, file):
        '''
        Get the information about the media file.
        Returns:
        media_info - A dictionary containing the
                     information of the media file.
        '''
        media_info = {'type': None}
        if self._is_audio_file(file):
            media_info = self._get_audio_info(file)
        return media_info

    def _parse_id3v1_info(self, file, audio_info):
        try:
            id3v1 = ID3v1(file)
        except:
            logging.debug("No ID3v1 info in file: "+str(file))
            return audio_info

        if id3v1.tag_exists():
            audio_info['title'] = str(id3v1.songname)
            audio_info['artist'] = str(id3v1.artist)
            audio_info['album'] = str(id3v1.album)
            audio_info['year'] = str(id3v1.year)
            audio_info['description'] = str(id3v1.comment)
            audio_info['genre'] = str(id3v1.genre)
        return audio_info

    def _parse_id3v2_info(self, file, audio_info):
        try:
            id3v2 = ID3v2(file)
        except:
            logging.debug("No ID3v2 info in file: "+str(file))
            return audio_info

        if id3v2.tag_exists():
            audio_info['mediaType']="audio/mpeg"
            #The frame identifier folow the table
            #in http://www.id3.org/id3v2.3.0
            for frame in id3v2.frames:
                if frame.fid in self.conv_fid:
                    field = self.conv_fid[frame.fid]
                    if len(frame.strings) > 0:
                        audio_info[field]=frame.strings[0]
        # If the date is unavaliable create it using the year
        if audio_info['date']==''and audio_info['year']!='':
            audio_info['date']=audio_info['year']+'-01-01'

        return audio_info

    def _get_audio_info(self, file):
        '''
        Get the information about the audio media file.
        Returns:
        audio_info - A dictionary containing the
                     information of the media file.
        '''
        audio_info = self.default_MusicTrack
        audio_info['title'] = ""
        audio_info['mediaLength'] = self.queryDuration(file)
        audio_info['local_path'] = file

        (root, ext) = os.path.splitext(file)
        audio_info['mediaType']= mimetypes.types_map[ext]

        #Parse id3v1 info
        audio_info = self._parse_id3v1_info(file, audio_info)
        #Parse id3v2
        audio_info = self._parse_id3v2_info(file, audio_info)

        if (audio_info['title'].strip() == ""):
            audio_info['title'] = os.path.basename(file)

        return audio_info

    def _real_add_item(self, media):
        '''
        Add description in media to DB
        '''
        if media['type']=='MusicTrack':
            self._add_MusicTrack_to_DB(media)

    def _add_MusicTrack_to_DB(self, media):
        try:
            track_num=int(media['originalTrackNumber'].split('/')[0])
        except:
            track_num = 0

        try:
            media_length=str(media['mediaLength'])
        except:
            media_length= ("")

        reg_genre_id = re.compile(r"(\d+)")

        id_found = reg_genre_id.search(media['genre'])
        if id_found and id_found.group() in genre_codes:
            media['genre'] = genre_codes[id_found.group()]
        else:
            if not media['genre'] in genre_codes.values():
                media['genre'] = "Unknown"

        media['artist'] = media['artist'].strip().title()
        media['album'] = media['album'].strip().title()
        media['title'] = media['title'].strip().title()
        media['genre'] = media['genre'].strip().title()

        if (media['album'] == ""):
            media['album'] = "Unknown Album"

        if (media
