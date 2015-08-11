# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import os
from unittest import TestCase, TestSuite, TextTestRunner
from time import sleep
from tempfile import mkstemp
from sqlite3 import dbapi2 as sqlite

from brisa.utils.watch import *


class FlagWatchTestCase(TestCase):
    flag = False
    cargo = 'test cargo'
    last_event = None
    delay_between_commands = 0.05

    def setUp(self):
        self.flag_watch = FlagWatch(self, 'flag', self.onModify,
                                    0.01, self.cargo)
        self.flag_watch.start()

    def tearDown(self):
        self.flag_watch.stop()

    def runTest(self):
        """ Tests on the FlagWatch using direct flag changes
        """
        self.flag = True
        sleep(self.delay_between_commands)
        self.testModifyEvent()

        self.flag = False
        sleep(self.delay_between_commands)
        self.testModifyEvent()

        self.flag = True
        sleep(self.delay_between_commands)
        self.testModifyEvent()

    def onModify(self, event, cargo):
        self.last_event = event
        self.last_cargo = cargo

    def testModifyEvent(self):
        self.assertEquals(self.last_event.mask, ModifyEventCode)
        self.assertEquals(self.last_event.value, self.flag)
        self.assertEquals(self.last_cargo, self.cargo)


class SQLLiteWatchTestCase(TestCase):
    last_event = None
    last_cargo = None
    cargo = 'test cargo'
    tmp_database_path = mkstemp()[1]
    connection = None
    watch_simple = None
    watch_with_filter = None
    delay_between_commands = 0.3

    def _create_test_database(self):
        self.connection = sqlite.connect(self.tmp_database_path)
        self.connection.execute('create table test_table(text VARCHAR(100),'\
                                ' number INTEGER)')
        self.connection.commit()

    def setUp(self):
        self._create_test_database()
        self.watch_simple = SQLLiteWatch(self.tmp_database_path,
                                             'test_table',
                                             self.onModify,
                                             {'text': None,
                                              'number': None},
                                             0.01,
                                             self.cargo)
        self.watch_with_filter = SQLLiteWatch(self.tmp_database_path,
                                                    'test_table',
                                                    self.onModify,
                                                    {'text': None,
                                                     'number': 5},
                                                    0.01,
                                                    self.cargo)
        self.watch_simple.start()

    def tearDown(self):
        self.connection.close()
        self.watch_with_filter.stop()
        os.remove(self.tmp_database_path)

    def runTest(self):
        """ Tests on the SQLLiteWatch for watching with and without filters
        """
        self.testSimpleWatch()
        self.testWatchFilters()

    def testSimpleWatch(self):
        """ Tests SQLLiteWatch without filters.
        """
        self.connection.execute('insert into test_table (text, number) values'\
                                ' (?, ?)', ('test text', 5))
        self.connection.commit()
        sleep(self.delay_between_commands)
        self.testEvent('test text', 5)
        self.resetEvent()

        self.connection.execute('update test_table set text=? where number=5',\
                                ('test text 1', ))
        self.connection.commit()
        sleep(self.delay_between_commands)
        self.testEvent('test text 1', 5)
        self.resetEvent()
        self.watch_simple.stop()

    def testWatchFilters(self):
        """ Tests SQLLiteWatch filters.
        """
        self.watch_with_filter.start()
        self.connection.execute('update test_table set text=? where number=5',
                                ('another text', ))
        self.connection.commit()
        sleep(self.delay_between_commands)
        self.testEvent('another text', 5)
        self.resetEvent()

    def testEvent(self, expected_text, expected_number):
        changes_list = self.last_event.value

        if len(changes_list) > 0:
            self.assertEquals(changes_list[0]['text'], expected_text)
            self.assertEquals(changes_list[0]['number'], expected_number)

        self.assertEquals(self.last_event.mask, ModifyEventCode)
        self.assertEquals(self.last_cargo, self.cargo)

    def resetEvent(self):
        self.last_event = None

    def onModify(self, event, cargo):
        self.last_event = event
        self.last_cargo = cargo


def watch_test_suite():
    tests = [FlagWatchTestCase(), SQLLiteWatchTestCase()]
    suite = TestSuite(tests)
    return suite

if __name__ == "__main__":
    TextTestRunner(verbosity=2).run(watch_test_suite())
