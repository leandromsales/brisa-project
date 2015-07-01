# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import sys
import os
import filecmp

from Persistence import *
from brisa.upnp.didl.didl_lite import *
from brisa import log

log = log.getLogger('facade')


def select_audio_list():
    """Select a audio list from DB."""
    try:
        object = AudioItemsList()
        return object.didl_list()
    except Exception, e:
        log.debug(e)
        return []


def select_audio_item(id):
    try:
        item = AudioItems(id)
        return [item.didlobject]
    except:
        return []


def select_url_audio_item_by_id(objectId):
    """Select the url from audio identify by Id."""
    try:
        object = AudioItemsList()
        return object.url_audio(objectId.split('/')[2])
    except Exception, e:
        return []


def select_video_list():
    """Select a audio list from DB."""
    try:
        object = VideoItemsList()
        return object.didl_list()
    except Exception, e:
        log.debug(e)
        return []


def select_video_item(id):
    try:
        item = VideoItems(id)
        return [item.didlobject]
    except:
        return []


def select_url_video_item_by_id(objectId):
    """Select the url from audio identify by Id."""
    try:
        object = VideoItemsList()
        return object.url_video(objectId.split('/')[2])[0]
    except Exception, e:
        log.debug(e)
        return []


def select_picture_list():
    """Select a audio list from DB."""
    try:
        object = PictureItemsList()
        return object.didl_list()
    except Exception, e:
        log.debug(e)
        return []


def select_picture_item(id):
    try:
        item = PictureItems(id)
        return [item.didlobject]
    except:
        return []


def select_url_picture_item_by_id(objectId):
    """Select the url from audio identify by Id."""
    try:
        object = PictureItemsList()
        return object.url_picture(objectId.split('/')[2])[0]
    except Exception, e:
        log.debug(e)
        return []
