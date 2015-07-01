# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.services.cds.plugins.SQLWorkerThread import SQLWorkerThread
from brisa.services.cds.plugins.SQLQueueManager import SQLQueueManager
from brisa.services.web_server import WebServer
from brisa.upnp.didl.didl_lite import *
from brisa.threading import ThreadManager
from brisa import log, config


# If one wants to disable database debugging...


DEBUG = False


def debug(msg):
    # Debug wrapper for database code
    if DEBUG:
        log.debug('Database access: %s' % msg)


# SQL queries queue and worker thread start


queue_manager = SQLQueueManager()
worker_thread = SQLWorkerThread(config.get_parameter('persistence',
                                             'connection').split('sqlite:')[1])
worker_thread.set_sql_queue_manager(queue_manager)
worker_thread.start()


# Database structuring in a dictionary: easier to maintain. This database
# structure is used for retrieving valid columns for saving the objects later.
table_data = {'DBObject': ('db_object', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('parent_container_id', 'INTEGER,'),
                                    ('title', 'VARCHAR(100),'),
                                    ('namespace', 'VARCHAR(30),'),
                                    ('type', 'VARCHAR(255)'))),
                   'DBContainer': ('db_container', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('parent_container_id', 'INTEGER,'),
                                    ('type', 'VARCHAR(255),'),
                                    ('local_path', 'VARCHAR(255)'))),
                   'DBItem': ('db_item', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('date', 'VARCHAR(10),'),
                                    ('copyright', 'VARCHAR(100),'),
                                    ('protocolInfo', 'VARCHAR(255),'),
                                    ('local_path', 'VARCHAR(1000),'),
                                    ('rating', 'INT,'),
                                    ('description', 'VARCHAR(100),'),
                                    ('longDescription', 'TEXT,'),
                                    ('publisher', 'VARCHAR(100),'),
                                    ('producer', 'VARCHAR(50),'),
                                    ('author', 'VARCHAR(50),'),
                                    ('protection', 'VARCHAR(50),'),
                                    ('contributor', 'VARCHAR(50),'),
                                    ('relation', 'VARCHAR(50),'),
                                    ('storage_medium', 'VARCHAR(50),'),
                                    ('media_type', 'VARCHAR(25),'),
                                    ('duration', 'VARCHAR(25),'),
                                    ('sizebytes', 'VARCHAR(32),'),
                                    ('valid', 'VARCHAR(1),'),
                                    ('language', 'VARCHAR(50),'),
                                    ('bitrate', 'VARCHAR(25),'),
                                    ('rights', 'VARCHAR(25),'),
                                    ('genre', 'INT,'),
                                    ('root', 'VARCHAR(50),'),
                                    ('samplerate', 'VARCHAR(25),'),
                                    ('resolution', 'VARCHAR(25),'),
                                    ('type', 'VARCHAR(255)'),
                                    )),
                   'DBAudioItem': ('db_audio_item', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('region', 'INT,'),
                                    ('channel_nr', 'INT,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBMusicTrack': ('db_music_track', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('original_track_number', 'INT,'),
                                    ('composer', 'VARCHAR(100),'),
                                    ('artist', 'VARCHAR(100),'),
                                    ('album', 'VARCHAR(100),'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBGenre': ('db_genre', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBPerson': ('db_person', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBMusicArtist': ('db_music_artist', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'type VARCHAR(255)')
                                    )),
                   'DBMusicGenre': ('db_music_genre', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBAlbum': ('db_album', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBMusicAlbum': ('db_music_album', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('album_art_uri', 'VARCHAR(150),'),
                                    ('name', 'VARCHAR(255),'),
                                    ('toc', 'VARCHAR(50)')
                                    )),
                   'DBPhotoAlbum': ('db_photo_album', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBVideoItem': ('db_video_item', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBMovie': ('db_movie', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('dvd_region_code', 'INT,'),
                                    ('channel_name', 'VARCHAR(50),'),
                                    ('scheduled_start_time', 'TIMESTAMP,'),
                                    ('scheduled_end_time', 'TIMESTAMP,'),
                                    ('actor', 'VARCHAR(50),'),
                                    ('director', 'VARCHAR(20),'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBMusicVideoClip': ('db_music_video_clip', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('scheduled_start_time', 'TIMESTAMP,'),
                                    ('scheduled_end_time', 'TIMESTAMP,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBImageItem': ('db_image_item', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBPhoto': ('db_photo', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('album', 'VARCHAR(50),'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBVideoAlbum': ('db_video_album', (
                                    ('id', 'INTEGER PRIMARY KEY,'),
                                    ('name', 'VARCHAR(255)')
                                    )),
                   'DBRelations': ('db_container_db_item', (
                                    ('db_container_id', 'INT NOT NULL,'),
                                    ('db_item_id', 'INT NOT NULL')
                                    ))}


def create_db():
    """ Creates the database based on the dictionary table_data. Any
    structural changes on database should be done on table_data.
    """
    debug('creating database')
    for k, v in table_data.items():
        debug('Database access: creating table %s' % k)

        command = 'CREATE TABLE %s(' % v[0]
        keys = v[1]

        for k in keys:
            command += '%s %s' % (k[0], k[1])

        command += ')'

        queue_manager.put_and_join((command, ))


# Save/retrieve related methods


def table_update(o, attributes, table_name):
    """ Updates an object's data on the database. It must have been already
    saved.

    This function is called on internal parts of the save mechanism. This means
    you should not call this directly. Please refer to class DBStorageMethod
    documentation for further explanations.

    @param o: object instance
    @param attributes: attributes for this object that have columns on the
                       database
    @param table_name: table in which this object will be updated

    @type attributes: list
    @type table_name: string
    """
    if not o.saved:
        debug('table update object not saved yet. Misuse of'\
              'DBStorageMethod.save()')

    command = 'UPDATE %s SET ' % table_name

    arg_list = []

    for attr in attributes:
        value = getattr(o, attr)

        if attributes[-1] != attr:
            if value == None:
                value = ''
            command += '%s=?, ' % (attr)
            arg_list.append(value)
        else:
            command += '%s=? ' % (attr)
            arg_list.append(value)

    arg_list.append(o.id)
    command += 'WHERE id=?'

    if hasattr(o, 'items') and hasattr(o, 'containers'):
        debug('table update save container')
        for item in o.items:
            if not item.saved:
                raise Exception('Container has item not saved. Dump: %s' %
                                str(item))
            queue_manager.put_and_join(('SELECT count(*) from '\
                'db_container_db_item where db_container_id=? and '\
                'db_item_id=?', (o.id, item.id)))
            count = queue_manager.task_result[0][0]

            if count <= 0:
                debug('table update item relation insert %s<->%s' % (o.id,
                                                                     item.id))
                queue_manager.put_and_join(('INSERT INTO db_container_db_item'\
                ' (db_container_id, db_item_id) values (?,?)',
                                           (o.id, item.id)))

        for container in o.containers:
            container.save()
            queue_manager.put_and_join(('UPDATE db_container SET '\
                'parent_container_id=? WHERE id=?', (str(container.id),
                                                           str(o.id))))
    else:
        pass
    debug('table update %s' % command)
    queue_manager.put_and_join((command, tuple(arg_list)))


def table_add(o, attributes, table_name):
    """  Adds an object into the database.

    This function is called on internal parts of the save mechanism. This means
    you should not call this directly. Please refer to class DBStorageMethod
    documentation for further explanations.

    @param o: object instance
    @param attributes: attributes for this object that have columns on the
                       database
    @param table_name: table in which this object will be added

    @type attributes: list
    @type table_name: string
    """
    # Refactor these iterations
    # they can be done in the same 'for'
    if o.saved:
        debug('table add object already saved. Misuse on \
        DBStorageMethod.save()')
        return

    command = 'INSERT INTO %s (' % table_name

    arg_list = []

    for attr in attributes:
        if attr != attributes[-1]:
            command += '%s, ' % attr
        else:
            command += '%s) values (' % attr

    for attr in attributes:
        value = getattr(o, attr)
        if value == None:
            value = ''

        arg_list.append(value)

        if attr != attributes[-1]:
            command += '?, '
        else:
            command += '?)'


    if hasattr(o, 'items') and hasattr(o, 'containers'):
        debug('table add container items and containers')
        for item in o.items:
            if not item.saved:
                raise Exception('Container has item not saved. Dump: %s' %
                                str(item))

            debug('table add item relation insert %d<->%d' % (o.id, item.id))
            queue_manager.put_and_join(('INSERT INTO db_container_db_item '\
               '(db_container_id, db_item_id) values (?,?)', (o.id, item.id)))

        for container in o.containers:
            if not container.saved:
                raise Exception('Container has container not saved. Dump: %s' %
                                str(container))
            queue_manager.put_and_join(('UPDATE db_container SET '\
                'parent_container_id=? WHERE id=?', (str(container.id),
                                                           str(o.id))))

    debug('table add %s' % command)
    queue_manager.put_and_join((command, tuple(arg_list)))


def get_by_id(id, cls=None, skip_children=True, starting_index=0,
              requested_count=None, sort_criteria=''):
    """ Retrieves an object of the table given its id.

    General parameters: these parameters apply to all objects
    @param id: row id in the table

    Container parameters: these parameters apply to Container objects
    @param skip_children: if True, then its sub-containers and items will be
                          skipped
    @param starting_index: if skip children is False, then item retrieval will
                           begin at this index
    @param requested_count: number of items to retrieve (starting at
                            starting_index)
    @param sort_criteria: sorting criteria

    @type id: integer
    @type skip_children: boolean
    @type starting_index: integer
    @type requested_count: integer
    @type sort_criteria: string

    @return: a DB* associated with the given id or None
    """
    if not cls:
        cls = get_class_from_database_id(id)
        if not cls:
            return False

    debug('get by id %s %d' % (cls, int(id)))
    top_level_table = table_data[cls.__name__][0]
    top_level_cls = table_classes[cls.__name__]

    inherited_classes = get_inherited_classes(cls)
    tables_to_select = []

    for i in inherited_classes:
        table = table_data[i][0]
        tables_to_select.append(table)

    tables_to_select.append(top_level_table)

    data_dict = {}

    for table in tables_to_select:
        queue_manager.put_and_join(('select * from %s where id=?' % table,
                                   (id, )))
        items = queue_manager.task_result
        if len(items) <= 0:
            continue
        item = items[0]

        for k, v in table_data.items():
            if v[0] == table:
                table_attribute_keys = [k[0] for k in v[1]]
                break

        if not table_attribute_keys:
            debug('fatal bug')
            return

        for key in table_attribute_keys:
            key_index = table_attribute_keys.index(key)
            data_dict[key] = item[key_index]

    try:
        inst = top_level_cls(**data_dict)
        if is_container(class_reference=top_level_cls) and not skip_children:
            # If limit was specified
            limit_str = ''

            if requested_count:
                limit_str = 'LIMIT ? OFFSET ?'
            if sort_criteria:
                sort_criteria = 'ORDER BY ? '
            else:
                sort_criteria = ''

            criteria = ''.join([sort_criteria, ' ', limit_str])

            # Add sub-containers
            queue_manager.put_and_join(('select id,type from db_container '\
                                       'where parent_container_id=?',
                                       (inst.id, )))
            containers = queue_manager.task_result
            for c in containers:
                cls_ref = table_classes[str(c[1])]
                if is_container(class_reference=cls_ref):
                    instance = get_by_id(c[0], cls_ref)
                    if instance:
                        inst.add_container(instance)

            arg = [inst.id]
            if criteria:
                if sort_criteria:
                    arg.append(sort_criteria)
                if limit_str:
                    arg.append(requested_count)
                    arg.append(starting_index)

            # Add items

            queue_manager.put_and_join(('select db_item.id,db_item.type from '\
                                       'db_object,db_item where db_object.id'\
                                       '=db_item.id and '\
                                       'db_object.parent_container_id=? %s' %
                                       criteria,
                                       tuple(arg)))
            items = queue_manager.task_result

            items_added = []  # List with item ids already added
            for i in items:
                cls_ref = table_classes[str(i[1])]
                if is_item(class_reference=cls_ref):
                    instance = get_by_id(i[0], cls_ref)
                    if instance:
                        inst.add_item(instance)
                        items_added.append(i[0]) # Set that we added
                else:
                    debug('get by id problem getting subitems')

            # Add items with multiple parents
            queue_manager.put_and_join(('select db_item_id from '\
               'db_container_db_item where db_container_id=? %s' %
                             criteria,
                             tuple(arg)))
            items = queue_manager.task_result

            for i in items:
                if i[0] not in items_added:
                    instance = get_by_id(i[0])
                    if instance:
                        inst.add_item(instance)

        inst.saved = True
        return inst
    except Exception, e:
        debug('BUG @ get by id %d | %s' % (id, str(e)))
        return None


def get_by_title(title, skip_children=True, force_class=None):
    """ Returns the first object that matches this title on the database.

    Note that this function does not return only the type DBObject. Given the
    title, the type is guessed from data already on the database. For example,
    if a DBMusicTrack title is passed, you will get a DBMusicTrack instance.

    @param title: title of the object
    @type title: string

    @return: corresponding instance for the match or None
    @rtype: depends on the match
    """
    debug('get by title: %s' % title)

    queue_manager.put_and_join(('select id,type from db_object where '\
                                'title=?', (title, )))
    try:
        id, type = queue_manager.task_result[0]
        if force_class:
            return get_by_id(id, force_class, skip_children)
        return get_by_id(id, table_classes[type], skip_children)
    except:
        return None


def get_class_from_database_id(id):
    debug('get class by database id: %s' % str(id))
    queue_manager.put_and_join(('select type from db_object where id=?',\
                               (str(id), )))
    result = queue_manager.task_result
    try:
        return table_classes[str(result[0][0])]
    except Exception, e:
        debug('get class from database id %s' % str(e))
        return None


def get_inherited_classes(cls):
    """ Returns a list of classes from which the given class inherits (directly
    or and indirectly).

    @param cls: class reference

    @return: list containing the inherited classes of 'cls'
    @rtype: list
    """
    p = True
    l = []
    while p:
        p = cls.__parent__
        if p:
            l.append(p.__name__)

        cls = cls.__parent__
    return l


def is_container(class_name='', class_reference=None):
    """ Returns whether the class IS a DBContainer or not.

    Recommended use is to pass only one of the parameters. Passing the class
    name and the class reference is ambiguous but will work.

    @param class_name: string
    @param class_reference: type

    @return: whether is a container or not
    @rtype: boolean
    """
    if not class_name and not class_reference:
        # We don't have any info
        return False

    if class_name and not class_reference:
        class_reference = table_classes[class_name]

    if class_reference:
        # We have the reference, we can retrieve the requested info
        # No need to check for class name since this class ref has it already
        if 'DBContainer' in get_inherited_classes(class_reference) or\
            class_reference.__name__ == 'DBContainer':
            return True
        return False

    # Should never reach here
    debug('is container condition bug')
    return False


def is_item(class_name='', class_reference=None):
    """ Returns whether the class IS a DBItem or not.

    Recommended use is to pass only one of the parameters. Passing the class
    name and the class reference is ambiguous but will work.

    @param class_name: string
    @param class_reference: type

    @return: whether is a item or not
    @rtype: boolean
    """
    if not class_name and not class_reference:
        # We don't have any info
        return False

    if class_name and not class_reference:
        class_reference = table_classes[class_name]

    if class_reference:
        # We have the reference, we can retrieve the requested info
        # No need to check for class name since this class ref has it already
        if 'DBItem' in get_inherited_classes(class_reference) or\
           class_reference.__name__ == 'DBItem':
            return True
        return False

    # Should never reach here
    debug('is item condition bug')
    return False


# Save/Delete method model


class DBStorageMethod(object):
    """ Generic storage method for sqlite3 database objects. This is supposed
    to work on DB* classes declared both in table_data and table_classes
    (See beginning and end of this file for reference).

    Supposing you have a DB* class instance which is associated with data on
    the database, say an unique ID, this storage method provides a way to save
    and know if your memory object has already been saved on the database.

        - save: saves/updates your object on the database according to the
    situation. If not yet in table, it's inserted, or else it will be
    updated with data on your object's attributes.

        - save_parents: saves classes on your class hierarchy. All classes the
    hierarchy must inherit from DBStorageMethod so that they behave correctly.

    Practical example: DBMusicTrack
    When a DBMusicTrack has data stored on the database, data must be split
    in different tables and columns.

    Those tables are specifically db_music_track, db_audio_item, db_item
    and db_object which have respectively associated classes DBMusicTrack,
    DBAudioItem, DBItem, DBObject.

    Suppose you have a DBMusicTrack instance with a new track that is not
    yet on the database. If you save this music track with save(), its data
    must be split on the different tables, so there's a call for all
    inherited classes save() methods (DBAudioItem, DBItem and DBObject).
    This way data gets split on all tables as expected.
    """
    # This flag tells if the object is already in the database
    saved = False

    # Modifying this flag might break things. This flags tells save() if
    # it is the root (the save you called). If True, then save will call
    # save_parents (which is an iterative method) and set the flag to False.
    # For each parent the save() method is called, but since this flag is now
    # False (and all parents are the same instance as the root one),
    # save_parents wont be called again.
    _root_save = True

    def save(self):
        """ Saves an object data on the sqlite3 database.
        """
        cls = self.__name__

        debug('save object of class %s' % cls)
        # Use data from table_data to get object parameters name. Using dir is
        # not reliable since it has lots of attributes that don't matter

        if cls not in table_data:
            debug('save object: class %s not in table_data' % cls)
            return

        table_name = table_data[cls][0]

        # Get attributes
        attributes = []

        for t in table_data[cls][1]:
            attributes.append(t[0])

        debug('save object attributes: %s' % str(attributes))

        if self.saved:
            debug('save object in table! updating it')
            table_update(self, attributes, table_name)
        else:
            debug('save object not in table! inserting it')
            table_add(self, attributes, table_name)

        if self._root_save:
            debug('saving object parent!')
            self._root_save = False
            self.save_parents()
        else:
            debug('skipping object parent save')

    def delete(self):
        """ Removes this object from the database but does not remove from
        memory, so that it can be saved again, if needed.
        """
        if not self.saved:
            debug('Could not delete because it has not been saved yet.')

        cls = table_classes[self.__name__]
        inherited_classes = get_inherited_classes(cls)
        inherited_classes.append(self.__name__)
        tables_to_delete = []

        for i in inherited_classes:
            table = table_data[i][0]
            tables_to_delete.append(table)

        # Get real id
        id = self.id

        if type(self.id) == str and ':' in self.id:
            # Format namespace:id
            id = self.id.split(':')[1]

        for t in tables_to_delete:
            queue_manager.put_and_join(("delete from %s where id=?" % t,
                                         (id, )))

        if is_container(self.__name__):
            queue_manager.put_and_join(('select db_item_id from'\
                                         ' db_container_db_item where'\
                                         ' db_container_id=?', (id, )))
            ans = queue_manager.task_result

            if ans:
                for t in ans:
                    instance = get_by_id(t[0])
                    if instance:
                        instance.delete()
                        queue_manager.put_and_join(('delete from'\
                                               ' db_container_db_item where'\
                                               ' db_item_id=?', (t[0], )))

    def save_parents(self):
        """ Saves iteratively parent classes.

        This method must not be called directly.
        """
        base_name = self.__name__
        debug('save parents for %s' % base_name)
        for cls in get_inherited_classes(self.__class__):
            cls_ref = table_classes[cls]
            if hasattr(cls_ref, '__parent__'):
                self.__name__ = cls_ref.__name__
                self.save()
            else:
                debug('skipping save, no parent for %s' % self.__name__)
                break

        self._root_save = True
        self.__name__ = base_name
        self.saved = True

    def is_stored(self, table):
        """ Returns whether this object is already stored on the table or not
        """
        debug('is in table self.id class: %s' % self.id.__class__)
        queue_manager.put_and_join(('select count(*) from ? where id=?',
                                     (table, self.id)))
        taskres = queue_manager.task_result
        return taskres[0][0] > 0


# Database representative classes


class DBObject(Object, DBStorageMethod):

    __name__ = 'DBObject'
    __parent__ = None

    def __init__(self, *args, **kwargs):
        Object.__init__(self, *args, **kwargs)
        self.type = self.__name__


class DBContainer(Container, DBObject, DBStorageMethod):

    __name__ = 'DBContainer'
    __parent__ = DBObject

    def __init__(self, *args, **kargs):
        Container.__init__(self, *args, **kargs)
        DBObject.__init__(self, *args, **kargs)
        self.type = self.__name__

    def add_item(self, item):
        if item not in self.items:
            self.items.append(item)

    def add_container(self, container):
        if container not in self.containers:
            self.containers.append(container)


class DBItem(Item, DBObject, DBStorageMethod):

    __name__ = 'DBItem'
    __parent__ = DBObject

    def __init__(self, *args, **kargs):
        Item.__init__(self, *args, **kargs)
        DBObject.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBAudioItem(AudioItem, DBItem, DBStorageMethod):

    __name__ = 'DBAudioItem'
    __parent__ = DBItem

    def __init__(self, *args, **kargs):
        AudioItem.__init__(self, *args, **kargs)
        DBItem.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBMusicTrack(MusicTrack, DBAudioItem, DBStorageMethod):

    __name__ = 'DBMusicTrack'
    __parent__ = DBAudioItem

    def __init__(self, *args, **kargs):
        MusicTrack.__init__(self, *args, **kargs)
        DBAudioItem.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBGenre(Genre, DBContainer, DBStorageMethod):

    __name__ = 'DBGenre'
    __parent__ = DBContainer

    def __init__(self, *args, **kargs):
        Genre.__init__(self, *args, **kargs)
        DBContainer.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBPerson(Person, DBContainer, DBStorageMethod):

    __name__ = 'DBPerson'
    __parent__ = DBContainer

    def __init__(self, *args, **kargs):
        Person.__init__(self, *args, **kargs)
        DBContainer.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBMusicArtist(MusicArtist, DBPerson, DBStorageMethod):

    __name__ = 'DBMusicArtist'
    __parent__ = DBPerson

    def __init__(self, *args, **kargs):
        MusicArtist.__init__(self, *args, **kargs)
        DBPerson.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBMusicGenre(MusicGenre, DBGenre, DBStorageMethod):

    __name__ = 'DBMusicGenre'
    __parent__ = DBGenre

    def __init__(self, *args, **kargs):
        MusicGenre.__init__(self, *args, **kargs)
        DBGenre.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBMovieGenre(MovieGenre, DBGenre, DBStorageMethod):

    __name__ = 'DBMovieGenre'
    __parent__ = DBGenre

    def __init__(self, *args, **kargs):
        MovieGenre.__init__(self, *args, **kargs)
        DBGenre.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBAlbum(Album, DBContainer, DBStorageMethod):

    __name__ = 'DBAlbum'
    __parent__ = DBContainer

    def __init__(self, *args, **kargs):
        Album.__init__(self, *args, **kargs)
        DBContainer.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBMusicAlbum(MusicAlbum, DBAlbum, DBStorageMethod):

    __name__ = 'DBMusicAlbum'
    __parent__ = DBAlbum

    def __init__(self, *args, **kargs):
        DBAlbum.__init__(self, *args, **kargs)
        MusicAlbum.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBPhotoAlbum(PhotoAlbum, DBAlbum, DBStorageMethod):

    __name__ = 'DBPhotoAlbum'
    __parent__ = DBAlbum

    def __init__(self, *args, **kargs):
        DBAlbum.__init__(self, *args, **kargs)
        PhotoAlbum.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBVideoAlbum(VideoAlbum, DBAlbum, DBStorageMethod):

    __name__ = 'DBVideoAlbum'
    __parent__ = DBAlbum

    def __init__(self, *args, **kargs):
        VideoAlbum.__init__(self, *args, **kargs)
        DBAlbum.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBVideoItem(VideoItem, DBItem, DBStorageMethod):

    __name__ = 'DBVideoItem'
    __parent__ = DBItem

    def __init__(self, *args, **kargs):
        VideoItem.__init__(self, *args, **kargs)
        DBItem.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBMovie(Movie, DBItem, DBStorageMethod):

    __name__ = 'DBMovie'
    __parent__ = DBItem

    def __init__(self, *args, **kargs):
        Movie.__init__(self, *args, **kargs)
        DBItem.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBMusicVideoClip(MusicVideoClip, DBItem, DBStorageMethod):

    __name__ = 'DBMusicVideoClip'
    __parent__ = DBItem

    def __init__(self, *args, **kargs):
        MusicVideoClip.__init__(self, *args, **kargs)
        DBItem.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBImageItem(ImageItem, DBItem, DBStorageMethod):

    __name__ = 'DBImageItem'
    __parent__ = DBItem

    def __init__(self, *args, **kargs):
        ImageItem.__init__(self, *args, **kargs)
        DBItem.__init__(self, *args, **kargs)
        self.type = self.__name__


class DBPhoto(Photo, DBItem, DBStorageMethod):

    __name__ = 'DBPhoto'
    __parent__ = DBItem

    def __init__(self, *args, **kargs):
        Photo.__init__(self, *args, **kargs)
        DBItem.__init__(self, *args, **kargs)
        self.type = self.__name__


table_classes = {'DBObject': DBObject,
                 'DBContainer': DBContainer,
                 'DBItem': DBItem,
                 'DBAudioItem': DBAudioItem,
                 'DBMusicTrack': DBMusicTrack,
                 'DBGenre': DBGenre,
                 'DBPerson': DBPerson,
                 'DBMusicArtist': DBMusicArtist,
                 'DBMusicGenre': DBMusicGenre,
                 'DBMovieGenre': DBMovieGenre,
                 'DBAlbum': DBAlbum,
                 'DBMusicAlbum': DBMusicAlbum,
                 'DBPhotoAlbum': DBPhotoAlbum,
                 'DBVideoAlbum': DBVideoAlbum,
                 'DBVideoItem': DBVideoItem,
                 'DBMovie': DBMovie,
                 'DBMusicVideoClip': DBMusicVideoClip,
                 'DBImageItem': DBImageItem,
                 'DBPhoto': DBPhoto}
