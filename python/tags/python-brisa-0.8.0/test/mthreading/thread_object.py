# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from unittest import TestCase, TestSuite
from time import sleep
import threading

from brisa.threading import *


class MyThread(ThreadObject):

    def __init__(self):
        ThreadObject.__init__(self)
        self.exception_was_raised = False

    def run(self):
        try:
            sleep(20)
        except:
            self.exception_was_raised = True


class ThreadObjectTestCase(TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def runTest(self):
        t1 = MyThread()
        t1.start()

        t2 = MyThread()
        t2.start()

        sleep(2)

        self.assertEquals(t1.is_running(), True)
        self.assertEquals(t2.is_running(), True)
        self.assertEquals(self.thread_is_running(t1), True)
        self.assertEquals(self.thread_is_running(t2), True)


        t2.interrupt()
        sleep(2)

        self.assertEquals(t1.is_running(), True)
        self.assertEquals(t2.is_running(), False)
        self.assertEquals(self.thread_is_running(t1), True)
        self.assertEquals(self.thread_is_running(t2), False)
        self.assertEquals(t1.exception_was_raised, False)
        self.assertEquals(t2.exception_was_raised, True)


        t1.interrupt()
        sleep(2)

        self.assertEquals(t1.is_running(), False)
        self.assertEquals(t2.is_running(), False)
        self.assertEquals(self.thread_is_running(t1), False)
        self.assertEquals(self.thread_is_running(t2), False)
        self.assertEquals(t1.exception_was_raised, True)
        self.assertEquals(t2.exception_was_raised, True)

    def thread_is_running(self, t):
        for tid, tobj in threading._active.items():
            if tobj is t:
                return True
        return False


def thread_object_test_suite():
    tests = [ThreadObjectTestCase()]
    suite = TestSuite(tests)
    return suite
