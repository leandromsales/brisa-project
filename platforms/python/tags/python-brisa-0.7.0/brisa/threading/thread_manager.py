# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Manager for threads. Keeps track of all threads, and provides a light
main loop.
"""

import os
import signal

from thread import start_new_thread
from threading import Timer, Condition

from brisa import log
from brisa.utils.python.sleep import sleep


class _ThreadManager:
    """ This class is not intended for direct use. Consider using ThreadManager
    reference for programming and as documentation reference.
    """

    STOP_FUNCTIONS_DELAY = 2
    STOP_THREADOBJECTS_DELAY = 1

    def __init__(self):
        self._threads = []
        self._stop_functions = []
        self.__stopped = True
        self.cond = Condition()
        signal.signal(signal.SIGINT, self._exit)
        signal.signal(signal.SIGTERM, self._exit)

    def add_thread_object(self, thread_object):
        """ Adds a new ThreadObject.

        This is called internally everytime a new ThreadObject is instantiated
        (it registers itself).

        Only call this directly if you know what you are doing.
        """
        self._threads.append(thread_object)

    def remove_thread_object(self, thread_object):
        """ Removes ThreadObject from the threads list if present.
        """
        if thread_object in self._threads:
            self._threads.remove(thread_object)

    def run_async_function(self, f, param_tuple=(), delay=0):
        """ Calls a function passing a parameters tuple. Note that this
        function returns nothing. If you want an asynchronous call with a
        monitor object, see run_async_call and ThreadedCall.
        """
        if delay > 0:
            # If delay is valid, schedule a timer for that call
            t = Timer(delay, f, args=list(param_tuple))
            t.start()
        else:
            # Instant call
            start_new_thread(f, param_tuple)

    def stop_all(self):
        """ Stops all ThreadObjects added and calls all stop functions
        registered.

        This should not be called directly. Use stop_main_loop instead for
        closing all threads and returning to the main loop call origin.
        """
        log.debug('ThreadManager: calling all stop functions')
        for s in self._stop_functions:
            try:
                if not s():
                    # Returned False, means it needs to be called again.
                    # If one wants to modify the delay for the stop functions,
                    # then modify this parameter directly.
                    self.run_async_function(s, delay=self.STOP_FUNCTIONS_DELAY)
            except Exception, e:
                # TODO treat this exception, force quit
                log.debug('ThreadManager: could not call stop function %s \
                          (message: %s)' % (str(s), str(e)))

        log.debug('ThreadManager: stopping all threads')
        for t in self._threads:
            try:
                if not t.stop():
                    self. run_async_function(t.stop,\
                                        delay=self.STOP_THREADOBJECTS_DELAY)
            except Exception, e:
                # TODO treat this exception, force quit
                log.debug('ThreadManager: could not stop thread %s (message:\
                           %s)' % (t.getName(), str(e)))
        log.debug('ThreadManager: all threads stopped')

    def main_loop(self):
        """ Simple main loop that holds the application from quitting
        unexpectedly.

        Any call to this method will block the thread until stop_main_loop
        is called.
        """
        self.__stopped = False
        while not self.__stopped:
            try:
                self.cond.acquire()
                self.cond.wait(1000000)
                self.cond.release()
            except KeyboardInterrupt, k:
                self._exit()
            except Exception, e:
                log.debug("Exception caught by main loop: %s" % str(e))
        return

    def stop_main_loop(self):
        """ Stops all threads, calls all stop functions registered.

        Returns the main thread to the origin of the call of main_loop().
        """
        self._exit()

    def register_stop_function(self, f):
        """ Register a function to be called when the main loop exits.

        This should be primarily used for clean-up and stop functions.
        """
        self._stop_functions.append(f)

    def _exit(self, s=None, t=None):
        """ Real exit implementation.

        Flags the exit, notifies the condition and start the stop_all()
        procedure.
        """
        self.stop_all()
        self.__stopped = True
        self.cond.acquire()
        self.cond.notify()
        self.cond.release()
        log.debug('ThreadManager: exiting...')


_singleton = _ThreadManager()


def ThreadManager():
    """ ThreadManager is the class that manages BRisa's threads and provides
    some useful functions such as running asynchronous function calls, a
    blocking main loop and proper threads stopping.

    If you're using ThreadManager's loop and you want your threads to finish
    nicely, it's strongly recommended to inherit from brisa.threading.\
    ThreadObject (see its documentation).

    By doing this, your thread will automatically register itself on the
    ThreadManager and finish smoothly at the end of execution
    (ThreadManager().main_loop_quit()).

    Using ThreadManager:
        - Using ThreadObject to have threads properly registered to the
    ThreadManager
        - Running a main loop with ThreadManager().main_loop()
        - Stopping all threads with ThreadManager().stop_main_loop()
        - Registering "stop" functions to be called when the main loop quits

    ThreadManager is a *SINGLETON*. This means you can import it at any time
    and use its functions.

    Some other useful management functions are also provided in the brisa.\
    threading package.
    """
    return _singleton
