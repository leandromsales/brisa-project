# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from thread import exit_thread
from sqlite3 import dbapi2 as sqlite

from brisa.threading import ThreadObject
from brisa import config, log


class SQLWorkerThread(ThreadObject):
    """ Worker thread for database requests, since one sqlite connection object
    can only be used in the same thread it was created.
    """

    def __init__(self, database_uri):
        ThreadObject.__init__(self)
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
                connection = sqlite.connect(self.database_uri)
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

        cursor.close()
        connection.close()
        exit_thread()
