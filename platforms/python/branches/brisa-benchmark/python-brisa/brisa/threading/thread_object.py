# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Concept inside BRisa that abstracts thread management from the developer.
"""

import threading
import signal

from brisa.threading import thread_manager
from brisa import log


thread_counter = 0


# We're near of removing this Thread inheritance and rename ThreadObject to
# something else, since it will not be a thread. Something like TrackedClass,
# TrackedObject. This new class will be basically a wrapper for automatically
# registering a stop/cleanup function for classes. This will help avoiding
# manually stopping everything and make code simpler.


class ThreadObject(threading.Thread):
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
        threading.Thread.__init__(self, name=name)
        self._running = False
        thread_manager.add_thread_object(self)

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
        if not self._running:
            # Already stopped
            return True

        if self._running:
            if self.prepare_to_stop(*args, **kwargs):
                # Returned True, means success
                self._running = False
                thread_manager.remove_thread_object(self)
                return True
            else:
                return False

    def start(self, *args, **kwargs):
        """ Prepares to start and starts the ThreadObject.
        """
        if not self._running:
            self.prepare_to_start(*args, **kwargs)
            self._running = True
            self._interrupted = False
            threading.Thread.start(self)
            log.debug('Starting ThreadObject %s', self)
            return True
        else:
            log.debug('Tried to start ThreadObject %s when its already '\
                      'started', self)
            return False

    def interrupt(self):
        # TODO: interrupt mechanism with pipes ;) (study also the necessity,
        # I could still not find threads within brisa that needs to be
        # interrupted) - dieb
        log.debug('%s was interrupted' % str(self))

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
