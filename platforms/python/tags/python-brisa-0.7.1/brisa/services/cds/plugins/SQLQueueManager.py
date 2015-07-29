# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from Queue import Queue


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
