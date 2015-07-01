# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Manager for threads. Keeps track of all threads, and provides a light
main loop with features such as file handling and repeated timed callbacks.

Basically, thread_manager is the class that manages BRisa's threads, handles
file descriptors, timed callbacks and also provides useful functions such
as running asynchronous function calls, a blocking main loop and proper
threads stopping.

If you're using thread_manger main loop and you want your threads to finish
nicely, it's strongly recommended to inherit from brisa.threading.\
ThreadObject (see its documentation).

By doing this, your thread will automatically register itself on the
thread_manager and finish smoothly at the end of execution
(thread_manager.main_loop_quit()).

thread_manager is a *SINGLETON*. This means you can import it at any time
and use its functions. It will be created when first used, and destroyed when
collected by gc.

See also brisa.threading for more useful functions and facilities.
"""

import time
import select
import signal
import random
import thread
import threading

from brisa import log
from errno import EINTR, EBADF


# Define file handling constants
FD_EVENT_TYPE_READ, FD_EVENT_TYPE_WRITE, FD_EVENT_TYPE_ERROR = (1, 2, 4)


class TimeoutCallback(object):
    """ Information wrapper for timed callbacks.
    """

    def __init__(self, callback, timeout_rel, timeout_abs, threshold):
        """ Constructor for the TimeoutCallback class

        @param callback: function to be called
        @param timeout_rel: seconds from now to sleep before the call
        @param timeout_abs: seconds since epoch when the call is scheduled
        @param threshold: lower acceptable bound for timeout_abs precision
        """
        self.callback = callback
        self.timeout_rel = timeout_rel
        self.timeout_abs = timeout_abs
        self.threshold = threshold

    def __call__(self):
        """ Performs the callback.
        """
        self.callback()

    def update_abs_timeout(self):
        """ Updates absolute timeout based on the time now and the relative
        timeout specified.
        """
        self.timeout_abs = self.timeout_rel + time.time()

    def __str__(self):
        """ String representation of the class.
        """
        return '<TimeoutCallback callback=%s, timeout_rel=%s, timeout_abs=%s' \
               ', threshold=%s>' % (str(self.callback), str(self.timeout_rel),
               str(self.timeout_abs), str(self.threshold))


class _ThreadManager(object):
    """ This class is not intended for direct use. Consider using ThreadManager
    reference for programming and as documentation reference.
    """

    def __init__(self):
        self._threads = []
        self._stop_functions = []
        self.__stopped = True
        self._read_fd = {}
        self._write_fd = {}
        self._error_fd = {}
        self._timeout_callbacks = {}
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

    def add_fd(self, fd, event_cb, type=FD_EVENT_TYPE_READ):
        """ Adds a file descriptor to be handled by the main loop. Events are
        delivered to the event callback, which must receive a parameter
        which will specify the type of event.

        @param fd: file descriptor to be handled
        @param event_cb: event handler (callback)
        @param type: an OR (|) combination of FD_EVENT_TYPE_* flags

        @type fd: file
        @type event_cb: callable
        @type type: integer
        """
        if type & FD_EVENT_TYPE_READ:
            log.debug('add_fd(): recognized type READ on %s' % str(fd))
            self._read_fd[fd] = event_cb
        if type & FD_EVENT_TYPE_WRITE:
            log.debug('add_fd(): recognized type WRITE on %s' % str(fd))
            self._write_fd[fd] = event_cb
        if type & FD_EVENT_TYPE_ERROR:
            log.debug('add_fd(): recognized type ERROR on %s' % str(fd))
            self._write_fd[fd] = event_cb

    def remove_fd(self, fd):
        """ Removes a file descriptor from being handled by the main loop.

        TODO: add the ability to remove a file descriptor from one or more
        event types, not all.

        @param fd: file descriptor to be removed from handling

        @type fd: file
        """
        for d in (self._read_fd, self._write_fd, self._error_fd):
            d.pop(fd, None)

    def add_timeout_callback(self, callback, timeout, threshold=0.01):
        """ Creates a timed callback and returns an unique id (which can be
        used for later removal of the callback). This callback will be called
        every 'timeout' seconds until the callback is removed with
        remove_timeout_callback() or the main loop stops.

        @param callback: function to be called
        @param timeout_rel: seconds from now to sleep before the call
        @param timeout_abs: seconds since epoch when the call is scheduled
        @param threshold: lower acceptable bound for timeout_abs precision

        @type callback: callable
        @type timeout_rel: integer
        @type timeout_abs: integer
        @type threshold: float

        @return: unique ID for the callback
        @rtype: integer
        """
        log.debug('add_timeout_callback(%s, %d, %f)' % (str(callback), timeout,
                                                        threshold))
        id = random.randint(-50000, 50000)

        while id in self._timeout_callbacks:
            id = random.randint(-50000, 50000)

        self._timeout_callbacks[id] = TimeoutCallback(callback,
                                                      timeout,
                                                      timeout + time.time(),
                                                      threshold)
        return id

    def remove_timeout_callback(self, id):
        """ Removes a timed callback given it's id.

        @param id: callback id from add_timeout_callback()
        @type id: integer
        """
        try:
            self._timeout_callbacks.pop(id)
        except KeyError:
            raise KeyError('No such timeout callback registered with id %d' %
                           id)

    def _process_timeout_table(self):
        """ Processes the timeout callbacks table.
        """
        for callback in self._timeout_callbacks.values():
            if callback.timeout_abs - callback.threshold < time.time():
                log.debug('Callback ready: %s' % str(callback))
                if self.is_running():
                    # Call it
                    callback()
                # Update the absolute timeout anyways ;)
                callback.update_abs_timeout()

    def run_async_function(self, f, param_tuple=(), delay=0):
        """ Calls a function passing a parameters tuple. Note that this
        function returns nothing. If you want an asynchronous call with a
        monitor object, see brisa.threading.run_async_call and
        brisa.threading.ThreadedCall.

        @param f: function to be called
        @param param_tuple: tuple param for the function
        @param delay: wait time before calling the function
        """
        if delay > 0:
            # If delay is valid, schedule a timer for that call
            t = threading.Timer(delay, f, args=list(param_tuple))
            t.start()
        else:
            # Instant call
            thread.start_new_thread(f, param_tuple)

    def _stop_all(self):
        """ Stops all ThreadObjects added and calls all stop functions
        registered.

        This SHOULD NOT be called directly. Use main_loop_stop() instead for
        closing all threads and returning to the main loop call origin.
        """
        log.debug('Calling stop functions')
        for s in self._stop_functions[:]:
            try:
                log.debug('Stop function %s' % str(s))
                s()
                self._stop_functions.remove(s)
            except Exception, e:
                # TODO treat this exception, force quit
                log.debug('Could not call stop function %s '\
                          '(message: %s)' % (str(s), str(e)))

        log.debug('Stopping all threads')

        # We have to copy the list dynamically because some threads can be
        # added to self._threads and not to the copy
        copy = list(self._threads)
        while copy:
            for t in copy:
                if not t.is_running():
                    self.remove_thread_object(t)
                    continue
                try:
                    log.debug('Stopping thread %s' % str(t))
                    if not t.stop():
                        log.debug('Could not stop %s' % str(t))
                        t.interrupt()
                        self.remove_thread_object(t)
                except Exception, e:
                    log.debug('Could not stop thread %s ('\
                              'message: %s)' % (t.getName(), str(e)))
            copy = list(self._threads)
        log.debug('All threads stopped')
        log.debug('Dump threads dict: %s' % str(threading._active))

    def interrupt_all(self):
        """ Interrupts all threads.
        """
        for thr in self._threads[:]:
            thr.interrupt()
            thr.stop()
        self._stop_all()
        # Release all references to threads if they have already been stopped
        # and killed
        self._threads = []

    def _get_min_timeout(self):
        """ Returns the minimum timeout among registered TimeoutCallbacks.
        """
        min = 0
        for callback in self._timeout_callbacks.values():
            if min == 0 or callback.timeout_rel < min:
                min = callback.timeout_rel
        return min

    def stop_main_loop(self):
        log.critical('thread_manager.stop_main_loop is DEPRECATED. Use'\
                     'stop_main_loop() instead')
        self.main_loop_stop()

    def main_loop(self):
        """ Main loop.
        """
        log.debug('Main loop started')
        self.__stopped = False
        while not self.__stopped:
            self.main_loop_iterate()

    def main_loop_iterate(self):
        """ Iterates the main loop once if and only if __stopped is False.
        Hacking: try using ThreadManager._ThreadManager__stopped = False.

        @return: True if the iteration completed successfully. If False,
        it's recommended another iteration.
        """
        if not self.__stopped:
            try:
                revt, wevt, eevt = select.select(self._read_fd.keys(),
                                                 self._write_fd.keys(),
                                                 self._error_fd.keys(),
                                                 self._get_min_timeout())

                # Callbacks running on the main thread
                for read in revt:
                    if read not in self._read_fd:
                        continue
                    try:
                        log.debug('Read event on %s, calling %s', read,
                                  self._read_fd[read])
                        self._read_fd[read](FD_EVENT_TYPE_READ)
                    except Exception, e:
                        log.debug('Exception %s raised when handling a READ'\
                                  ' event on file %s', e, read)
                for write in wevt:
                    if write not in self._write_fd:
                        continue
                    try:
                        log.debug('Write event on %s, calling %s', write,
                                  self._write_fd[write])
                        self._write_fd[write](FD_EVENT_TYPE_WRITE)
                    except Exception, e:
                        log.debug('Exception %s raised when handling a WRITE'\
                                  ' event on file %s', e, write)
                for error in eevt:
                    if error not in self._error_fd:
                        continue
                    try:
                        log.debug('Error event on %s, calling %s', error,
                                  self._error_fd[error])
                        self._error_fd[error](FD_EVENT_TYPE_ERROR)
                    except Exception, e:
                        log.debug('Exception %s raised when handling a ERROR'\
                                  ' event on file %s', e, error)

            # Fix problems with problematic file descriptors
            except ValueError, v:
                log.debug('Main loop ValueError: %s' % str(v))
                self._clear_fd_lists()
            except TypeError, t:
                log.debug('Main loop TypeError %s' % str(t))
                self._clear_fd_lists()
            except (select.error, IOError), s:
                if s.args[0] in (0, 2):
                    if not ((not self._read_fd) and (not self._write_fd)):
                        raise
                elif s.args[0] == EINTR:
                    pass
                elif s.args[0] == EBADF:
                    self._clear_fd_lists()
            else:
                # Always executed if the try clause does not raises an
                # exception. Normal situations will reach this code.
                self._process_timeout_table()
                return True
            # Return False recommending another iteration: after removing the
            # problematic files, we need to select them ASAP. Don't forget to
            # process the timeout callbacks table ;)
            self._process_timeout_table()
            return False
        else:
            raise Exception('Could not iterate while stopped.')

    def _clear_fd_lists(self):
        """ Finds and removes problematic file descriptors.

        TODO: maybe use the event callback for signaling that the file
        descriptor was removed
        """
        log.debug('Problematic fd found. Cleaning up...')

        for d in [self._read_fd, self._write_fd, self._error_fd]:
            for s in d.keys():
                try:
                    select.select([s], [s], [s], 0)
                except Exception, e:
                    log.debug('Removing problematic fd: %s' % str(s))
                    d.pop(s)

    def _exit(self, s=None, t=None):
        """ Real exit implementation.

        Flags the exit, starts the _stop_all() procedure.
        """
        if self.__stopped == True:
            log.debug('Exit happened twice, dumping...')
            log.debug('Threads: %s' % str(threading._active))
            log.debug('Stop functions: %s' % str(self._stop_functions))
            return
        self.__stopped = True
        self._stop_all()
        log.debug('Exiting...')

    def main_loop_async(self):
        """ Runs the main loop in separate thread. Useful for applications
        which demand another main loop, such as Gtk. For example, if you need
        to run gtk.main():

        >>> gtk.gdk.threads_init()
        >>> thread_manager.main_loop_async()
        >>> thread_manager.register_stop_function(gtk.main_quit)
        >>> gtk.main()
        """
        self.run_async_function(self.main_loop)

    def main_loop_stop(self):
        """ Stops the main loop.
        """
        self._exit()

    def main_loop_quit(self):
        """ Same as main_loop_stop(). Stops the main loop.
        """
        self.main_loop_stop()

    def is_running(self):
        """ Returns whether the main loop is running.
        """
        return not self.__stopped

    def register_stop_function(self, f):
        """ Register a function to be called when the main loop exits.

        This should be used for clean-up and stop functions.

        @type f: callable
        """
        if f not in self._stop_functions:
            self._stop_functions.append(f)


thread_manager = _ThreadManager()
