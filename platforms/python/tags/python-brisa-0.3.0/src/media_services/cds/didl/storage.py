# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php

# Copyright 2005, Tim Potter <tpot@samba.org>
# Copyright 2006, John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007, Brisa Team

from core import Container
import sys
if sys.hexversion >= 0x20500f0:
    from xml.etree import ElementTree
    from xml.etree.ElementTree import Element, SubElement, tostring, _ElementInterface
else:
    from elementtree.ElementTree import Element, SubElement, tostring, _ElementInterface


class StorageSystem(Container):
    klass = Container.klass + '.storageSystem'

    total = -1
    used = -1
    free = -1
    maxpartition = -1
    medium = 'UNKNOWN'

    def toElement(self):

        root = Container.toElement(self)

        SubElement(root, 'upnp:storageTotal').text = str(self.total)
        SubElement(root, 'upnp:storageUsed').text = str(self.used)
        SubElement(root, 'upnp:storageFree').text = str(self.free)
        SubElement(root, 'upnp:storageMaxPartition').text = str(self.maxpartition)
        SubElement(root, 'upnp:storageMedium').text = self.medium

        return root

class StorageVolume(Container):
    klass = Container.klass + '.storageVolume'

    total = -1
    used = -1
    free = -1
    medium = 'UNKNOWN'

    def toElement(self):

        root = Container.toElement(self)

        SubElement(root, 'upnp:storageTotal').text = str(self.total)
        SubElement(root, 'upnp:storageUsed').text = str(self.used)
        SubElement(root, 'upnp:storageFree').text = str(self.free)
        SubElement(root, 'upnp:storageMedium').text = self.medium

        return root

class StorageFolder(Container):
    klass = Container.klass + '.storageFolder'

    used = -1

    def toElement(self):

        root = Container.toElement(self)

        if self.used is not None:
            SubElement(root, 'upnp:storageUsed').text = str(self.used)

        return root
