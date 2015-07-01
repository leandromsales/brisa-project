#!/usr/bin/env python
#
# Copyright (c) 2007 Leandro Melo de Sales
# Copyright (c) 2006-2007 Ricardo Garcia Gonzalez
#
# This code was adapted from youtube-dl project.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
# OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
# ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.
#
# Except as contained in this notice, the name(s) of the above copyright
# holders shall not be used in advertising or otherwise to promote the
# sale, use or other dealings in this Software without prior written
# authorization.
#

import httplib
import os
import re
import urllib2
import socket

# Install cookie and proxy handlers
urllib2.install_opener(urllib2.build_opener(urllib2.ProxyHandler()))
urllib2.install_opener(urllib2.build_opener(urllib2.HTTPCookieProcessor()))

# Global constants
const_video_url_str = 'http://www.youtube.com/watch?v=%s'
const_video_url_re = re.compile(r'^((?:http://)?(?:\w+\.)?youtube\.com/(?:(?:v\
/)|(?:(?:watch(?:\.php)?)?\?(?:.+&)?v=)))?([0-9A-Za-z_-]+)(?(1).+)?$')
const_url_t_param_re = re.compile(r', "t": "([^"]+)"')
const_video_url_real_str = 'http://www.youtube.com/get_video?video_id=%s&t=%s'


def _request_create(url, data=None):
    # Wrapper to create custom requests with typical headers
    retval = urllib2.Request(url)
    if data is not None:
        retval.add_data(data)
    # Try to mimic Firefox, at least a little bit
    retval.add_header('User-Agent', 'Mozilla/5.0 (Windows; U; Windows NT 5.1; \
                                en-US; rv:1.8.1) Gecko/20061010 Firefox/2.0')
    retval.add_header('Accept-Charset', 'ISO-8859-1,utf-8;q=0.7,*;q=0.7')
    retval.add_header('Accept', 'text/xml, application/xml, \
        "application/xhtml+xml,text/html;" \
        "q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5')
    retval.add_header('Accept-Language', 'en-us,en;q=0.5')
    return retval


def _perform_request(url, data=None):
    # Perform a request, process headers and return response
    request = _request_create(url, data)
    response = urllib2.urlopen(request)
    return response


def download_step(url, post_data=None):
    # Generic download step
    try:
        return _perform_request(url, post_data).read()
    except (urllib2.URLError, ValueError, httplib.HTTPException, TypeError,
            socket.error):
        raise Exception('Error: Cannot download de video webpage.')


def extract_step(regexp, data):
    # Generic extract step
    match = regexp.search(data)
    if match is None:
        raise Exception('Error: Match url pattern')
    extracted_data = match.group(1)
    return extracted_data


def get_real_video_url(video_url_cmdl):
    # Retrieve video data

    # Verify video URL format and convert to "standard" format
    video_url_mo = const_video_url_re.match(video_url_cmdl)
    if video_url_mo is None:
        raise Exception('Error: URL does not seem to be a youtube video URL.')
    video_url_id = video_url_mo.group(2)
    video_url = const_video_url_str % video_url_id

    # Retrieve video webpage
    video_webpage = download_step(video_url)

    # Extract needed video URL parameters
    video_url_t_param = extract_step(const_url_t_param_re, video_webpage)
    video_url_real = const_video_url_real_str % (video_url_id,
                                                 video_url_t_param)
    return video_url_real
