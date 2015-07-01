# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from unittest import TestCase, TestSuite

from brisa.utils.python import LoopingCall


class LoopingCallTestCase(TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def runTest(self):
        self.assertEquals(1, 1)


def looping_call_test_suite():
    return TestSuite([LoopingCallTestCase()])
