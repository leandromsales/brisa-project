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

#from tagger import ID3v2, ID3v1
import pyid3lib
import os
from stat import ST_SIZE

from persistence import Facade
from media_services.cds.plugins.plugin import Plugin
from brisa_utils import filesystem
import brisa_config

class FSPlugin(object):

    def __init__(self, *args, **kwargs):
        '''
        Arguments:
        args - The configuration parameters to the loadded plugin.
        '''
        pass

    def load(self):
        '''Load the plugin media information into the database.'''
        #Read all files from all directories andd add each file to the database
        logging.info("Loading Plugin " + str(self.name))
        for directory in self.dirs:
            files = filesystem.list_directory(directory)
            for file in files:
                self.add_item(file)
#        Isso provavelmente vai ficar junto com a linha de cima
#        item = Facade.selectItemByLocalPath(directory)
#        item.valid = 1
    def add_item(self, file):
        sizebytes = os.stat(file)[ST_SIZE]
        if not Facade.localItemExist(unicode(file, errors='replace').\
                                 encode('ascii', 'ignore'), sizebytes):
            media_info = self._get_media_info(file)
            self._real_add_item(media_info)
        else:
            item = Facade.selectItemByLocalPath(file)[0]
            item.valid = "1"

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

    id3_attrs = {'title': 'title', 'artist': 'artist', 'album': 'album',
                 'year': 'year', 'genre': 'contenttype', 'date': 'date',
                 'composer': 'composer', 'publisher': 'publisher',
                 'url': 'wwwaudiosource'}

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

    def _parse_id3(self, file, audio_info):
        id3 = pyid3lib.tag(file)

        items = self.id3_attrs.items()

        for (db_field, id3_field) in items:
            try:
                audio_info[db_field] = eval("id3." + id3_field)
            except AttributeError, e:
                audio_info[db_field] = ""

        # TODO read the comment field as a "advanced way" of pyid3lib
        audio_info['description'] = ""
        audio_info['longDescription'] = ""
        audio_info['mediaType'] = "audio/mpeg"

        # If the date is unavaliable create it using the year
        if audio_info['date'] == '' and audio_info['year'] != '':
            audio_info['date'] = str(audio_info['year']) + '-01-01'

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

        #Parse id3
        audio_info = self._parse_id3(file, audio_info)

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

        if (media['artist'] == ""):
            media['artist'] = "Unknown Artist"

        encoding = 'latin-1'
        if not isinstance(media['composer'], unicode):
            media['composer'] = unicode(media['composer'], encoding)
        if not isinstance(media['local_path'], unicode):
            media['local_path'] = unicode(media['local_path'], encoding)
        if not isinstance(media['title'], unicode):
            media['title'] = unicode(media['title'], encoding)
        if not isinstance(media['copyright'], unicode):
            media['copyright'] = unicode(media['copyright'], encoding)
        if not isinstance(media['url'], unicode):
            media['url'] = unicode(media['url'], encoding)
        if not isinstance(media['description'], unicode):
            media['description'] = unicode(media['description'], encoding)
        if not isinstance(media['longDescription'], unicode):
            media['longDescription'] = unicode(media['longDescription'],\
                                                     encoding)
        if not isinstance(media['publisher'], unicode):
            media['publisher'] = unicode(media['publisher'], encoding)
        if not isinstance(media['producer'], unicode):
            media['producer'] = unicode(media['producer'], encoding)
        if not isinstance(media['mediaType'], unicode):
            media['mediaType'] = unicode(media['mediaType'], encoding)
        if not isinstance(media['genre'], unicode):
            media['genre'] = unicode(media['genre'], encoding)
        if not isinstance(media['artist'], unicode):
            media['artist'] = unicode(media['artist'], encoding)
        if not isinstance(media['album'], unicode):
            media['album'] = unicode(media['album'], encoding)

        Facade.addMusicTrack(
               local_path=media['local_path'],
               originalTrackNumber=track_num,
               composer=media['composer'],
               title=media['title'],
               copyright=media['copyright'],
               url=media['url'],
               protocolInfo="http-get:*:audio/mpeg:*",
               description=media['description'],
               longDescription=media['longDescription'],
               publisher=media['publisher'],
               producer=media['producer'],
               mediaType=media['mediaType'],
               language=media['language'],
               mediaLength=media_length,
               bitRate="",
               genre=media['genre'],
               artist=media['artist'],
               album=media['album'])

    def queryDuration(self, path):
        import mad
        file = mad.MadFile(path)
        time = file.total_time()
        dur_str = self.convert_ms(time)
        return dur_str

    def convert_ms(self, time_int):
        time_int = time_int / 1000
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

#        time_str = time_str+"[]"

        if time_str == "00:00:0-1":
            time_str=""
        return time_str

class FSVideoPlugin(FSPlugin, Plugin):
    name='filevideo'
    use = brisa_config.get_parameter('video', 'usage')
    watch = brisa_config.get_parameter('image', 'watch')
    dirs = brisa_config.get_parameter_as_list('video', 'dirs')

    def __init__(self, *args, **kwargs):
        FSPlugin.__init__(self, *args, **kwargs)
        Plugin.__init__(self, *args, **kwargs)

    def _is_video_file(self, file):
        '''
        Discovery the tipe of media using the file name.
        '''
        video_types=['.mpg',
                     '.avi',
                     '.mpeg',
                     '.mpe',
                     '.mpa',
                     '.m1v',
                     '.mpg',
                     '.qt',
                     '.mov',
                     '.movie']
        (root, ext) = os.path.splitext(file)
        return video_types.__contains__(ext.lower())

    def _get_media_info(self, file):
        '''
        Get the information about the media file.
        Returns:
        media_info - A dictionary containing the
                     information of the media file.
        '''
        media_info = {'type': None}
        if self._is_video_file(file):
            media_info = self._get_video_info(file)
        return media_info

    def _get_video_info(self, file):
        '''
        Get the information about the video media file.
        Returns:
        video_info - A dictionary containing the
                     information of the media file.
        '''
        video_info = {'type': 'videoItem'}
        video_info['title']=os.path.basename(file)
        video_info['local_path'] = file
        video_info['path'] = ''

        (root, ext) = os.path.splitext(file)
        video_info['mediaType']= mimetypes.types_map[ext]

        return video_info

    def _real_add_item(self, media):
        '''
        Add description in media to DB
        '''
        if media['type']=='videoItem':
            self._add_videoItem_to_DB(media)

    def _add_videoItem_to_DB(self, media):

        media['title'] = media['title'].strip().title()

        encoding = 'latin-1'
        if not isinstance(media['local_path'], unicode):
            media['local_path'] = unicode(media['local_path'], encoding)
        if not isinstance(media['title'], unicode):
            media['title'] = unicode(media['title'], encoding)

        Facade.addVideoItem(
                            local_path=media['local_path'],
                            title=media['title'],
                           mediaType=media['mediaType'],
                           protocolInfo="http-get:*:"+media['mediaType']+":*")

class FSPhotoAlbum(FSPlugin, Plugin):
    name='fileLocalAlbum'
    use = brisa_config.get_parameter('image', 'usage')
    watch = brisa_config.get_parameter('image', 'watch')
    dirs = brisa_config.get_parameter_as_list('image', 'dirs')
    albums = {}

    def __init__(self, *args, **kwargs):

        FSPlugin.__init__(self, *args, **kwargs)
        Plugin.__init__(self, *args, **kwargs)
        self._parse_albums()

        #TODO:
        # if the user change the name of the album into the file, it will
        # necessary to "re-map" the files stored into the database to the
        # new album name. The complext of doing this is because it is not
        # the add_item is only called if the file is not read and stored
        # into the database
    def _parse_albums(self):
        album_default_index = 1
        image_container = Facade.selectContainerByTitle("My Images")
        if image_container.count() > 0:
            image_container = image_container[0]
            for directory in self.dirs:
                album_file_desc = os.path.join(directory, ".album_name")
                if os.path.isfile(album_file_desc):
                    title = file(album_file_desc).readline()
                else:
                    title = "Album %d" % album_default_index
                    album_default_index = album_default_index + 1
                self.albums[title] = directory
                aAlbum = Facade.selectPhotoAlbumByTitle(title)
                if aAlbum.count() == 0:
                    if not isinstance(title, unicode):
                        name = unicode(title, "latin-1")
                    Facade.addPhotoAlbum(title=title, 
                                                 container=image_container)

    def _is_image_file(self, file):
        '''
        Discovery the tipe of media using the file name.
        '''
        image_types=['.jpg',
                     '.ras',
                     '.xwd',
                     '.bmp',
                     'jpe',
                     '.jpeg',
                     '.xpm',
                     '.ief',
                     '.pbm',
                     '.tif',
                     '.gif',
                     '.ppm',
                     '.xbm',
                     '.tiff',
                     '.rgb',
                     '.pgm',
                     '.png',
                     'pnm']
        (root, ext) = os.path.splitext(file)
        return image_types.__contains__(ext.lower())

    def _get_media_info(self, file):
        '''
        Get the information about the media file.
        Returns:
        media_info - A dictionary containing the
                     information of the media file.
        '''
        media_info = {'type': None}
        if self._is_image_file(file):
            media_info = self._get_image_info(file)
        return media_info

    def _get_image_info(self, file):
        '''
        Get the information about the audio media file.
        Returns:
        image_info - A dictionary containing the
                     information of the media file.
        '''
        image_info = {'type': 'imageItem'}
        image_info['title']=os.path.basename(file)
        image_info['local_path'] = file
        image_info['path'] = ''

        (root, ext) = os.path.splitext(file)
        ext = ext.lower()
        image_info['mediaType']= mimetypes.types_map[ext]

        return image_info

    def _real_add_item(self, media):
        '''
        Add description in media to DB
        '''
        if media['type']=='imageItem':
            self._add_imageItem_to_DB(media)

    def _add_imageItem_to_DB(self, media):

        for (album_name, path) in self.albums.items():
            if media['local_path'].startswith(path):
                album = album_name
                break

        photoAlbum = Facade.selectPhotoAlbumByTitle(album)[0]

        encoding = 'latin-1'
        if not isinstance(media['local_path'], unicode):
            media['local_path'] = unicode(media['local_path'], encoding)
        if not isinstance(media['title'], unicode):
            media['title'] = unicode(media['title'], encoding)

        photo = Facade.addPhoto(
                            local_path=media['local_path'],
                            title=media['title'],
                           mediaType=media['mediaType'],
                           protocolInfo="http-get:*:" + media['mediaType'] + \
                            ":*", dateTaken="1900-01-01")
        photoAlbum.addItem(photo)

genre_codes = {"0": "Blues",
              "1": "Classic Rock",
              "2": "Country",
              "3": "Dance",
              "4": "Disco",
              "5": "Funk",
              "6": "Grunge",
              "7": "Hip-Hop",
              "8": "Jazz",
              "9": "Metal",
              "10": "New Age",
              "11": "Oldies",
              "12": "Other",
              "13": "Pop",
              "14": "R&B",
              "15": "Rap",
              "16": "Reggae",
              "17": "Rock",
              "18": "Techno",
              "19": "Industrial",
              "20": "Alternative",
              "21": "Ska",
              "22": "Death Metal",
              "23": "Pranks",
              "24": "Soundtrack",
              "25": "Euro-Techno",
              "26": "Ambient",
              "27": "Trip-Hop",
              "28": "Vocal",
              "29": "Jazz+Funk",
              "30": "Fusion",
              "31": "Trance",
              "32": "Classical",
              "33": "Instrumental",
              "34": "Acid",
              "35": "House",
              "36": "Game",
              "37": "Sound Clip",
              "38": "Gospel",
              "39": "Noise",
              "40": "AlternRock",
              "41": "Bass",
              "42": "Soul",
              "43": "Punk",
              "44": "Space",
              "45": "Meditative",
              "46": "Instrumental Pop",
              "47": "Instrumental Rock",
              "48": "Ethnic",
              "49": "Gothic",
              "50": "Darkwave",
              "51": "Techno-Industrial",
              "52": "Electronic",
              "53": "Pop-Folk",
              "54": "Eurodance",
              "55": "Dream",
              "56": "Southern Rock",
              "57": "Comedy",
              "58": "Cult",
              "59": "Gangsta",
              "60": "Top 40",
              "61": "Christian Rap",
              "62": "Pop/Funk",
              "63": "Jungle",
              "64": "Native American",
              "65": "Cabaret",
              "66": "New Wave",
              "67": "Psychadelic",
              "68": "Rave",
              "69": "Showtunes",
              "70": "Trailer",
              "71": "Lo-Fi",
              "72": "Tribal",
              "73": "Acid Punk",
              "74": "Acid Jazz",
              "75": "Polka",
              "76": "Retro",
              "77": "Musical",
              "78": "Rock & Roll",
              "79": "Hard Rock",
              "80": "Folk",
              "81": "Folk-Rock",
              "82": "National Folk",
              "83": "Swing",
              "84": "Fast Fusion",
              "85": "Bebob",
              "86": "Latin",
              "87": "Revival",
              "88": "Celtic",
              "89": "Bluegrass",
              "90": "Avantgarde",
              "91": "Gothic Rock",
              "92": "Progressive Rock",
              "93": "Psychedelic Rock",
              "94": "Symphonic Rock",
              "95": "Slow Rock",
              "96": "Big Band",
              "97": "Chorus",
              "98": "Easy Listening",
              "99": "Acoustic",
              "100": "Humour",
              "101": "Speech",
              "102": "Chanson",
              "103": "Opera",
              "104": "Chamber Music",
              "105": "Sonata",
              "106": "Symphony",
              "107": "Booty Bass",
              "108": "Primus",
              "109": "Porn Groove",
              "110": "Satire",
              "111": "Slow Jam",
              "112": "Club",
              "113": "Tango",
              "114": "Samba",
              "115": "Folklore",
              "116": "Ballad",
              "117": "Power Ballad",
              "118": "Rhythmic Soul",
              "119": "Freestyle",
              "120": "Duet",
              "121": "Punk Rock",
              "122": "Drum Solo",
              "123": "A capella",
              "124": "Euro-House",
              "125": "Dance Hall"}