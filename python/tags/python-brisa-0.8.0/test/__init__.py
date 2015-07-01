# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from unittest import TestSuite, TextTestRunner

from utils import *
from config import *
from mthreading import *


def all_tests():
    """ Returns a suite containing all tests.
    """
    suite = TestSuite([network_test_suite(), looping_call_test_suite(),
                       watch_test_suite(), config_test_suite(),
                       thread_object_test_suite(), threaded_call_test_suite()])
    return suite


# This is the core Brisa's unit tests. Feel free to modify the __main__
# below for your tests with specific modules/test suites.

if __name__ == '__main__':
    # Example to perform unit tests only on network related modules
    # TextTestRunner(verbosity=2).run(network_test_suit())

    TextTestRunner(verbosity=2).run(all_tests())
