# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Watch facilities for object flags and SQLLite databases.
"""

from time import sleep
from sqlite3 import dbapi2 as sqlite

from brisa.threading import ThreadObject
from brisa import log


# Events codes
ModifyEventCode, DeleteEventCode, CreateEventCode = range(3)


class Event:
    """ A basic event wrapper.
    """

    def __init__(self, d):
        """ Constructor for the Event class.

        @param d: attributes for describing the event
        @type d: dictionary
        """
        for k, v in d.items():
            setattr(self, k, v)


class FlagWatch(ThreadObject):
    """ Watch for flags. Can watch a flag inside an object and fire a
    callback with any change on that flag.

    We recommend using the flag watch only in flags with locks (for avoiding
    racing conditions).
    """

    def __init__(self, object, flag_name, callback=None, delay=3.0,
                 cargo=None):
        """ Constructor for the FlagWatch class.

        @param object: object that contains the flag
        @param flag_name: name of the flag. object.flag_name must be accessible
        @param callback: callback on change
        @param delay: delay between checks on the flag

        @type object: object
        @type flag_name: string
        @type callback: callable
        @type delay: float
        """
        ThreadObject.__init__(self)
        self._object = object
        self._flag_name = flag_name
        self._callback = callback
        self._cargo = cargo
        self._stored = None
        self.delay = delay

    def run(self):
        """ Main loop for the watch. May not be overwritten.
        """
        while self.is_running:
            value = getattr(self._object, self._flag_name)

            if self._stored != value:
                self._stored = value
                self.on_modify(Event({'value': self._stored,
                                      'mask': ModifyEventCode,
                                      'path': ''.join([str(self._object),
                                                       str(self._flag_name)]),
                                      'name': ''.join([str(self._object),
                                                      str(self._flag_name)])}))

            sleep(self.delay)

    def on_modify(self, event):
        """ Receives modifications.
        """
        if self._callback:
            self._callback(event, self._cargo)


class SQLLiteWatch(ThreadObject):
    """ Watch for SQLLite databases. Can watch fields of an entire table or
    watch with some filters.


    Example of watching with filter:

    >>> def callback(v):
            print 'I received %s' % str(v)

    >>> sqlite_watch = SQLLiteWatch('/path/to/some_db.db',
                                        'table_x',
                                        callback,
                                        {'column_A': None,
                                         'column_B': 5},
                                         0.5)
    >>> sqlite_watch.start()

    In the example above, we design a callback to receive changes and then
    instantiate the SQLLiteWatch class. The parameters are telling we want
    to watch the table named 'table_x' of the database located at
    '/path/to/some_db.db' and we want to watch columns 'column_A' and
    'column_B' of 'table_x'. Also note that the number 5 on the dict tells the
    watch you want only to watch rows where column_B has the value 5. If you
    pass None as the value of the key:value pair, you will have this field
    watched but you won't impose any filtering on that field.

    Changes on the database are passed to the callback designed as a list of
    dicts. Each dict is a database row. Each key from the dict is a field you
    asked for watching and the value is the value from the database.

    Also note that if you do not pass any filter (on the dict values), a single
    change on the database will give the callback a list of ALL rows, including
    the row that changed. Since you didn't request for any filtering, you will
    work with that list of dicts as you wish to get what you want.
    """

    def __init__(self, database_uri, table_name, callback=None, fields={},
                 delay=3.0, cargo=None):
        """ Constructor for the SQLLiteWatch class.

        @param database_uri: database location
        @param table_name: table to watch on
        @param callback: callback on change
        @param fields: fields to watch on. Keys are the columns to watch on and
                       values are the filters.
        @param delay: delay between checks for changes
        @param cargo: callback additional parameters

        @type database_uri: string
        @type table_name: string
        @type callback: callable
        @type fields: dict
        @type delay: float
        """
        ThreadObject.__init__(self)
        self._database_uri = database_uri
        self._callback = callback
        self._table_name = table_name
        self._cargo = cargo
        self._fields = []
        self._filters = []
        self._qmark_args = []
        self._stored = []
        self._query = None
        self._generate_query(table_name, fields)
        self.delay = delay

    def _generate_query(self, table_name, fields):
        """ Generates the SQL query based on the passed information.
        """
        for k, v in fields.items():
            self._fields.append(k)

            if v:
                self._qmark_args.append(v)
                self._filters.append('%s=?' % k)

        fields = ','.join(self._fields)
        filters = ' and '.join(self._filters)

        if self._filters:
            self._query = 'select %s from %s where %s' % (fields,
                                                          table_name,
                                                          filters)
        else:
            self._query = 'select %s from %s' % (fields, table_name)

    def run(self):
        """ Main loop for the watch. May not be overwritten.
        """
        connection = sqlite.connect(self._database_uri)
        cursor = connection.cursor()

        while self.is_running():
            cursor.execute(self._query, tuple(self._qmark_args))
            result = self._parse_result(cursor.fetchall())

            if result != self._stored:
                self._stored = result
                self.on_modify(Event({'value': self._stored,
                                      'mask': ModifyEventCode,
                                      'name': 'SQLLite watch on %s.%s' %\
                                    (self._database_uri, self._table_name),
                                      'path': self._database_uri,
                                      'query': self._query,
                                      'table': self._table_name,
                                      'fields': self._fields}))

            sleep(self.delay)

        log.debug('SQLLIte run exited while loop')
        cursor.close()
        connection.close()

    def on_modify(self, event):
        """ Receives modifications. Forwards to the callback assigned.

        @param value: list
        """
        if self._callback:
            self._callback(event, self._cargo)

    def _parse_result(self, result):
        """ Parses the query result to the form [ {fields1: values1},
        {fields2: values2}, ...].
        """
        if len(result) > 0:
            return [dict(zip(self._fields, r)) for r in result]
        else:
            return []
