# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from Persistence import *
from Persistence import create_db as persistence_create_db


# Database Manipulation methods


# General Methods


def get_next_object_id():
    """ Returns the next object id according to actual database status.
    """
    queue_manager.put_and_join(('select max(id) from db_object', ))
    res = queue_manager.task_result[0][0]

    if not res:
        return 1
    else:
        return res + 1


def remove_all_invalid_items():
    """ Removes all invalid items (which have the valid attribute set to 0) out
    from the database.
    """
    queue_manager.put_and_join(('DELETE FROM db_item WHERE valid=0', ))


def sync_database():
    """ Initial step for synchronizing system with the database.

    In this step all objects have their 'valid' attributes set to '0'.

    In a later stage, the responsible plugin for each file will see if the data
    on the database is consistent, that is, the file still exists on the system
    and its database information are consistent. If consistent, valid is set
    back to '1' so that consults on the database for this runtime context are
    correct.
    """
    queue_manager.put_and_join(('UPDATE db_item SET valid = 0 where'\
                                ' valid = 1', ))


def create_db():
    """ Creates all tables.
    """
    persistence_create_db()


# Objects


def search_object_by_contain_in_title(title):
    debug('TODO search object by contain in title %s' % title)
    return ''


def search_object_by_exactly_title(title):
    debug('TODO search object by exactly title %s' % title)
    return ''


def search_object_by_description(description):
    debug('TODO search object by description' % description)
    return ''


# Containers


def add_container(title, namespace, parent_container_id=0, local_path=''):
    """Adds a new Container to the database.

    @param title: title
    @param namespace: owner plugin name
    @param parent_container_id: parent container id
    @param local_path: if it represents a folder, it is the path

    @type               title: string
    @type           namespace: string
    @type parent_container_id: integer
    @type          local_path: string

    @return: DBContainer object representing it or None (in case of failure)
    """

    id = get_next_object_id()
    container = None

    debug('add container %s next id is %d' % (title, id))
    debug('add container %s %s %s' % (title, namespace, parent_container_id))


    if parent_container_id > 0:
        parent_container = select_container_by_id(parent_container_id)

        if parent_container:
            debug('add container has parent and selected it correctly')
            container = DBContainer(id=id,
                                    parent_container_id=parent_container_id,
                                    title=title,
                                    namespace=namespace,
                                    parent_container=parent_container)
        else:
            debug('add container has parent and could not select it, creating')
            parent_container = DBContainer(id=parent_container_id,
                                           namespace=namespace)
            parent_container.save()
            container = DBContainer(id=id,
                                    parent_container_id=parent_container_id,
                                    title=title,
                                    namespace=namespace,
                                    parent_container=parent_container)
    else:
        debug('add container root clause')
        container = DBContainer(id=id,
                                parent_container_id=0,
                                title=title,
                                namespace=namespace)

    if not container:
        debug('add container BUG (could not create container)')
        return None

    container.save()

    return container


def select_container_by_title(title, skip_children=True):
    """Select container by title.

    Select a Container with title passed."""
    return get_by_title(title, skip_children)


def remove_container_by_local_path(local_path):
    debug('remove container by local path %s' % local_path)
    queue_manager.put_and_join(('select id from db_container where'\
                               ' local_path=?', (local_path, )))
    res = queue_manager.task_result

    if not res:
        return False

    for r in res:
        id = int(r[0])
        object = get_by_id(id)
        object.delete()


def has_object_by_title(title):
    queue_manager.put_and_join(('SELECT id from db_object WHERE title=?',
                               (title, )))

    items = queue_manager.task_result

    if len(items) > 0:
        debug('has object by title %s: True' % (title))
        return True

    debug('has object by title %s: False' % (title))
    return False


# Items


def select_item_by_local_path(local_path):
    """ Select an item given its local path. Returns a list of DBItem instances
    with data from this select

    @param local_path: item's path
    @type local_path: string

    @return: items selected
    @rtype: list or None
    """

    queue_manager.put_and_join(('SELECT id FROM db_item WHERE local_path=?',
                               (local_path, )))

    try:
        id = queue_manager.task_result[0][0]
        return get_by_id(id)
    except:
        return None


def has_item_by_local_path(local_path, sizebytes=0):
    """Check if local item exist.

    This method analyzes if exist a Item in the specified path.
    Return: count - The number of items found.
    """
    items = []

    debug('local item exist %s %d' % (local_path, sizebytes))

    if sizebytes:
        queue_manager.put_and_join(('SELECT local_path FROM db_item WHERE'\
                                   ' local_path=? and sizebytes=?',
                                   (local_path, sizebytes)))
        items = queue_manager.task_result
        debug('local item exist %s %d: %s' % (local_path, sizebytes,
                                              len(items)>0))
    else:
        queue_manager.put_and_join(('SELECT local_path FROM db_item WHERE'\
                                   ' local_path=?', (local_path, )))
        items = queue_manager.task_result

        debug('local item exist %s: %s' % (local_path, len(items)>0))

    return len(items) > 0


# Genre


def add_music_genre(title, namespace, parent_container_id, path):
    """ Add a music genre to the database

    @param title: container title
    @param namespace: namespace
    @param parent_container_id: id of parent container
    """
    debug('add music genre %s %s %s' % (title, namespace,
                                        str(parent_container_id)))

    music_genre = None

    if not has_object_by_title(title):
        debug('add music genre does not have container')
        # If not created yet, create it
        id = get_next_object_id()
        music_genre = DBMusicGenre(id=id,
                                   name=title,
                                   parent_container_id=parent_container_id,
                                   title=title,
                                   namespace=namespace,
                                   local_path=path)
        music_genre.save()
    return music_genre


def add_movie_genre(title, container=None):
    """ Adds a movie genre to the database
    """

    debug('add movie genre %s %s' % (title, str(container)))

    queue_manager.put_and_join(('INSERT INTO db_movie_genre(type) values'\
                                 '(?)', (title, )))

    return DBContainer(title=title, container=container)


# Music


def add_music_artist(title, namespace, parent_container_id, path):
    """ Adds a music artist to the database
    """
    debug('add music artist %s %s %s' % (title, namespace,
                                         str(parent_container_id)))

    if not has_object_by_title(title):
        music_artist = DBMusicArtist(id=get_next_object_id(),
                                     title=title,
                                     name=title,
                                     namespace=namespace,
                                     parent_container_id=parent_container_id,
                                     local_path=path)
        music_artist.save()
        return music_artist

    return None


def add_music_album(title, namespace, parent_container_id, path):
    """ Adds a music album to the database
    """
    debug('add music album %s %s %s' % (title, namespace,
                                        str(parent_container_id)))

    if not has_object_by_title(title):
        music_album = DBMusicAlbum(id=get_next_object_id(),
                                   title=title,
                                   name=title,
                                   namespace=namespace,
                                   parent_container_id=parent_container_id,
                                   local_path=path)
        music_album.save()
        return music_album

    return None


def add_music_track(**music_data):
    """ Add a music track to the database given a dict with necessary info.
    Adds also genre, artist and album if they're not on the database yet.
    """

    debug('add music track music_template: %s' % str(music_data))

    music_genre = None
    music_artist = None
    music_album = None
    music_all = None

    music_genre_parent = None
    music_audio_artist_parent = None
    music_album_parent = None

    music_genre = get_by_title(music_data['genre'], True, DBMusicGenre)
    if not music_genre:
        music_genre_parent = select_container_by_title('Audio Genres')

        if music_genre_parent:
            music_genre = add_music_genre(music_data["genre"],
                                         music_data['namespace'],
                                         music_genre_parent.id,
                                         music_data['folder_path'])
        else:
            debug('add music track critical 1 | dump: %s' % str(music_data))

    music_artist = get_by_title(music_data['artist'], True, DBMusicArtist)
    if not music_artist:
        music_audio_artist_parent = select_container_by_title('Audio Artists')

        if music_audio_artist_parent:
            music_artist = add_music_artist(music_data['artist'],
                                           music_data['namespace'],
                                           music_audio_artist_parent.id,
                                           music_data['folder_path'])
        else:
            debug('add music track critical 2 | dump: %s' % str(music_data))

    music_album = get_by_title(music_data['album'], True, DBMusicAlbum)
    if not music_album:
        music_album_parent = select_container_by_title('Audio Albums')

        if music_album_parent:
            music_album = add_music_album(music_data['album'],
                                         music_data['namespace'],
                                         music_album_parent.id,
                                         music_data['folder_path'])
        else:
            debug('add music track critical 3 | dump: %s' % str(music_data))

    music_all = select_container_by_title('All Music')

    if not music_all:
        # Perhaps we should create it
        debug('add music track critical 4 | dump %s' % str(music_data))

    # Sets a few important variables
    music_data['id'] = get_next_object_id()
    music_data['parent_container_id'] = music_album.id
    music_data['artist'] = music_artist.name
    music_data['album'] = music_album.name
    music_data['name'] = music_data['title']

    # Add the music track and save it
    music_track = DBMusicTrack(**music_data)
    music_track.save()

    # Add to respective containers
    music_genre.add_item(music_track)
    music_artist.add_item(music_track)
    music_album.add_item(music_track)
    music_all.add_item(music_track)

    # Save containers (this saves the relations between the containers and this
    # item)
    music_genre.save()
    music_artist.save()
    music_album.save()
    music_all.save()

    return music_track


def add_movie(**movie_data):
    """ Add a movie to the database given
    """
    if not has_object_by_title(movie_data['genre']):
        # todo
        movie_genre = add_movie_genre(movie_data['genre'])
    else:
        movie_genre = select_genre_by_title(movie_data['genre'])

    del movie_data["genre"]
    movie_data["movie_genre"] = movie_genre

    movie = DBMovie(**movie_data)
    movie.save()

    movie_genre.add_item(movie)
    movie_genre.save()
    return movie


def add_video_album(title, parent_container_id):
    """ Adds a video album to the database
    """
    debug('add video album %s %s' % (title, str(parent_container_id)))
    video_album = DBVideoAlbum(id=get_next_object_id(),
                                                         title=title,
                                                         name=title,
                                                         parent_container_id=\
                                                         parent_container_id)
    video_album.save()
    return video_album


def add_video_item(**video_data):
    """ Add a video item to the database given a data dict, which must comply
    to this video data dict definition
    """
    debug('add video item video_data: %s' % str(video_data))

    video_container = None
    video_container_all = None

    if not has_object_by_title(video_data['root']):
        parent_container = select_container_by_title('Video Folders')
        video_container = add_video_album(video_data['root'],
                                          parent_container.id)
    else:
        video_container = select_container_by_title(video_data['root'])

    video_container_all = select_container_by_title('All Video')

    if not video_container or not video_container_all:
        debug('add video item bug detected!')
        return None

    video_data['id'] = get_next_object_id()
    video_data['name'] = video_data['title']
    video_data['parent_container_id'] = video_container.id

    video = DBVideoItem(**video_data)
    video.save()

    video_container.add_item(video)
    video_container_all.add_item(video)

    video_container.save()
    video_container_all.save()
    return video


def add_photo_album(title, namespace, parent_container_id=0, path=''):
    """ Add a photo folder/album to the database
    """
    debug('add photo folder %s %s %s' % (title, namespace,
                                         str(parent_container_id)))
    if not has_object_by_title(title):
        photo_album = DBPhotoAlbum(id=get_next_object_id(),
                                   namespace=namespace,
                                   name=title,
                                   parent_container_id=parent_container_id,
                                   local_path=path)
        photo_album.save()
        return photo_album
    else:
        debug('add photo folder did not add because db already has that genre')
        return None


def add_photo(**photo_data):
    """ Add a photo item to the database
    """
    debug('add_photo: %s' % str(photo_data))

    photo_album = None
    photo_container_all = None

    if not has_object_by_title(photo_data['root']):
        parent_container = select_container_by_title("Local Images")
        photo_album = add_photo_album(photo_data['root'],
                                      photo_data['namespace'],
                                      parent_container.id,
                                      photo_data['folder_path'])
    else:
        photo_album = select_container_by_title(photo_data["root"])

    photo_container_all = select_container_by_title("All Pictures")

    photo_data['id'] = get_next_object_id()
    photo_data['parent_container_id'] = photo_album.id
    photo_data['album'] = photo_album.name
    photo = DBPhoto(**photo_data)
    photo.save()

    photo_album.add_item(photo)
    photo_container_all.add_item(photo)

    photo_album.save()
    photo_container_all.save()
    return photo


def remove_item_by_local_path(local_path):
    """ Remove an item from the database given its path
    """
    debug('remove item by local path %s' % local_path)
    queue_manager.put_and_join(('select id from db_item where'\
                               ' local_path=?', (local_path, )))
    res = queue_manager.task_result

    if not res:
        return False

    id = int(res[0][0])
    object = get_by_id(id)

    if object:
        object.delete()

    return True


# Selection methods


def select_audio_list():
    """ Selects audio list
    """
    debug('select audio list')
    try:
        queue_manager.put_and_join(('SELECT title FROM db_object WHERE'\
                                    'type="DBItem"', ))
        return queue_manager.task_result
    except:
        return False


def select_genre_by_title(title):
    """ Selects a genre by its title
    """
    debug('select genre by title %s' % title)
    genre = get_by_title(title)
    return genre
