# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Repeated function calls (with or without delay between calls).
"""

from threading import Timer
from brisa import log
from brisa.threading import thread_manager


class LoopingCall(object):
    """ Class that performs repeated function calls in a interval.
    """

    def __init__(self, f, *a, **kw):
        """ Constructor for the LoopingCall class.
        """
        self.function = f
        self.a = a
        self.kw = kw
        self.interval = 0
        self.running = False
        self.callback_handler = None

    def start(self, interval, now=True):
        """ Starts the function calls in the interval specified. If now is
        False, it waits the interval before starting doing calls.

        @param interval: interval between calls
        @param now: whether it will start calling now or not

        @type interval: float
        @type now: boolean
        """
        self.interval = interval

        assert interval != 0, ('(warning) starting LoopingCall with \
                                interval %f' % self.interval)

        if now:
            self.register_timeout()
        else:
            Timer(interval, self.register_timeout).start()

    def register_timeout(self):
        self.running = True
        # Call first time!
        self.call()
        self.callback_handler = thread_manager.\
            add_timeout_callback(self.call, self.interval)

    def unregister(self):
        thread_manager.remove_timeout_callback(self.callback_handler)

    def call(self):
        """ LoopingCall main actions. Instead of calling this directly, call
        start().
        """
        if not self.running:
            log.debug('LoopingCall critical mistake: should not still be'\
                      ' registered on ThreadManager after stopped.')
            return

        self.function(*self.a, **self.kw)

    def cleanup(self):
        self.function = lambda: None
        self.a = []
        self.kw = {}

    def stop(self):
        """ Stops the LoopingCall.
        """
        if self.running:
            self.running = False
            self.cleanup()
            self.unregister()
