# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>


# Implementation of a simple daemon Application of UPnP Media Server
# It is based on the Twisted Framework

#from config_handle import rootDir
from media_devices.media_renderer import brisarenderer

if __name__ == '__main__':
    #rootDir = sys.path[0]
    brisarenderer.main()
