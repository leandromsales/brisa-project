# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

#The new plugin are hard coded here
#they will be loaded dinamically in future

from os import path, walk

modules = set()
modules_ext=['.py','.pyc','.pyo']
brisa_root='.'
plugin_root=path.join(brisa_root,'media_services','cds','plugins')
for root, dirs, files in walk(plugin_root):
    for file in files:
        (file_root, ext) = path.splitext(file)
        if ext in modules_ext:
            modules.add(file_root)
for m in modules:
    __import__('media_services.cds.plugins.' + m)
