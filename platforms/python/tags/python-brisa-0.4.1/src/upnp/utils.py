# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.

# Copyright 2006, Elisa team <contact@fluendo.com>
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import sys

if sys.hexversion >= 0x20500f0:
    from xml.etree import ElementTree
else:
    import elementtree.ElementTree
    from elementtree.ElementTree import Element

import urllib2
import codecs
import cStringIO
import string
from twisted.python import log
import socket

socket.setdefaulttimeout(15)


def url_fetch(url):
    log.msg('Fetching %r' % url)
    req = urllib2.Request(url)
    try:
        handle = urllib2.urlopen(req)
    except IOError, e:
        if hasattr(e, 'reason'):
            log.msg('We failed to reach a server. Reason: %s' % e.reason)
        elif hasattr(e, 'code'):
            log.msg('The server couldn\'t fulfill the request. Error code: %s'\
                    % e.code)
        handle = None
    return handle


def parse_xml(data, encoding="iso-8859-1"):
    p = XMLParser(encoding=encoding)
    p.feed(data.encode(encoding))
    return ElementTree(p.close())


def parse_http_response(data):
    header, payload = data.split('\r\n\r\n')
    lines = header.split('\r\n')
    cmd = string.split(lines[0], ' ')
    lines = map(lambda x: x.replace(': ', ':', 1), lines[1: ])
    lines = filter(lambda x: len(x) > 0, lines)
    headers = [string.split(x, ':', 1) for x in lines]
    headers = dict(map(lambda x: (x[0].lower(), x[1]), headers))

    return cmd, headers
