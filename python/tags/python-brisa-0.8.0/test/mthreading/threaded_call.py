# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from unittest import TestCase, TestSuite
from time import sleep

from brisa.threading import *
from brisa.threading.thread_object import *


class MyThreadCall(ThreadedCall):

    def __init__(self, function, *args, **kwargs):
        ThreadedCall.__init__(self, function, delay=4, *args, **kwargs)

    def run(self):
        try:
            ThreadedCall.run(self)
        except ThreadMustDie:
            pass


class ThreadedCallTestCase(TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def runTest(self):

        testFunction = lambda: True

        t1 = MyThreadCall(testFunction)
        t1.start()
        t2 = MyThreadCall(testFunction)
        t2.start()
        t3 = MyThreadCall(testFunction)
        t3.start()

        self.assertEquals(t1.result, None)
        self.assertEquals(t2.result, None)
        self.assertEquals(t3.result, None)

        sleep(1)
        t2.cancel()
        t3.stop()
        sleep(5)

        self.assertEquals(t1.result, True)
        self.assertEquals(t2.result, None)
        self.assertEquals(t3.result, None)


def threaded_call_test_suite():
    tests = [ThreadedCallTestCase()]
    suite = TestSuite(tests)
    return suite
