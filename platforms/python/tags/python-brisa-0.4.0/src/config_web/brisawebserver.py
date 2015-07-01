# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import brisa_logging
import os
import logging
import platform
from commands import getoutput
from twisted.web import resource, static, server
from twisted.internet import reactor
import brisa_config


class BrisaConfServer(object):

    static_pages = ['brisa_style.css', 'top_frame.html', 'about.html']
    page_list = {'shoutcast': ['usage'],
                 'image': ['dirs'],
                 'flickr': ['usage', 'username', 'private'],
                 'video': ['dirs'],
                 'audio': ['dirs'],
                 'persistence': ['connection']}

    def __init__(self, port_number):
        self.port = port_number
        self.root = resource.Resource()
        self.root.putChild('', static.File('config_web/brisa.html'))
        self.root.putChild('images', static.File('config_web/images'))
        self.load_static_pages()
        self.load_dynamic_pages()

    def load_static_pages(self):
        for i in self.static_pages:
            self.put_static_file(i)

    def load_dynamic_pages(self):
        self.root.putChild('brisa_main.html', MainPage())
        for key in self.page_list:
            my_page = DynamicPage(key, self.page_list[key])
            self.root.putChild(key + '.html', my_page)

    def put_static_file(self, file_name):
        self.root.putChild(file_name, static.File('config_web/' + file_name))

    # This implementation for obtaining the IP address of an interface on
    # Linux (eth0, for example) was taken from Paul Cannon on ASPN Python
    # Cookbook, see
    # http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/439094
    #
    def get_ip_address(self, ifname):
        import socket
        import fcntl
        from struct import pack, unpack

        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        return socket.inet_ntoa(fcntl.ioctl(
                                            s.fileno(),
                                            0x8915,  # SIOCGIFADDR
                                            pack('256s', ifname[:15])
                                            )[20:24])
    def run(self):
        if platform.uname()[4].find('arm') is not -1:
                listenAddr = self.get_ip_address('wlan0')
        elif platform.system() == 'Linux':
            listenAddr = self.get_ip_address('eth0')
        logging.info('Staring Brisa web configuration interface.')
        site = server.Site(self.root)
        reactor.listenTCP(self.port, site, interface=listenAddr)
        reactor.run()

class DynamicPage(resource.Resource):
    page_attr = []
    page_id = ''
    page_name = ''

    def __init__(self, id, attr):
        self.page_attr = attr
        self.page_id = id
        self.page_name = id + '.html'
        self.has_extras = False
        if self.page_id == 'video' or self.page_id == 'audio' or \
                self.page_id == 'image':
            self.has_extras = True

    def render(self, request):
        fd = open('config_web/' + self.page_name)
        homepage = fd.read()
        fd.close()
        result = self.get_config_parameters()
        extra = ''
        if self.has_extras:
            parm = self.parse_request(request)
            dirs = '/'
            parent_dirs = dirs
            if parm != {}:
                dirs = parm['dirs']
                dirs = self.convert_to_char(dirs)
                if dirs.endswith('/') == False:
                    parent_dirs = os.path.split(dirs)[0]
                    dirs += '/'
            extra = self.html_convert(self.list_localdirs(dirs), parent_dirs)
            result.append(dirs)
            result.append(extra)
        homepage = homepage % tuple(result)
        return homepage

    def get_config_parameters(self):
        config_parameters = []
        for i in self.page_attr:
            config_parameters.append(brisa_config.\
                                     get_parameter(self.page_id, i))
        return config_parameters

    def list_localdirs(self, root):
        dirs = os.listdir(root)
        result = []
        for entry in dirs:
            if os.path.isdir(root + entry):
                result.append(root + entry)
        result.sort()
        return result

    def html_convert(self, list, parent_dirs):
        html_code = '<a href="' + self.page_name +'?dirs='+ parent_dirs + '"> \
            <strong>..</strong></a>\n<br>'
        for entry in list:
            html_code += '<a href="' + self.page_name +'?dirs='+ entry + '">' \
                + entry + '</a>\n<br>'
        return html_code

    def parse_request(self, request):
        data = request.uri
        data = data.split('/' + self.page_name)[1]
        my_vars = {}
        if data != '':
            data = data.split('?')[1]
            for i in data.split('&'):
                list_var = i.split('=')
                my_vars[list_var[0]] = list_var[1]
        return my_vars

    def convert_to_char(self, sentence):
        sentence = sentence.replace('%2F', '/')
        sentence = sentence.replace('%3A', ':')
        sentence = sentence.replace('%20', ' ')
        sentence = sentence.replace('%2C', ',')
        return sentence

class MainPage(resource.Resource):

    def __init__(self):
        pass

    def render(self, request):
        result = self.parse_request(request)
        if result != {}:
            self.update_config(result)

        fd = open('config_web/brisa_main.html')
        homepage = fd.read()
        fd.close()
        return homepage % ( \
           brisa_config.get_parameter('brisa', 'version'),
           self.mediaserver_status(),
           brisa_config.get_parameter('persistence', 'connection'),
           brisa_config.get_parameter('audio', 'usage'),
           brisa_config.get_parameter('audio', 'dirs'),
           brisa_config.get_parameter('video', 'usage'),
           brisa_config.get_parameter('video', 'dirs'),
           brisa_config.get_parameter('image', 'usage'),
           brisa_config.get_parameter('image', 'dirs'),
           brisa_config.get_parameter('flickr', 'usage'),
           brisa_config.get_parameter('flickr', 'username'),
           brisa_config.get_parameter('flickr', 'private'),
           brisa_config.get_parameter('shoutcast', 'usage'))

    def mediaserver_status(self):
        logging.debug('Getting Media Server Status')
        pids = self.mediaserver_pids()
        if pids != []:
            status = 'Running on PIDs: ' +str(pids)
        else:
            status = 'Stopped'
        logging.debug('Media Server Status: %s'% (status))
        return status

    def stop_mediaserver(self):
        if self.mediaserver_status() != 'Stopped':
            pids = self.mediaserver_pids()
            for pid in pids:
                os.system('/bin/kill -15 ' + pid)

    def start_mediaserver(self):
        if self.mediaserver_status() == "Stopped":
            os.system('/usr/bin/brisa-server 2>/dev/null 1>/dev/null &')

    def mediaserver_pids(self):
        pid_list=[]
        data = getoutput('ps xau|grep brisaMediaServer.py')
        data = data.splitlines()
        if data != '':
            for line in data:
                if line.find('grep') == -1:
                    list = line.split(' ')
                    count = list.count('')
                    for i in range(count):
                        list.remove('')
                    aux_list = []
                    aux_list.append(list[0])
                    aux_list.append(list[1])
                    aux_list.sort()
                    pid_list.append(aux_list[0])
        return pid_list

    def parse_request(self, request):
        data = request.uri
        data = data.split('/brisa_main.html')[1]
        my_vars = {}
        if data != '':
            data = data.split('?')[1]
            for i in data.split('&'):
                list_var = i.split('=')
                my_vars[list_var[0]] = list_var[1]
        return my_vars

    def update_config(self, result):
        plugin_name = result['plugin_name']
        logging.debug('Updating configuration to
