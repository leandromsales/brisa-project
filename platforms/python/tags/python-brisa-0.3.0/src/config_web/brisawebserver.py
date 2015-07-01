# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import os
from twisted.web import resource, static, server
from twisted.internet import reactor
from config_handle.config import BrisaConf


class BrisaConfServer(object):

    static_pages = ['brisa_style.css', 'top_frame.html', 'about.html']
    page_list = {'shoutcast':['usage'], 'photoalbum':['albums'], 'flickr':['usage', 'username', 'private'],
                         'video':['dir'], 'sound':['dir'], 'persistence':['connection']}

    def __init__(self, port_number):
        self.port = port_number
        self.root = resource.Resource()
        self.root.putChild('', static.File('config_web/brisa.html'))
        self.root.putChild('images', static.File('config_web/images'))
        self.load_static_pages()
        self.load_dynamic_pages()
        self.config_handler = BrisaConf()

    def load_static_pages(self):
        for i in self.static_pages:
            self.put_static_file(i)

    def load_dynamic_pages(self):
        self.root.putChild('brisa_main.html', MainPage())
        for i in self.page_list:
            my_page = DynamicPage(i, self.page_list[i])
            self.root.putChild(i + '.html', my_page)

    def put_static_file(self, file_name):
        self.root.putChild(file_name, static.File('config_web/' + file_name))

    def run(self):
        site = server.Site(self.root)
        reactor.listenTCP(self.port, site, interface='localhost')
        reactor.run()

class DynamicPage(resource.Resource):
    page_attr = []
    page_id = ''
    page_name = ''

    def __init__(self, id , attr):
        self.page_attr = attr
        self.page_id = id
        self.page_name = id + '.html'
        self.has_extras = False
        if self.page_id == 'video' or self.page_id == 'sound' or self.page_id == 'photoalbum':
            self.has_extras = True

    def render(self, request):
        fd = open('config_web/' + self.page_name)
        homepage = fd.read()
        fd.close()
        result = self.get_config_parameters()
        extra = ''
        if self.has_extras:
            parm = self.parse_request(request)
            dir = '/'
            parent_dir = dir

            if parm != {}:
                dir = parm['dir']
                dir = self.convert_to_char(dir)
                if dir.endswith('/') == False:
                    parent_dir = os.path.split(dir)[0]
                    dir += '/'
            extra = self.html_convert(self.list_localdir(dir), parent_dir)
            result.append(dir)
            result.append(extra)
        homepage = homepage % tuple(result)
        return homepage

    def get_config_parameters(self):
        config_handler = BrisaConf()
        config_parameters = []
        for i in self.page_attr:
            config_parameters.append(config_handler.get_parameter(self.page_id, i))
        return config_parameters

    def list_localdir(self, root):
        dir = os.listdir(root)
        result = []
        for entry in dir:
            if os.path.isdir(root + entry):
                result.append(root + entry)
        result.sort()
        return result

    def html_convert(self, list, parent_dir):
        html_code = '<a href="' + self.page_name +'?dir='+ parent_dir + '"><strong>..</strong></a>\n<br>'
        for entry in list:
            html_code += '<a href="' + self.page_name +'?dir='+ entry + '">' + entry + '</a>\n<br>'
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

    def convert_to_char(self,sentence):
        sentence = sentence.replace('%2F','/')
        sentence = sentence.replace('%3A',':')
        sentence = sentence.replace('%20',' ')
        sentence = sentence.replace('%2C',',')
        return sentence

class MainPage(resource.Resource):

    def __init__(self):
        self.config_handler = BrisaConf()

    def render(self, request):
        result = self.parse_request(request)
        if result != {}:
            self.update_config(result)

        fd = open('config_web/brisa_main.html')

        homepage = fd.read()
        fd.close()
        return homepage % (self.config_handler.get_parameter('brisa', 'version'),
                           self.mediaserver_status(),
           self.config_handler.get_parameter('persistence', 'connection'),
           self.config_handler.get_parameter('sound', 'dir'),
           self.config_handler.get_parameter('video', 'dir'),
           self.config_handler.get_parameter('shoutcast', 'usage'),
           self.config_handler.get_parameter('flickr', 'usage'),
           self.config_handler.get_parameter('flickr', 'username'),
           self.config_handler.get_parameter('flickr', 'private'),
           self.config_handler.get_parameter('photoalbum', 'albums')
        )

    def mediaserver_status(self):
        pids = self.mediaserver_pids()
        if pids != []:
            status = 'Running on PIDs: ' +str(pids)
        else:
            status = 'Stopped'
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
        os.system('ps aux|grep "python2.5 brisaMediaServer.py" > /tmp/brisa.pid')
        fd = open('/tmp/brisa.pid')
        data = fd.read()
        fd.close()
        data = data.splitlines()
        os.system('/bin/rm /tmp/brisa.pid')
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
        if plugin_name == 'flickr':
            self.config_handler.set_parameter('flickr',
                                             'usage',
                                             self.convert_to_char(result['usage']))
            self.config_handler.set_parameter('flickr',
                                             'username',
                                             self.convert_to_char(result['username']))
            self.config_handler.set_parameter('flickr',
                                             'private',
                                             self.convert_to_char(result['private']))
        elif plugin_name == 'shoutcast':
            self.config_handler.set_parameter('shoutcast',
                                             'usage',
                                             self.convert_to_char(result['usage']))
        elif plugin_name == 'sound':
            self.config_handler.set_parameter('sound',
                                             'dir',
                                             self.convert_to_char(result['dir']))
        elif plugin_name == 'video':
            self.config_handler.set_parameter('video',
                                             'dir',

