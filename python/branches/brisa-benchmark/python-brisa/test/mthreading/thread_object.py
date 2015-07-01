# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from unittest import TestCase, TestSuite


class ThreadObjectTestCase(TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def runTest(self):
        pass


def thread_object_test_suite():
    tests = [ThreadObjectTestCase()]
    suite = TestSuite(tests)
    return suite
