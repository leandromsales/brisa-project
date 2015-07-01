# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php

# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006, John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007, Brisa Team

from core import Item

class VideoItem(Item):
    klass = Item.klass + '.videoItem'

class Movie(VideoItem):
    klass = VideoItem.klass + '.movie'

class VideoBroadcast(VideoItem):
    klass = VideoItem.klass + '.videoBroadcast'

class MusicVideoClip(VideoItem):
    klass = VideoItem.klass + '.musicVideoClip'
