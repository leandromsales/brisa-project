# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from unittest import TestCase, TestSuite

from brisa.utils.network import *


class NetworkProtocolsTestCase(TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def runTest(self):
        self.assertEquals(1, 0+1)


class NetworkFunctionsTestCase(TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def runTest(self):
        self.assertEquals(1, 0+1)


def network_test_suite():
    tests = [NetworkFunctionsTestCase(), NetworkProtocolsTestCase()]
    suite = TestSuite(tests)
    return suite
