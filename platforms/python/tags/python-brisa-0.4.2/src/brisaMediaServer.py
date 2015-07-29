# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a simple daemon Application of UPnP Media Server
# It is based on the Twisted Framework

from media_devices.media_server import brisaserver
#from brisa_utils.process import daemon

if __name__ == '__main__':
    #daemon.daemonize(brisaserver.main)
    brisaserver.main()