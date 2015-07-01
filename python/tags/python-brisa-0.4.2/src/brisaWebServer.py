# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a Web Server to Configure Brisa
# It is based on the Twisted Framework

from config_web.brisawebserver import BrisaConfServer

if __name__=='__main__':
    brisa_server = BrisaConfServer(7777)
    brisa_server.run()
