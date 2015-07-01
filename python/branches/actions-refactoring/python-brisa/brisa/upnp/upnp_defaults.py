# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" UPnP constants defined on the 1.0 specification.
"""


class UPnPDefaults(object):
    """ UPnP constants defined on the 1.0 specification.
    """
    SERVICE_ID_PREFIX = "urn:upnp-org:serviceId:"
    SERVICE_ID_MS_PREFIX = "urn:microsoft.com:serviceId:"
    SERVICE_SCHEMA_VERSION = "service-1-0"
    SCHEMA_VERSION = "device-1-0"
    SCHEMA_VERSION_MAJOR = "1"
    SCHEMA_VERSION_MINOR = "0"
    NAME_SPACE_XML_SCHEMA = '%s%s' % ("urn:schemas-upnp-org:", SCHEMA_VERSION)
    SSDP_PORT = 1900
    SSDP_ADDR = '239.255.255.250'
    MSEARCH_DEFAULT_SEARCH_TIME = 600.0
    MSEARCH_DEFAULT_SEARCH_TYPE = 'ssdp:all'
