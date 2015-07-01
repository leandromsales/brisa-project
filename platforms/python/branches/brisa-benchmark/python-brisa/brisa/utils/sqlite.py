# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from Queue import Queue
from thread import exit_thread
from sqlite3 import dbapi2 as sqlite

from brisa.threading import ThreadObject
from brisa import log


class SQLQueueManager(Queue):

    def __init__(self, maxsize=1):
        Queue.__init__(self, maxsize)
        self.__task_response = None
        self._commit = False

    def put(self, item, commit=True):
        """ Puts an item in work queue.
        """
        self.commit = commit
        Queue.put(self, item)

    def put_and_join(self, item, commit=True):
        self.put(item, commit)
        self.join()

    def _get_result(self):
        """ Fetches the result of work done on the last item added. You may
        only fetch once for each work.
        """
        r = self.__task_response
        self.__task_response = None
        return r

    task_result = property(_get_result)

    def _set_result(self, r):
        """ Do *NOT* call this method directly if you don't know what you're
        doing
        """
        self.__task_response = r

    def task_done(self):
        """ Wrapper for Queue's task_done method
        """
        # Resets commit flag for next item
        self.commit = False
        Queue.task_done(self)

    def _get_commit(self):
        return self._commit

    def _set_commit(self, c):
        self._commit = c

    commit = property(_get_commit, _set_commit)


class SQLWorkerThread(ThreadObject):
    """ Worker thread for database requests, since one sqlite connection object
    can only be used in the same thread it was created.
    """

    def __init__(self, database_uri):
        ThreadObject.__init__(self)
        self.setDaemon(True)
        self.sql_queue_manager = None
        self.database_uri = database_uri

    def set_sql_queue_manager(self, q):
        """ Sets the SQLQueueManager associated with this worker thread. This
        MUST be done before starting this worker thread.
        """
        self.sql_queue_manager = q

    def run(self):
        """ Worker routine for performing database requests.
        """
        if not self.sql_queue_manager:
            raise Exception('SQLQueueManager not set')

        connection = None
        cursor = None

        while self.is_running():
            item = self.sql_queue_manager.get()

            command = item[0]
            args = None

            if len(item) > 1:
                args = item[1]

            if not connection:
                try:
                    connection = sqlite.connect(self.database_uri)
                except Exception, e:
                    log.debug('Could not open database %s', e)
                    break
                cursor = connection.cursor()

            try:
                if not args:
                    cursor.execute(command)
                else:
                    cursor.execute(command, args)
            except Exception, e:
                log.debug('SQLWorkerThread: command/exception: (%s, %s, %s)' %
                            (command, str(args), str(e)))
            self.sql_queue_manager._set_result(cursor.fetchall())
            if self.sql_queue_manager.commit:
                connection.commit()
            self.sql_queue_manager.task_done()

        if cursor:
            cursor.close()

        if connection:
            connection.close()
        exit_thread()
