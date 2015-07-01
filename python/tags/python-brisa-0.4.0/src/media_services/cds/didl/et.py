# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2006, Frank Scholz <coherence@beebits.net>
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# a little helper to get the proper ElementTree package
import logging

try:
    from xml.etree import cElementTree as ET
except ImportError:
    try:
        import cElementTree as ET
    except ImportError:
        try:
            from xml.etree import ElementTree as ET
        except ImportError:
            try:
                from elementtree import ElementTree as ET
            except ImportError:
                import sys
                logging.debug('no ElementTree module found, critical error')
                sys.exit(0)

try:
    from xml.etree.ElementTree import _ElementInterface
except ImportError:
    from elementtree.ElementTree import _ElementInterface

def namespace_map_update(namespaces):
    try:
        from xml.etree import ElementTree
    except ImportError:
        from elementtree import ElementTree

        ElementTree._namespace_map.update(namespaces)

class ElementInterface(_ElementInterface):
    """ helper class """
