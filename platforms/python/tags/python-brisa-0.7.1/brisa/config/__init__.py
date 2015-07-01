# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

"""
The configuration API provides functions for editing configuration entries.

For the whole configuration API, consider the following illustration of a
configuration entry for understanding the definitions of section, parameter and
value. Note that a much richer example of these entries is brisa.conf itself,
located at BRisa's package root.


[section]

some_parameter=some_value

other_parameter=other_value

[..]


Special variables
=================
- brisa_home: BRisa's home folder for the user running brisa
- __version__: BRisa version
- __interface__: network interface. Can be modified directly during
                 runtime for modifying other modules behavior
- shared_url: may be used as a global variable for sharing url between
              web servers
"""

import sys
import os
import shutil
import ConfigParser

import brisa

__docformat__ = 'epytext en'


_absolute_path_conf_file = os.path.join(os.path.dirname(brisa.__file__),
                                        'brisa.conf')


class BrisaConf(object):
    """ BRisa Configuration API main class.
    """
    brisa_home = os.path.join(os.path.expanduser('~'), ".brisa")
    config_file = os.path.join(brisa_home, 'brisa.conf')
    update_conf = False
    brisa_version = '0.7.1'

    def __init__(self):
        """ Constructor for the BrisaConf class.
        """
        self.config = ConfigParser.SafeConfigParser()
        self._check_config_file()
        self.config.readfp(open(self.config_file))
        self._check_db_config()
        self.config.readfp(open(self.config_file))

    def _check_db_config(self):
        """ If the class is set to update, then this method updates the
        filesystem database path on the file.
        """
        if self.update_conf:
            db_path = os.path.join(self.brisa_home, 'filesystem.db')
            self.set_parameter('persistence', 'connection',
                               'sqlite:%s' % db_path)

    def _check_config_file(self):
        """ If brisa_home does not exist for the user, it will be created and
        brisa.conf default file will be copied. If the config is too old, a
        backup is saved and the new replaces the old.

        TODO: merge old and new config files.
        """
        if os.path.exists(self.brisa_home):
            if os.path.isfile(self.config_file):
                self.config.readfp(open(self.config_file))
                not_have_version = self.contains('brisa', 'version') == False
                config_file_version = self.get_parameter('brisa', 'version')
                old_config_file = config_file_version != self.brisa_version
                if not_have_version or old_config_file:
                    os.rename(self.config_file, "%s.old" % self.config_file)
                    shutil.copyfile("brisa.conf", self.config_file)
                    self.update_conf = True
            else:
                shutil.copyfile(_absolute_path_conf_file, self.config_file)
                self.update_conf = True
        else:
            os.mkdir(self.brisa_home)
            os.chmod(self.brisa_home, 0777)
            shutil.copyfile(_absolute_path_conf_file, self.config_file)
            self.update_conf = True
            os.chmod(self.config_file, 0666)

    def get_parameter(self, section='', parameter=''):
        """ Retrieves the value associated with the parameter in the section
        given.

        @param section: section to find the parameter
        @param parameter: parameter to return the value

        @type section: string
        @type parameter: string

        @return: the value for the given parameter
        @rtype: string
        """
        return self.config.get(section, parameter)

    def get_parameter_bool(self, section='', parameter=''):
        """ Retrieves the bool associated with the parameter in the section
        given.

        @param section: section to find the parameter
        @param parameter: parameter to return the value

        @type section: string
        @type parameter: string

        @return: True or False for the given parameter
        @rtype: boolean
        """
        parameter_value = self.config.get(section, parameter).lower()
        if parameter_value in ['on', 'yes', '1', 1, True]:
            return True
        return False

    def get_parameter_as_list(self, section='', parameter='', token=':'):
        """ Retrieves the list associated with the parameter in the section
        given.

        @param section: section to find the parameter
        @param parameter: parameter where is located the list
        @param token: split token for the list

        @type section: string
        @type parameter: string
        @type token: string

        @return: list associated with the parameter
        @rtype: []
        """
        return self.config.get(section, parameter).split(token)

    def set_parameter(self, section, parameter, par_value):
        """ Sets a parameter's value in the given section.

        @param section: section to set the parameter
        @param parameter: parameter to set the value
        @param par_value: value to be set

        @type section: string
        @type parameter: string
        @type par_value: string
        """
        self.config.set(section, parameter, par_value)
        self.config.write(open(self.config_file, 'w'))

    def contains(self, section, parameter):
        """ Returns wether the given section exists and contains the given
        parameter.

        @param section: section name
        @param parameter: parameter to check if present

        @type section: string
        @type parameter: string
        """
        return self.config.has_option(section, parameter)

    def items(self, section):
        """ Returns all the items of the given section.

        @param section: string

        @type section: string

        @return: all the items of the given section
        @rtype: dictionary
        """
        return self.config.items(section)

conf = BrisaConf()

shared_url = ""
get_parameter_bool = conf.get_parameter_bool
get_parameter = conf.get_parameter
get_parameter_as_list = conf.get_parameter_as_list
set_parameter = conf.set_parameter
contains = conf.contains
brisa_home = conf.brisa_home
__version__ = conf.brisa_version
__interface__ = get_parameter('connection', 'listenif')
