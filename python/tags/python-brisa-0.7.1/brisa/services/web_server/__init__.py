# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Provides a Web server implementation with classes representing static files
and resources.

A static file is an object that matches with a file on the machine. A request
to an URL that matches with a static file will have the file as response (for
example, a simple download).

A resource is an object that needs special information to be used for
generating the response. For example, if you have a form with login and
password fields, the submit URL must be Resource that understands the form
data passed.

WebServer is a singleton and an example of how to retrieve it can be found
below.

Basic Usage
===========

>>> from brisa.services.web_server import WebServer
>>> server = WebServer()
>>> server.listen_url = 'http://ip:port'
"""

from brisa.services.web_server.web_server_impl import WebServer
from brisa.services.web_server.resource import CustomResource
from brisa.services.web_server.static import StaticFile
