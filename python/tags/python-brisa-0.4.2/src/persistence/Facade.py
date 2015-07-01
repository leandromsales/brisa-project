# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from Persistence import *
import Persistence
import sys
import os
import filecmp


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


def removeItemsOfAContainer(container):
    container.items[:] = []


def removeItemsAndContainerOfAContainer(container):
    removeItemsOfAContainer(container)
    for container in selectContainersOfAContainer(container):
        container.destroySelf()


def selectContainersOfAContainer(container):
    return Container.selectBy(container=container)


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


def addMusicAlbum(title, container=None):
    return MusicAlbum(title=title, container=container)


def removeMusicAlbumById(id):
    return removeObjectById(id)


def addMusicTrack(*args, **kwargs):
    musicGenre = selectGenreByTitle(title=kwargs["genre"])
    if musicGenre.count() > 0:
        musicGenre = musicGenre[0]
    else:
        container = selectContainerByTitle("Audio Genre")[0]
        musicGenre = addMusicGenre(kwargs["genre"], container)
    del kwargs["genre"]
    kwargs["musicGenre"] = musicGenre

    musicArtist = selectMusicArtistByTitle(title=kwargs["artist"])
    if musicArtist.count() > 0:
        musicArtist = musicArtist[0]
    else:
        container = selectContainerByTitle("Audio Artists")[0]
        musicArtist = addMusicArtist(kwargs["artist"], container)
    del kwargs["artist"]
    kwargs["musicArtist"] = musicArtist

    musicAlbum = selectMusicAlbumByTitle(title=kwargs["album"])
    if musicAlbum.count() > 0:
        musicAlbum = musicAlbum[0]
    else:
        container = selectContainerByTitle("Audio Albums")[0]
        musicAlbum = addMusicAlbum(kwargs["album"], container)
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


def removeItemByLocalPath(local_path):
    item = Item.selectBy(local_path=local_path)
    if item.count() > 0:
        item[0].destroySelf()


#-- Selection methods
def selectObjectById(objectId):
    try:
        return Object.get(objectId)
    except:
        return False


def selectItemById(itemId):
    try:
        return Item.get(itemId)
    except:
        return False


def searchObjectByContainInTitle(title):
    criterium = """object.title LIKE '%""" + title + """%'"""
    return Object.select(criterium, clauseTables=['object'])


def searchObjectByExactlyTitle(title):
    return Object.selectBy(title=title)


def searchObjectByDescription(description):
    criterium = """item.description LIKE '%""" + description + """%'"""
    return Object.select(criterium, clauseTables=['item'])


def selectAllItems():
    return Item.select()


def selectAllGenres():
    return Genre.select()


def selectGenreByTitle(title):
    return Genre.selectBy(title=title)


def selectPhotoAlbumByTitle(title):
    return PhotoAlbum.selectBy(title=title)


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


def selectItemByLocalPath(local_path):
    return Item.selectBy(local_path=local_path)


def localItemExist(local_path, sizebytes=None):
    if sizebytes != None:
        count = Item.select(AND(Item.q.local_path == \
                local_path, Item.q.sizebytes == sizebytes)).count()
    else:
        count = Item.select((Item.q.local_path) == \
                local_path).count()
    return (count > 0)


def makeAllObjectsInvalid():
    from sqlobject.sqlbuilder import Update
    database = Item._connection
    valid_field = Item.q.valid.fieldName
    updatestatement = database.sqlrepr(Update(Item.q, {valid_field: "0"}))
    #database.debug = True
    database.query(updatestatement)
    database.cache.clear()
    #database.debug = False


def removeAllInvalidItems():
    #from sqlobject.sqlbuilder import Delete
    #database = Item._connection
    #deletestatement = database.sqlrepr(Delete(Item.q, where=(Item.q.valid == '0')))
    #database.debug = True
    #database.query(deletestatement)
    #database.cache.clear()
    #database.debug = False
    items = Item.selectBy(valid=0)
    for item in items:
        item.destroySelf()

def sync_database():
    container = selectContainerByTitle("Audio Broadcast")
    if container.count() > 0:
        removeItemsAndContainerOfAContainer(container[0])

    container = selectContainerByTitle("Video Broadcast")
    if container.count() > 0:
        removeItemsAndContainerOfAContainer(container[0])

    container = selectContainerByTitle("Internet Photos")
    if container.count() > 0:
        removeItemsAndContainerOfAContainer(container[0])

    makeAllObjectsInvalid()

def notify_shared_url(shared_url):
    Persistence._shared_url = shared_url


def createDB():
    Persistence.createDB()