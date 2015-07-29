# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2008 Brisa Team <brisa-develop@garage.maemo.org>

import os

from brisa import config


plugin_section = 'media_server_plugin-maemo-multimedia'
user_home = os.path.expanduser('~')


def setup_basic_configuration():
    config.manager.set_parameter(plugin_section, 'owner', plugin_section)
    config.manager.set_parameter(plugin_section, 'enable', 'True')
    config.manager.set_parameter(plugin_section, 'database_location',
                                 '%s/%s' % (user_home,
                                 '.meta_storage/maemo_multimedia.db'))


def setup_fields_spec():
    config.manager.set_parameter(plugin_section, 'enable.field_type',
                                 'checkbox')
    config.manager.set_parameter(plugin_section, 'enable.priority',
                                 '0')

    config.manager.set_parameter(plugin_section,
                                 'database_location.field_type',
                                 'select-file')
    config.manager.set_parameter(plugin_section, 'database_location.priority',
                                 '1')


def main():
    setup_basic_configuration()
    setup_fields_spec()
    config.manager.save()


if __name__ == '__main__':
    main()
