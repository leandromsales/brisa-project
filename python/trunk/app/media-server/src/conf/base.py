# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from brisa.core import config


class BaseController(object):

    def __init__(self, section):
        self.section = section

    def set_param(self, param, param_value):
        """ Sets a parameter value.
        """
        config.manager.set_parameter(section, param, param_value)

    def get_param(self, param):
        """ Retrieves a parameter value.
        """
        config.manager.get_parameter(section, param)

    def reset(self):
        """ Resets the config by removing the section.
        """
        config.manager.rem_section(section)


class BaseConfig(object):
    """ Performs a first-time configuration.
    """

    def __init__(self, sect_name, d):
        """ Constructor for the BaseConfig class.

        @param sect_name: section name
        @param d: dict of initial configs

        @type sect_name: string
        @type d: dict
        """
        if sect_name in config.manager.get_section_names():
            return
        for k,v in d.items():
            config.manager.set_parameter(sect_name, k, v)
            config.manager.save()
