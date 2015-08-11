# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php

# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006, John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007, Brisa Team

from core import Item, Album

class ImageItem(Item):
    klass = Item.klass + '.imageItem'

class Photo(ImageItem):
    klass = ImageItem.klass + '.photo'

class PhotoAlbum(Album):
    klass = Album.klass + '.photoAlbum'
