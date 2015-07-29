# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2009 Brisa Team <brisa-develop@garage.maemo.org>

import time
import unittest

from brisa_media_center import model
from brisa.core import log


class MediaCenterShutdownTestCase(unittest.TestCase):
    """ Test case for shutting down services.
    """

    model = model.MediaCenterModel(False)

    def testDmsHalt(self):
        """ Test DMS halt method.
        """
        self.model.acquire_media_server_proxy()
        self.model.dms_halt()
        time.sleep(10)
        self.assertEquals(self.model.is_server_running(), False)

    def testDmrHalt(self):
        """ Test DMR halt method.
        """
        self.model.acquire_media_renderer_proxy()
        self.model.dmr_halt()
        time.sleep(10)
        self.assertEquals(self.model.is_renderer_running(), False)


class MediaCenterModelTestCase(unittest.TestCase):
    """ Media Center Model test case.

    @note: these tests require brisa media server and brisa media renderer
           installed
    """
    model = model.MediaCenterModel()

    def testBusSetup(self):
        """ Tests bus setup.
        """
        self.failUnless(model.bus != None)

    def testPrefsManagerSetup(self):
        """ Tests prefs manager setup.
        """
        self.failUnless(model.prefs_manager != None)

    def testDmsAcquire(self):
        """ Tests DMS proxy acquire.
        """
        self.failUnless(self.model.dms_proxy != None)

    def testDmrAcquire(self):
        """ Test DMR proxy acquire.
        """
        self.failUnless(self.model.dmr_proxy != None)

    def testDmsMethods(self):
        """ Tests DMS dbus methods.
        """
        # Some of these tests are just for checking errors on the calls, not on
        # the results.
        self.assertTrue(bool(self.model.dms_cds_browse(0)))
        self.assertTrue(bool(self.model.dms_cm_get_protocol_info()))
        self.assertTrue(bool(self.model.dms_cm_get_current_connection_ids()))
        self.assertTrue(bool(self.model.dms_cm_get_current_connection_info()))
        self.model.dms_cds_search(0, 'St')
        self.model.dms_cds_get_search_caps()
        self.model.dms_cds_get_sort_caps()
        self.model.dms_cds_get_sys_update_id()

    def testDmrMethods(self):
        """ Tests DMR dbus methods.
        """
        # Some of these tests are just for checking erros on the calls, not on
        # the results.
        self.model.dmr_avt_set_av_transport_uri(0, '', '')
        self.model.dmr_avt_get_media_info()
        self.model.dmr_avt_get_media_info_ext()
        self.model.dmr_avt_get_transport_info()
        self.model.dmr_avt_get_position_info()
        self.model.dmr_avt_get_device_caps()
        self.model.dmr_avt_get_transport_settings()
        self.model.dmr_avt_play()
        self.model.dmr_avt_stop()
        self.model.dmr_avt_pause()
        self.model.dmr_avt_seek(0, 1, '')
        self.model.dmr_avt_next()
        self.model.dmr_avt_previous()
        self.model.dmr_rc_list_presets()
        self.model.dmr_rc_select_preset('')
        self.model.dmr_rc_get_volume(0, 0)
        self.model.dmr_rc_set_volume(0, 0, 0)
        self.model.dmr_cm_get_protocol_info()
        self.model.dmr_cm_get_current_connection_info()
        self.model.dmr_cm_get_current_connection_ids()


suite = unittest.TestLoader().loadTestsFromTestCase(MediaCenterModelTestCase)
unittest.TextTestRunner(verbosity=2).run(suite)

suite = unittest.TestLoader().loadTestsFromTestCase(MediaCenterShutdownTestCase)
unittest.TextTestRunner(verbosity=2).run(suite)
