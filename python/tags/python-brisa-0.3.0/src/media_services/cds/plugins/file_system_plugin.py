# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team

from media_services.cds.plugins.plugin import Plugin
#from media_services.cds.FSStorage import FSDirectory
import os
from tagger import ID3v2,ID3v1
from persistence import Facade
import platform
import time
from twisted.web import resource, static

class FSPlugin(object):

    def __init__(self, *args, **kwargs):
        '''
        Arguments:
        args - The configuration parameters to the loadded plugin.
        '''
        self.FS_directories = self._parse_plugin_config(*args)
        self.content = kwargs['webResource']
        self.urlbase = str(kwargs['urlbase'])

    def _parse_plugin_config(self,args):
        '''
        Parse the list of directories.
        Arguments:
        directories - An string containing the information
                      about the directories to be read.
        Returns:
        storage_directory_list - A list containing all
                                 directories into the plugin.
        '''
        #TODO: error handle code here
        storage_directory_list = args.split(":")
        return storage_directory_list

    def _list_directory(self,dir_name):
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
        '''
        Load the plugin media information into the database.
        '''
        # Read all files from all directories andd add each file to the database
        print "Loading Plugin " + self.name
        for directory in self.FS_directories:
            files = self._list_directory(directory)
            for file in files:
                media_info = self._get_media_info(file)
                self._add_to_DB(media_info)

    def getUsage(self):
        '''
        Return a usage message containing the structure of the
        args argument of the construct
        '''
        usage = "\"name:path1:path2, ...\""
        return usage

class FSMp3Plugin(FSPlugin,Plugin):
    name = 'filemp3'
    FS_directories = None

    default_MusicTrack = {
                          'type':'MusicTrack',
                          'composer':'',
                          'title':'',
                          'date':'1900-00-00',
                          'copyright':'',
                          'url':'',
                          'protocolInfo':'',
                          'description':'',
                          'longDescription':'',
                          'publisher':'',
                          'producer':'',
                          'mediaType':"audio/xxx",
                          'language':"",
                          'path':'',
                          'genre':'',
                          'artist':'',
                          'mediaLength':0,
                          'originalTrackNumber': '0'
                          }


    # Convert a field ID from http://www.id3.org/id3v2.3.0
    #to the database structure
    conv_fid= {
               'TRCK':'originalTrackNumber',
               'TYER':'year',
               'PRIV':'private', # unused
               'TIT2':'title',
               'TALB':'album', #unused
               'TCON':'genre',
               'WOAF':'', #unused oficial file webpage
               'WOAR':'url',
               'WOAS':'',#unused
               'TPE1':'artist',
               'TLEN':'mediaLength',
               'COMM':'description',
               'TENC':'TENC',
               'TLAN':'language',
               'TDRC':'TDRC', # not appear in http://www.id3.org/id3v2.3.0
               'TPE2':'TPE2',
               'MCDI':'MCDI',
               'TCOM':'composer',
               'APIC':'APIC',
               'WXXX':'WXXX',
               'TOPE':'TOPE',
               'TSSE':'TSSE',
               'TPUB':'publisher'
               }
    def __init__(self,*args,**kwargs):
        FSPlugin.__init__(self,*args,**kwargs)

    def _is_audio_file(self,file):
        '''
        Discovery the tipe of media using the file name.
        '''
        audio_types=['.mp3']
        (root, ext) = os.path.splitext(file)
        return audio_types.__contains__(ext.lower())


    def _get_media_info(self,file):
        '''
        Get the information about the media file.
        Returns:
        media_info - A dictionary containing the
                     information of the media file.
        '''
        media_info = {'type':None}

        if self._is_audio_file(file):
            media_info = self._get_audio_info(file)

        return media_info


    def _get_audio_info(self,file):
        '''
        Get the information about the audio media file.
        Returns:
        audio_info - A dictionary containing the
                     information of the media file.
        '''
        audio_info = self.default_MusicTrack
        audio_info['title']=os.path.basename(file)

        audio_info['mediaLength'] = self.queryDuration(file)

        j = 1
        if len(self.content.listEntities()) is not 0:
            for i in self.content.listEntities():
                if i[0] == os.path.basename(file):
                    self.content.putChild(str(j) + "" + os.path.basename(file).replace(" ","_"),static.File(file))
                    audio_info['path']= self.urlbase.rstrip("/") + "/" + str(j) + "" + os.path.basename(file).replace(" ","_")
                else:
                    self.content.putChild(os.path.basename(file).replace(" ","_"),static.File(file))
                    audio_info['path']= self.urlbase.rstrip("/") + "/" + os.path.basename(file).replace(" ","_")
                j = j + 1
        else:
           self.content.putChild(os.path.basename(file).replace(" ","_"),static.File(file))
           audio_info['path']= self.urlbase.rstrip("/") + "/" + os.path.basename(file).replace(" ","_")

        #print file
        #Parse id3v1 info
        try:
            id3v1 = ID3v1(file)
        except:
            return audio_info

        if id3v1.tag_exists():
            audio_info['title']=str(id3v1.songname)
            audio_info['artist']=str(id3v1.artist)
            audio_info['album']=str(id3v1.album)
            audio_info['year']=str(id3v1.year)
            audio_info['description']=str(id3v1.comment)
            audio_info['genre']=str(id3v1.genre)
        #Parse id3v2
        #print file
        try:
            id3 = ID3v2(file)
        except:
            return audio_info

        if id3.tag_exists():
            audio_info['mediaType']="audio/mpeg"
            #The frame identifier folow the table
            #in http://www.id3.org/id3v2.3.0
            for frame in id3.frames:
                if self.conv_fid.has_key(frame.fid):
                    field = self.conv_fid[frame.fid]
                    if len(frame.strings)>0:
                        audio_info[field]=frame.strings[0]
        # If the date is unavaliable creat it using the year
        if audio_info['date']==''and audio_info['year']!='':
            audio_info['date']=audio_info['year']+'-01-01'

        return audio_info

    def _add_to_DB(self,media):
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

        try:
            Facade.addMusicTrack(
                   originalTrackNumber=track_num,
                   composer=media['composer'].decode('utf-8','replace').encode('ascii','ignore'),
                   title=media['title'].decode('utf-8','replace').encode('ascii','ignore'),
                   copyright=media['copyright'].decode('utf-8','repla
