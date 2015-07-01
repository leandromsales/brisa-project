# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team

from Persistence import *
import sys, os

#-- Manipulation methods

def removeObjectById(id):
    try:
        Object.delete(id)
        return True
    except:
        return False

def addContainer(title, container=None):
    return Container(title=title, container=container)

def removeContainerById(id):
    return removeObjectById(id)

def addGenre(title, container=None):
    return Genre(title=title, container=container)

def removeGenreById(id):
    return removeObjectById(id)

def addMusicGenre(title, container=None):
    return MusicGenre(title=title, container=container)

def removeMusicGenreById(id):
    return removeObjectById(id)

def addMovieGenre(title, container=None):
    return MovieGenre(title=title, container=container)

def removeMovieGenreById(id):
    return removeObjectById(id)

def addMusicArtist(title, container=None):
    return MusicArtist(title=title, container=container)

def removeMusicArtistById(id):
    return removeObjectById(id)

def addMusicAlbum(title, parentMusicAlbum=None, albumArtURI=None, toc=None):
    return MusicAlbum(title=title, parentMusicAlbum=parentMusicAlbum, albumArtURI=albumArtURI, toc=toc)

def removeMusicAlbumById(id):
    return removeObjectById(id)

def addMusicTrack(*args, **kwargs):

    musicGenre = selectGenreByTitle(title=kwargs["genre"])
    if musicGenre.count() > 0:
        musicGenre = musicGenre[0]
    else:
        musicGenre = addMusicGenre(title=kwargs["genre"])
    del kwargs["genre"]
    kwargs["musicGenre"] = musicGenre

    musicArtist = selectMusicArtistByTitle(title=kwargs["artist"])
    if musicArtist.count() > 0:
        musicArtist = musicArtist[0]
    else:
        musicArtist = addMusicArtist(title=kwargs["artist"])
    del kwargs["artist"]
    kwargs["musicArtist"] = musicArtist

    musicAlbum = selectMusicAlbumByTitle(title=kwargs["album"])
    if musicAlbum.count() > 0:
        musicAlbum = musicAlbum[0]
    else:
        musicAlbum = addMusicAlbum(title=kwargs["album"])
    del kwargs["album"]

    musicTrack = MusicTrack(*args, **kwargs)
    musicAlbum.addItem(musicTrack)
    musicGenre.addItem(musicTrack)
    musicArtist.addItem(musicTrack)

    return musicTrack

def removeMusicTrackById(id):
    return removeObjectById(id)

def addMovie(*args, **kwargs):

    movieGenre = selectGenreByTitle(title=kwargs["genre"])
    if genre.count() > 0:
        movieGenre = movieGenre[0]
    else:
        movieGenre = addMovieGenre(title=kwargs["genre"])
    del kwargs["genre"]
    kwargs["movieGenre"] = movieGenre

    return Movie(*args, **kwargs)

def removeMovieById(id):
    return removeObjectById(id)

def addVideoItem(*args, **kwargs):

    videoFolder = selectContainerByTitle("Video Folders")[0]
    videoItem = VideoItem(*args, **kwargs)
    videoFolder.addItem(videoItem)

    return videoItem

def addMusicVideoClip(*args, **kwargs):
    return MusicVideoClip(*args, **kwargs)

def removeMusicVideoClipById(id):
    return removeObjectById(id)

def addText(*args, **kwargs):
    return Text(*args, **kwargs)

def removeTextById(id):
    return removeObjectById(id)

def addPhotoAlbum(*args, **kwargs):
    return PhotoAlbum(*args, **kwargs)

def addPhoto(*args, **kwargs):
    return Photo(*args, **kwargs)

def removePhotoById(id):
    return removeObjectById(id)

def addVideoBroadcast(*args, **kwargs):
    return VideoBroadcast(*args, **kwargs)

def removeVideoBroadcastById(id):
    return removeObjectById(id)

def addAudioBroadcast(*args, **kwargs):
    return AudioBroadcast(*args, **kwargs)

def removeAudioBroadcastById(id):
    return removeObjectById(id)

def addToContainer(parentContainer):
    pass

#-- Selection methods

def selectObjectById(objectId):
    try:
        return Object.get(objectId)
    except:
        return None

def searchObjectByContainInTitle(title):
    criterium = """object.title LIKE '%""" + title + """%'""";
    return Object.select(criterium, clauseTables=['object'])

def searchObjectByExactlyTitle(title):
    return Object.selectBy(title=title)

def searchObjectByDescription(description):
    criterium = """item.description LIKE '%""" + description + """%'""";
    return Object.select(criterium, clauseTables=['item'])

def selectAllGenres():
    return Genre.select()

def selectGenreByTitle(title):
    return Genre.selectBy(title=title)

def selectAllMusicGenre():
    return MusicGenre.select()

def selectMusicGenreByTitle(title):
    return MusicGenre.selectBy(title=title)

def selectAllMovieGenre():
    return MovieGenre.select()

def selectMovieGenreByTitle(title):
    return MusicGenre.selectBy(title=title)

def selectAllMusicArtists():
    return MusicArtist.select()

def selectMusicArtistByTitle(title):
    return MusicArtist.selectBy(title=title)

def selectAllMusicAlbum():
    return MusicAlbum.select()

def selectMusicAlbumByTitle(title):
    return MusicAlbum.selectBy(title=title)

def selectContainerByTitle(title):
    return Container.selectBy(title=title)

if __name__ == "__main__":
    #print selectGenreByTitle("Rock").count()
    #print selectObjectById(5).items
    #genre = Genre.selectBy(title="Rock")
    musicTrack = addMusicTrack(originalTrackNumber=11, composer="Axl Rose", title="Sweet Child O' Mine",date="1993-01-01", copyright="Geffen Records",
              url="http://www.gunsnroses.com/", protocolInfo="http-get:*:audio/mpeg:*",
              description="one of the most known song from GNR", longDescription="",
              publisher="", producer="", protection="", contributor="", relation="", storageMedium="", mediaType="audio/mp3",
              language="English", path="/musics/guns/sweetchildomine.mp3", mediaLength=563, bitRate="192kbps", genre="Rock", artist="Guns", album="apetite for destruction")
