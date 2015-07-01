# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

from os import path, walk
from brisa import log

module_dir_plugin = 'brisa.services.cds.plugins'
fs_dir_plugin = path.dirname(__file__)

for root, dirs, files in walk(fs_dir_plugin):
    for dir in dirs:
        if not dir.startswith("."):
            try:
                module_path = '%s.%s' % (module_dir_plugin, dir)
                __import__(module_path)
            except Exception, e:
                msg = 'Error while importing %s plugin. The module \
path used to load was: %s' % (dir, module_path)
                log.error(msg)
                log.show_exception(msg, e)
    break

del path
del walk
del module_dir_plugin
del fs_dir_plugin
del root
del files
del dirs
try:
    del dir
    del e
    del module_path
except:
    pass
