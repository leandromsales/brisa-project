# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Runs a call asynchronously and forwards the result/error to specified
callbacks.
"""

from threading import Semaphore

from brisa import log
from brisa.utils.python.sleep import sleep
from brisa.threading.thread_object import ThreadObject


class ThreadedCall(ThreadObject):
    """ This class runs a call asynchronously and forwards the result/error
    to specified callbacks.

    One can instantiate this class directly or use the method located at
    package brisa.threading.

    @param function: function to be called passing *args and **kwargs
    @param success_callback: called in case of success, receives call result
    @param error_callback: called in case of error, receives call result
    @param delay: time to be wait before performing the call
    @param args: arguments to the function
    @param kwargs: arguments to the function

    @type function: callable
    @type success_callback: callable
    @type error_callback: callable
    @type delay: float
    """

    def __init__(self, function, success_callback=None, error_callback=None,
                 success_callback_cargo=None, error_callback_cargo=None,
                 delay=None, *args, **kwargs):
        ThreadObject.__init__(self)
        self.function = function
        self.args = args
        self.kwargs = kwargs
        self.success_callback = success_callback
        self.success_callback_cargo = success_callback_cargo
        self.error_callback = error_callback
        self.error_callback_cargo = error_callback_cargo
        self.delay = delay
        self.result = None
        self.completed_flag = False
        self.completed = Semaphore()

    def run(self):
        """ Implementation of the call procedure.
        """
        log.debug('ThreadedCall: running')
        if self.delay:
            # This runs in a thread. We can sleep here instead using time
            sleep(self.delay)
            log.debug('ThreadedCall: sleeping %d' % self.delay)

        try:
            log.debug('ThreadedCall: calling')
            # Performing the call
            self.result = self.function(*self.args, **self.kwargs)
            log.debug('ThreadedCall: got result %s' % self.result)
            if self.success_callback:
                log.debug('ThreadedCall: forwarding to success_callback')
                self.success_callback(self.result, self.success_callback_cargo)

            self.set_completed()

        except Exception, e:
            log.debug('ThreadedCall: exception happened (%s), forwarding...'
                      % e.message)
            # Storing exception for handling
            self.result = e

            if self.error_callback:
                self.error_callback(self.error_callback_cargo, e)

            self.set_completed()

    def cancel(self):
        """ Cancel the call.

        Not implemented yet.
        """
        # TODO: implement me
        pass

    def prepare_to_stop(self):
        """ Prepares to stop.
        """
        if not self.is_completed():
            self.cancel()
        self.set_completed()
        return True

    def is_completed(self):
        """ Returns whether the call has been completed or not.
        """
        self.completed.acquire()
        r = self.completed_flag
        self.completed.release()
        return r

    def set_completed(self):
        """ Sets the call as completed. This should not be called directly.
        Use stop() instead.
        """
        self.completed.acquire()
        self.completed_flag = True
        self.completed.release()
