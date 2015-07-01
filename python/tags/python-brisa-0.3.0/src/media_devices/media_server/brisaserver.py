# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>


# Implementation of a simple daemon Application of UPnP Media Server
# It is based on the Twisted Framework

# Configuring DB
import os
from config_handle.config import BrisaConf

homePath = os.path.expanduser('~')
homePath = homePath + "/.brisa"
update_conf = False
brisa_version = '0.3'

if os.path.exists(homePath):
    if os.path.isfile(homePath + "/brisa.conf"):
        conf = BrisaConf()
        if conf.contains('brisa', 'version') == False or conf.get_parameter('brisa', 'version') != brisa_version:
            os.system("/bin/rm " + homePath + "/brisa.conf")
            os.system("/bin/cp brisa.conf "+ homePath)
            update_conf = True

    else:
        os.system("/bin/cp brisa.conf "+ homePath)
        update_conf = True
else:
    os.system("/bin/mkdir " + homePath)
    os.system("/bin/cp brisa.conf " + homePath)
    update_conf = True

conf = BrisaConf()
if update_conf:
    conf.set_parameter('persistence', 'connection',
                       'sqlite:' + homePath + '/brisaobject.db'
                       )
db = conf.get_parameter('persistence', 'connection')
db = db.split(':')[1]
if os.path.exists(db):
    os.system('/bin/rm ' + db)

from optparse import OptionParser
import random
import string
import sys
import socket
import platform

from upnp import RootDevice, Service, DeviceHandler

from media_services.cds.FSStorage import FSDirectory
from media_services.cds import ContentDirectoryControl
from media_services.cds import ContentDirectorySql

from media_services.cds.plugins.plugin import Plugin
from media_services.cds.plugins.plugin import PluginManager

from media_services.connmgr import ConnectionManagerControl

from twisted.internet import reactor
from twisted.python import log
from twisted.web import resource

# This implementation for obtaining the IP address of an interface on
# Linux (eth0, for example) was taken from Paul Cannon on ASPN Python
# Cookbook, see
# http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/439094
#
def get_ip_address(ifname):
     import socket
     import fcntl
     from struct import pack, unpack

     s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
     return socket.inet_ntoa(fcntl.ioctl(
         s.fileno(),
         0x8915,  # SIOCGIFADDR
         pack('256s', ifname[:15])
     )[20:24])


def directory_parser(directories):
    #TODO: Some error handle code

    sdls = directories.split(",")
    list_of_sdl = []

    for sd in sdls:
        mediaPathName, mediaPath = sd.split(":")
        list_of_sdl.append({'klass':FSDirectory, 'path':mediaPath,'title':mediaPathName})

    return list_of_sdl

def main():

    logOn = False
    listenPort = 0

    usage = "\nBrisa UPnP MediaServer version 0.3 \nusage: python %prog [options] arg"

    parser = OptionParser(usage)
#    parser.add_option("-d", "--directory", dest="directory", metavar="\"name:path,name1:path1, ...\"",
#                      help="set content directories to share and its names")
    parser.add_option("-v", "--verbose",
                      action="store_true", dest="verbose")
    parser.add_option("-q", "--quiet",
                      action="store_false", dest="verbose")
    parser.add_option("-i", "--address", dest="address",
                      help="set the UPnP MediaServer IP address")
    parser.add_option("-p","--port", dest="port",
                      help="set the UPnP MediaServer Port")

    (options, args) = parser.parse_args()

    if len(args) != 0:
        parser.error("incorrect number of arguments")

#    if options.directory is None:
#        parser.error("need to specify a directory name:path")

    if options.address is None:
        conf = BrisaConf()
        if conf.contains("connection", "listenif"):
            interface = conf.get_parameter("connection", "listenif")
            listenAddr = get_ip_address(interface)
        elif platform.uname()[4].find('arm') is not -1:
                listenAddr = get_ip_address('wlan0')
        elif platform.system() == 'Linux':
            listenAddr = get_ip_address('eth0')
    else:
        listenAddr = options.address

    if options.port is None:
        listenPort = random.randint(10000, 65000)
    else:
        listenPort = options.port

    listenURL = "http://" + str(listenAddr) + ":" + str(listenPort)

#    list_of_sdl = directory_parser(options.directory)

    print "Brisa UPnP Media Server v. 0.3"
    print "now, starting the server at " + listenURL
    print "press Ctrl+C or kill the process to stop the server"

    if options.verbose:
         logOn = True

        # Creating a random udn
    uuid = ''.join(['uuid:'] + [ random.choice(string.letters) for x in xrange(20) ])
    #uuid = '123438asdkf'
    serverName = "Brisa MediaServer at " + socket.gethostname()
    # Creating a root device and setting some features
    myDevice = RootDevice("MediaServer:1", serverName, uuid, listenURL)

    myDevice.set_manufacturer("Brisa Team");
    myDevice.set_manufacturer_url("https://garage.maemo.org/projects/brisa/");
    myDevice.set_model_description("Our first MediaServer Implementation");
    #myDevice.set_model_name("teste_model_name");
    myDevice.set_model_number("model 01");
    #myDevice.set_model_url("teste_model_url");
    myDevice.set_serial_number("0000001");

    # Creating a Content Directory Service

    myService = Service(listenURL, "ContentDirectory/control", "ContentDirectory/event", "ContentDirectory/presentation", "ContentDirectory/scpd.xml")
    myService.set_service_id("11")
    myService.set_service_type_version("ContentDirectory:1")

    # Inserting a control resource to deal with the actions
    content = resource.Resource()
    myService.add_description('media_services/cds/content-directory-scpd.xml')

    #Interpreting config file
    config = BrisaConf()
    mp3dirs = config.get_parameter('sound', 'dir')
    videodirs = config.get_parameter('video', 'dir')

    #Creating plugin Manager and Adding/Config plugins
    plugin_manager = PluginManager()
    plugins = plugin_manager.list_plugins()
    plugin_manager.add_plugin('filemp3',mp3dirs, webResource = content, urlbase = listenURL+"/content/")
    shoutcast_usage = config.get_parameter('shoutcast','usage')
    if shoutcast_usage == 'on':
        plugin_manager.add_plugin('shoutcast')

    plugin_manager.add_plugin('filevideo',videodirs, webResource = content, urlbase = listenURL+"/content/")
    flickr_usage = config.get_parameter('flickr', 'usage')
    if flickr_usage == 'on':
        plugin_manager.add_plugin('flickr',
                                  username=config.get_parameter('flickr', 'username'),
                                  private=config.get_parameter('flickr', 'private')
                                  )

    albums = config.get_parameter('photoalbum', 'albums')
    albums = albums.split(',')

    if isinstance(albums,list):
        for i in albums:
            plugin_manager.add_plugin('fileLocalAlbum',albumList=i,webResource = content, urlbase = listenURL+"/content/")
    else:
        plugin_manager.add_plugin('fileLocalAlbum',albumList=albums ,webResource = content, urlbase = listenURL+"/content/")

    control = ContentDirectorySql(plugin_manager)
    myService.insertControl(control)

    print "Plugins Loaded"

    # Creating a ConnectionManager Service
    myService2 = Service(listenURL, "ConnectionManager/control", "ConnectionManager/event", "ConnectionManager/presentation", "ConnectionManager/scpd.xml")
    myService2.set_service_id("22")
    myService2.set_service_type_version("ConnectionManager:1")


    # Inserting a control (empty) to deal with the actions
    myService2.add_description('media_services/connmgr/connection-manager-scpd.xml')
    myService2.insertControl(ConnectionManagerControl())

    # Adding the services into the device
    myDevice.add_service(myService)
    myDevice.add_service(myService2)

    # Inserting "extra" web resources to be "putted" in the root of devices webserver
    myDevice.add_webResource(('content',content))

    # Creating a handler for the Device

