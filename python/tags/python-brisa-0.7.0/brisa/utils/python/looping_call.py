# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Repeated function calls (with or without delay between calls).
"""

from threading import Timer

from brisa import log
from brisa.utils.python.sleep import sleep
from brisa.threading import ThreadObject


class LoopingCall(ThreadObject):
    """ Class that performs repeated function calls in a interval.
    """

    def __init__(self, f, *a, **kw):
        """ Constructor for the LoopingCall class.
        """
        ThreadObject.__init__(self)
        self.function = f
        self.a = a
        self.kw = kw
        self.interval = 0
        self.running = False

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
            ThreadObject.start(self)
        else:
            Timer(interval, Thread.start, self)

    def run(self):
        """ LoopingCall main actions. Instead of calling this directly, call
        start().
        """
        log.debug("LoopingCall started, interval %f" % self.interval)
        self.running = True

        while self.running:
            self.function(*self.a, **self.kw)
            sleep(self.interval)

    def stop(self):
        """ Stops the LoopingCall.
        """
        self.running = False
