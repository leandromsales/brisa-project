# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Concept inside BRisa that abstracts thread management from the developer.
"""

from threading import Thread
import threading
import ctypes
import signal

from brisa.threading.thread_manager import ThreadManager
from brisa import log


thread_counter = 0


class ThreadCancelled(Exception):

    message = "Thread killed by force."

_PyThreadState_SetAsyncExc = ctypes.pythonapi.PyThreadState_SetAsyncExc
_pthread_kill = ctypes.CDLL("libpthread.so.0").pthread_kill


def _ignore_handler(signum, frame):
    pass

signal.signal(signal.SIGUSR1, _ignore_handler)


class ThreadObject(Thread):
    """ ThreadObject is a concept inside BRisa which abstracts Thread
    management from the developer.

    Basically, a ThreadObject is a Thread with some useful access/control
    methods and automatic registering/closure.

    When a ThreadObject is instantiated, it registers itself on the
    ThreadManager, who keeps track of all ThreadObjects for clean exiting.

    Since ThreadObject is a Thread, all the logic of your Thread should be
    written in the run() method, which does nothing by default.

    If you need to do some actions before actually starting your ThreadObject
    routine, you should do this on the prepare_to_start() method.

    Analogous to the prepare_to_start() method, ThreadObject's also provides
    a prepare_to_stop() method, in which you should write actions to be done
    before exiting.

    ThreadObjects are named with the template "ThreadObject %d: %s" where
    %d is the unique ThreadObject number and %s is your class that inherits
    from ThreadObject. This keeps debugging easy.

    Note that if you overwrite stop() or start() methods, both prepare_to_start
    and prepare_to_stop methods will become useless, since the logic for these
    calls will be lost (unless you copy it, which is not a very smart thing).

    BRisa's developers really recommend developers to write the run() method in
    a way that is_running() method is often read and handles the exiting of
    your ThreadObject. We still don't have a force-quit scheme implemented and
    by doing this the developer will avoid defunct processes.
    """

    def __init__(self):
        global thread_counter
        name = 'ThreadObject %d: %s' % (thread_counter,
                                        str(self.__class__).replace('<', '').\
                                        replace('>', ''))
        thread_counter += 1
        Thread.__init__(self, name=name)
        self._running = False
        self._interrupted = False

        # HACK: replaces run() by an instrumented version
        self.original_run_method = self.run
        self.run = self.instrumented_run

        ThreadManager().add_thread_object(self)

    # Presence of __del__ causes thread objects not to be collected!
    #
    # def __del__(self):
    #     """ Does nothing by default.
    #     """
    #    # Closing before destruction, avoids unwanted errors
    #     pass

    def is_running(self):
        """ Returns whether the ThreadObject has been stopped or not.
        """
        return self._running

    def __repr__(self):
        """ Formal representation for a ThreadObject.
        """
        return '<%s, started: %s>' % (self.__class__, self._running)

    def stop(self, *args, **kwargs):
        """ Virtually stops this ThreadObject. This means that the run() method
        should be the one that exits when self.is_running() returns False.

        This method only prepares the ThreadObject to stop and sets running to
        False.
        """
        if self._running:
            if self.prepare_to_stop(*args, **kwargs):
                # Returned True, means success
                self._running = False
                ThreadManager().remove_thread_object(self)
                return True
        return False

    def interrupt(self):
        if self._interrupted:
            # Exception injection must not happen twice
            return

        thread_object_id = None
        for tid, tobj in threading._active.items():
            if tobj is self:
                thread_object_id = tid

        if thread_object_id is None:
            return

        self._running = False
        _PyThreadState_SetAsyncExc(thread_object_id,
                                   ctypes.py_object(ThreadCancelled))
        _pthread_kill(thread_object_id, signal.SIGUSR1)
        self._interrupted = True

    def start(self, *args, **kwargs):
        """ Prepares to start and starts the ThreadObject.
        """
        if not self._running:
            self.prepare_to_start(*args, **kwargs)
            self._running = True
            self._interrupted = False

            Thread.start(self)
            log.debug('%s: starting ThreadObject' % self)

            return True
        else:
            log.debug('%s: tried to start ThreadObject when its already \
                        started' % self)
            return False

    def instrumented_run(self):
        """ Calls original run() and catches ThreadCancelled exception """
        try:
            self.original_run_method()
        except ThreadCancelled:
            log.debug("Thread %s has been cancelled" % self.getName())
            pass

    # Methods below should be overwritten when needed

    def prepare_to_stop(self, *args, **kwargs):
        """ This method should be overwritten with actions that should be
        done before stopping.

        You must return True if your prepare process happened as desired. On
        problems, return False to indicate that the stop() call should be
        rescheduled.
        """
        return True

    def prepare_to_start(self, *args, **kwargs):
        """ This method should be overwritten with actions that should be
        done before starting.
        """
        return True

    def run(self):
        """ This method may be overwritten with the routine of this
        ThreadObject.
        """
        raise NotImplementedError('run() method not implemented for a \
                                  ThreadObject')
