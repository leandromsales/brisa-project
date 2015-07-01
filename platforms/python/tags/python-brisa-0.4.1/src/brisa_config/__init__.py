# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import sys
import os.path
import ConfigParser
import logging
import shutil

class BrisaConf(object):
    if sys.platform == "win32":
        brisa_home = os.path.join(os.path.expanduser('~'), "Brisa")
    else:
        brisa_home = os.path.join(os.path.expanduser('~'), ".brisa")
    config_file = os.path.join(brisa_home, 'brisa.conf')
    update_conf = False
    brisa_version = '0.4'

    def __init__(self):
        self.config = ConfigParser.SafeConfigParser()
        self._check_config_file()
        self.config.readfp(open(self.config_file))
        self._check_db_config()
        self.config.readfp(open(self.config_file))

    def _check_db_config(self):
        if self.update_conf:
            db_path=os.path.join(self.brisa_home, 'brisaobject.db')
            self.set_parameter('persistence',
                               'connection',
                               'sqlite:'+db_path)

    def _check_config_file(self):
        if os.path.exists(self.brisa_home):
            if os.path.isfile(self.config_file):
                self.config.readfp(open(self.config_file))
                not_have_version = self.contains('brisa', 'version') == False
                config_file_version = self.get_parameter('brisa', 'version')
                old_config_file = config_file_version != self.brisa_version
                if not_have_version or old_config_file:
                    os.unlink(self.config_file)
                    shutil.copyfile("brisa.conf", self.config_file)
                    self.update_conf = True
            else:
                shutil.copyfile("brisa.conf", self.config_file)
                self.update_conf = True
        else:
            os.mkdir(self.brisa_home)
            shutil.copyfile("brisa.conf", self.config_file)
            self.update_conf = True

    def get_parameter(self, section = '', parameter = ''):
        return self.config.get(section, parameter)

    def get_parameter_as_list(self, section = '', parameter = '', token = ':'):
        '''Get the parameter as a list'''
        return self.config.get(section, parameter).split(token)

    def set_parameter(self, section, parameter, par_value):
        self.config.set(section, parameter, par_value)
        self.config.write(open(self.config_file, 'w'))

    def contains(self, section, parameter):
        '''
        If the given section exists, and contains the given option,
        return True; otherwise return False.
        '''
        return self.config.has_option(section, parameter)

conf = BrisaConf()

get_parameter = conf.get_parameter
get_parameter_as_list = conf.get_parameter_as_list
set_parameter = conf.set_parameter
contains=conf.contains
brisa_home=conf.brisa_home
