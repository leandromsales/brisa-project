# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team
from symbol import parameters

from configobj import *
import sys
import os

if sys.platform == "win32":
    rootDir = "C:\\windows"
    bar = "\\"
else:
    rootDir = os.path.expanduser('~')+ "/.brisa"
    bar = "/"

class BrisaConf(object):

    def __init__(self):
        filePath = rootDir + bar + "brisa.conf"
        self.config = ConfigObj(filePath)

    def get_parameter(self, section = '', parameter=''):
        if section != "":
            aSection = self.config[section]
            return aSection[parameter]
        else:
            return self.config[parameter]

    def set_parameter(self, section , parameter, par_value):
        self.config[section][parameter] = par_value
        self.config.write()

    def contains(self, section, parameter):
        response = False
        parameters = self.config.dict()
        if parameters.has_key(section):
            for i in parameters[section]:
                if i == parameter:
                    response = True
                    break
        return response

