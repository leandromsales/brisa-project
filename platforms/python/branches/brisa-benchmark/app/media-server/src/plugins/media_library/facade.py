# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Database facade for the Media Library plugin.
"""

from brisa.upnp.didl.didl_lite import *
from brisa.utils.sqlite import SQLWorkerThread, SQLQueueManager
from brisa.services.web_server import WebServer


worker_thread = None
queue_manager = None


def init(db_path):
    """ Initializes the facade database scheme (sqlite queue manager, worker
    thread).

    Must be called before using the facade.
    """
    global worker_thread, queue_manager
    worker_thread = SQLWorkerThread(db_path)
    queue_manager = SQLQueueManager()
    worker_thread.set_sql_queue_manager(queue_manager)
    worker_thread.start()


def get_file_path(id):
    """ Returns the file path with this id.
    """
    queue_manager.put_and_join(('SELECT path FROM files WHERE id=?',
                               (id, )))
    try:
        return str(queue_manager.task_result[0][0])
    except:
        return ''


def get_audio_item(id):
    """ Returns the audio item (if exists) given its id.
    """
    url = '%s/media_library/audio-' % WebServer().listen_url
    queue_manager.put_and_join(('SELECT files.path, audios.id, '\
                                'audios.title, audio_albums.name, '\
                                'audio_artists.name, audio_genres.name, '\
                                'audios.trackno FROM audios INNER JOIN '\
                                'audio_albums INNER JOIN audio_artists '\
                                'INNER JOIN audio_genres INNER JOIN files ON '\
                                'audios.album_id=audio_albums.id AND '\
                                'audios.genre_id=audio_genres.id AND '\
                                'audios.artist_id=audio_artists.id '\
                                'where files.id=? AND audios.id=?',
                                (id, id)))
    result = queue_manager.task_result

    if not result:
        return []

    r = result[0]
    track = MusicTrack('media_library:audio-%d' % r[1], '', r[2], genres=[r[5]],
                       artists=[r[4]], albums=[r[3]],
                       original_track_number=r[6])
    track.add_resource(Resource('%s%d' % (url, r[1]), 'http-get:*:audio/%s:*' %
                       str(r[0]).split('.')[-1]))
    return [track]


def get_audio_all():
    """ Returns all audio.
    """
    url = '%s/media_library/audio-' % WebServer().listen_url
    queue_manager.put_and_join(('SELECT files.path, audios.id, '\
                                'audios.title, audio_albums.name, '\
                                'audio_artists.name, audio_genres.name, '\
                                'audios.trackno FROM audios INNER JOIN '\
                                'audio_albums INNER JOIN audio_artists '\
                                'INNER JOIN audio_genres INNER JOIN files ON '\
                                'audios.album_id=audio_albums.id AND '\
                                'audios.genre_id=audio_genres.id AND '\
                                'audios.artist_id=audio_artists.id AND '\
                                'audios.id = files.id', ()))

    rows = queue_manager.task_result
    ans = []

    for r in rows:
        track = MusicTrack('media_library:audio-%d' % r[1], '', r[2],
                           genres=[r[5]], artists=[r[4]], albums=[r[3]],
                           original_track_number=r[6])
        track.add_resource(Resource('%s%d' % (url, r[1]),
                           'http-get:*:audio/%s:*' % str(r[0]).split('.')[-1]))
        ans.append(track)

    return ans


def get_audio_genres():
    """ Returns all audio genres.
    """
    queue_manager.put_and_join(('SELECT ? || id, name FROM audio_genres',
                                ('media_library:genre-', )))

    rows = queue_manager.task_result
    result = []

    for r in rows:
        if r[1] == 'genre':
            r = (r[0], 'Unknown Genre')
        result.append(MusicGenre(**dict(zip(['id', 'title'], r))))

    return result


def get_audio_genre(id):
    """ Returns an audio genre given its id.
    """
    url = '%s/%s/audio-' % (WebServer().listen_url, 'media_library')
    queue_manager.put_and_join(('SELECT files.path, audios.id, '\
                                'audios.title, audio_albums.name, '\
                                'audio_artists.name, audio_genres.name, '\
                                'audios.trackno FROM audios INNER JOIN '\
                                'audio_albums INNER JOIN audio_artists '\
                                'INNER JOIN audio_genres INNER JOIN files ON '\
                                'audios.album_id=audio_albums.id AND '\
                                'audios.genre_id=audio_genres.id AND '\
                                'audios.artist_id=audio_artists.id AND '\
                                'audios.id = files.id WHERE audios.genre_id '\
                                '= ?', (id, )))
    rows = queue_manager.task_result
    ans = []

    for r in rows:
        track = MusicTrack('media_library:audio-%d' % r[1], '', r[2],
                           genres=[r[5]], artists=[r[4]], albums=[r[3]],
                           original_track_number=r[6])
        track.add_resource(Resource('%s%d' % (url, r[1]),
                           'http-get:*:audio/%s:*' % str(r[0]).split('.')[-1]))
        ans.append(track)

    return ans

def get_audio_artists():
    """ Returns all audio artists.
    """
    queue_manager.put_and_join(('SELECT ? || id, name FROM audio_artists',
                                ('media_library:artist-', )))

    rows = queue_manager.task_result

    return [MusicArtist(**dict(zip(['id', 'title'], r))) for r in rows]


def get_audio_artist(id):
    """ Returns an audio artist given its id.
    """
    url = '%s/%s/audio-' % (WebServer().listen_url, 'media_library')
    queue_manager.put_and_join(('SELECT files.path, audios.id, '\
                                'audios.title, audio_albums.name, '\
                                'audio_artists.name, audio_genres.name, '\
                                'audios.trackno FROM audios INNER JOIN '\
                                'audio_albums INNER JOIN audio_artists '\
                                'INNER JOIN audio_genres INNER JOIN files ON '\
                                'audios.album_id=audio_albums.id AND '\
                                'audios.genre_id=audio_genres.id AND '\
                                'audios.artist_id=audio_artists.id AND '\
                                'audios.id = files.id WHERE audios.artist_id '\
                                '= ?', (id, )))

    rows = queue_manager.task_result
    ans = []

    for r in rows:
        track = MusicTrack('media_library:audio-%d' % r[1], '', r[2],
                           genres=[r[5]], artists=[r[4]], albums=[r[3]],
                           original_track_number=r[6])
        track.add_resource(Resource('%s%d' % (url, r[1]), 'http-get:*:audio/%s:*' %
                                    str(r[0]).split('.')[-1]))
        ans.append(track)

    return ans


def get_audio_albums():
    """ Returns all audio albums.
    """
    queue_manager.put_and_join(('SELECT ? || id, name FROM audio_albums',
                                ('media_library:album-', )))

    rows = queue_manager.task_result

    return [MusicAlbum(**dict(zip(['id', 'title'], r))) for r in rows]


def get_audio_album(id):
    """ Returns an audio album given its id.
    """
    url = '%s/%s/audio-' % (WebServer().listen_url, 'media_library')
    queue_manager.put_and_join(('SELECT files.path, audios.id, '\
                                'audios.title, audio_albums.name, '\
                                'audio_artists.name, audio_genres.name, '\
                                'audios.trackno FROM audios INNER JOIN '\
                                'audio_albums INNER JOIN audio_artists '\
                                'INNER JOIN audio_genres INNER JOIN files ON '\
                                'audios.album_id=audio_albums.id AND '\
                                'audios.genre_id=audio_genres.id AND '\
                                'audios.artist_id=audio_artists.id AND '\
                                'audios.id = files.id WHERE audios.album_id '\
                                '= ?', (id, )))

    rows = queue_manager.task_result
    ans = []

    for r in rows:
        track = MusicTrack('media_library:audio-%d' % r[1], '', r[2],
                           genres=[r[5]], artists=[r[4]], albums=[r[3]],
                           original_track_number=r[6])
        track.add_resource(Resource('%s%d' % (url, r[1]),
                           'http-get:*:audio/%s:*' % str(r[0]).split('.')[-1]))
        ans.append(track)

    return ans


def get_images_local():
    """ Returns all local images.
    """
    url = '%s/%s/image-' % (WebServer().listen_url, 'media_library')
    queue_manager.put_and_join(('SELECT images.id, images.title, files.path '\
                                'FROM images INNER JOIN files ON images.id '\
                                '= files.id', ()))
    rows = queue_manager.task_result
    ans = []

    for r in rows:
        item = ImageItem('media_library:image-%d' % r[0], title=r[1])
        item.add_resource(Resource('%s%d' % (url, r[0]), 'http-get:*:image/%s:*' %
                          str(r[2]).split('.')[-1]))
        ans.append(item)

    return ans


def get_image_item(id):
    """ Returns an image given its id.
    """
    url = '%s/media_library/image-' % WebServer().listen_url
    queue_manager.put_and_join(('SELECT images.id, images.title, files.path '\
                                'FROM images INNER JOIN files ON images.id '\
                                '= files.id WHERE images.id=?', (id, )))
    rows = queue_manager.task_result
    ans = []

    if not rows or len(rows) < 1:
        return []

    r = rows[0]

    item = ImageItem('media_library:image-%d' % r[0], title=r[1])
    item.add_resource(Resource('%s%d' % (url, r[0]), 'http-get:*:image/%s:*' %
                      str(r[2]).split('.')[-1]))

    return [item]


def get_videos_local():
    """ Returns all local videos.
    """
    url = '%s/media_library/video-' % WebServer().listen_url
    queue_manager.put_and_join(('SELECT videos.id, videos.title, '\
                                'videos.artist, files.path FROM videos '\
                                'INNER JOIN files ON videos.id = files.id',
                               ()))
    rows = queue_manager.task_result
    ans = []

    for r in rows:
        item = VideoItem('media_library:video-%d' % r[0], title=r[1],
                         actors=[r[2]])
        item.add_resource(Resource('%s%d' % (url, r[0]), 'http-get:*:video/%s:*' %
                          str(r[3]).split('.')[-1]))
        ans.append(item)

    return ans


def get_video_item(id):
    """ Returns a video item given its id.
    """
    url = '%s/media_library/video-' % WebServer().listen_url
    queue_manager.put_and_join(('SELECT videos.id, videos.title, '\
                                'videos.artist, files.path FROM '\
                                'videos INNER JOIN files ON videos.id = '\
                                'files.id WHERE videos.id=?', (id, )))
    rows = queue_manager.task_result

    if not rows or len(rows) < 1:
        return []

    item = VideoItem('media_library:video-%d' % r[0], title=r[1], actors=[r[2]])
    item.add_resource(Resource('%s%d' % (url, r[0]), 'http-get:*:video/%s:*' %
                               str(r[3]).split('.')[-1]))
    return [item]


def get_playlists():
    """ Returns all playlists.
    """
    url = '%s/media_library/playlist-' % WebServer().listen_url
    ans = []

    queue_manager.put_and_join(('SELECT id, title FROM playlists', ()))
    rows = queue_manager.task_result

    for r in rows:
        playlist = PlaylistItem('media_library:playlist-%d' % r[0], title=r[1])
        playlist.add_resource(Resource('%s%d' % (url, r[0]),
                              'http-get:*:audio/mpegurl:*'))
        ans.append(playlist)

    return ans


def get_playlist(id):
    """ Returns a single playlist item based on its id.
    """
    url = '%s/media_library/playlist-' % WebServer().listen_url
    queue_manager.put_and_join(('SELECT title FROM playlists WHERE id=?',
                                (id, )))
    rows = queue_manager.task_result

    if not rows or len(rows) < 1:
        return []

    playlist = PlaylistItem('media_library:playlist-%d' % id, title=rows[0][0])
    playlist.add_resource(Resource('%s%d' % (url, id),
                          'http-get:*:audio/mpegurl:*'))

    return [playlist]
