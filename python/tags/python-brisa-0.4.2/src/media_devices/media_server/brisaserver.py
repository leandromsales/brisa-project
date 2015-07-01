# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>


# Implementation of a simple daemon Application of UPnP Media Server
# It is based on the Twisted Framework

import os.path
import shutil
import os
import logging
import random
import sys
import socket
import platform
import thread

from optparse import OptionParser

from twisted.internet import reactor
from twisted.web import resource, static

import brisa_config

from upnp import RootDevice, Service, DeviceHandler
from persistence import Facade

from media_services.cds import ContentDirectoryControl
from media_services.cds import ContentDirectory
from media_services.cds.plugins.plugin import Plugin
from media_services.cds.plugins.plugin import PluginManager
from media_services.connmgr import ConnectionManagerControl
from server_watcher import ServerWatcher
from brisa_utils import network


# This implementation for obtaining the IP address of an interface on
# Linux (eth0, for example) was taken from Paul Cannon on ASPN Python
# Cookbook, see
# http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/439094
#
#def get_ip_address(ifname):
#     import socket
#     import fcntl
#     from struct import pack, unpack
#
#     s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#     return socket.inet_ntoa(fcntl.ioctl(
#         s.fileno(),
#         0x8915,  # SIOCGIFADDR
#         pack('256s', ifname[:15])
#     )[20:24])


def directory_parser(directories):
    #TODO: Some error handle code
    sdls = directories.split(",")
    list_of_sdl = []

    for sd in sdls:
        mediaPathName, mediaPath = sd.split(":")
        list_of_sdl.append({'klass': FSDirectory, 'path': mediaPath, \
                            'title': mediaPathName})

    return list_of_sdl


def parse_args():
    usage = "\nBrisa UPnP MediaServer version " + \
    brisa_config.conf.brisa_version + " \nusage: python %prog [options] arg"
    parser = OptionParser(usage)

    parser.add_option("-v", "--verbose",
                      action="store_true", dest="verbose")
    parser.add_option("-q", "--quiet",
                      action="store_false", dest="verbose")
    parser.add_option("-i", "--address", dest="address",
                      help="set the UPnP MediaServer IP address")
    parser.add_option("-p", "--port", dest="port",
                      help="set the UPnP MediaServer Port")

    (options, args) = parser.parse_args()

    if len(args) != 0:
        parser.error("incorrect number of arguments")

    if options.address is None:
        if brisa_config.contains("connection", "listenif"):
            interface = brisa_config.get_parameter("connection", "listenif")
            listenAddr = network.get_ip_address(interface)
        elif platform.uname()[4].find('arm') is not -1:
                listenAddr = network.get_ip_address('wlan0')
        elif platform.system() == 'Linux':
            try:
                listenAddr = network.get_ip_address('eth0')
            except:
                logging.error(
                    'Can not get ip adress give it using the option --address')
                sys.exit()
    else:
        listenAddr = options.address

    if options.port is None:
        listenPort = random.randint(10000, 65000)
    else:
        listenPort = options.port

    listenURL = "http://" + str(listenAddr) + ":" + str(listenPort)

    return (options, listenAddr, listenPort, listenURL)


def show_startup_message(listenURL):
    logging.info("Brisa UPnP Media Server v. " + \
brisa_config.conf.brisa_version)
    logging.info("Server is listening at address " + listenURL)
    logging.info("Press Ctrl+C to stop the server")
    print "Brisa Media Server version %s started. Please refer to %s for \
more information." % (brisa_config.conf.brisa_version, logging.filename)


def first_server_init():
    Facade.createDB()
    root = Facade.addContainer(title="brisaServer")
    audioFolder = Facade.addContainer(title="Audio", container=root)
    videoFolder = Facade.addContainer(title="Video", container=root)
    imageFolder = Facade.addContainer(title="Image", container=root)

    genreFolder = Facade.addContainer(title="Audio Genre", \
                                        container=audioFolder)
    artistFolder = Facade.addContainer(title="Audio Artists", \
                                         container=audioFolder)
    albumFolder = Facade.addContainer(title="Audio Albums", \
                                        container=audioFolder)
    broadcastFolder = Facade.addContainer(title="Audio Broadcast", \
                                            container=audioFolder)
    videoGenreFolder = Facade.addContainer(title="Video Genre", \
                                             container=videoFolder)
    videoFolders = Facade.addContainer(title="Video Folders", \
                                         container=videoFolder)
    videoBroadcastFolder = Facade.addContainer(title="Video Broadcast", \
                                                 container=videoFolder)
    imageFolders = Facade.addContainer(title="My Images", \
                                         container=imageFolder)
    imageBlog = Facade.addContainer(title="Internet Photos", \
                                      container=imageFolder)


def create_root_device(listenURL, serverName):
    root_device = RootDevice("MediaServer:1", serverName, listenURL)
    root_device.manufacturer = "Brisa Team. Embedded Laboratory and INdT \
Brazil"
    root_device.manufacturer_url = "https://garage.maemo.org/projects/brisa/"
    root_device.model_description = "An Opensource UPnP A/V Media Server"
    root_device.model_name = "Brisa Media Server version " + \
                                brisa_config.conf.brisa_version
    root_device.model_number = brisa_config.conf.brisa_version
    root_device.model_url = "https://garage.maemo.org/projects/brisa/"
    root_device.serial_number = "0000004"
    return root_device


def create_server_service(listenURL):
    # Creating a Content Directory Service
    cds = Service(listenURL,
                        "ContentDirectory/control",
                        "ContentDirectory/event",
                        "ContentDirectory/presentation",
                        "ContentDirectory/scpd.xml")
    cds.set_service_id("11")
    cds.set_service_type_version("ContentDirectory:1")
    cds.add_description('media_services/cds/content-directory-scpd.xml')
    cds.insertControl(ContentDirectory())

    # Creating a ConnectionManager Service
    cm = Service(listenURL,
                     "ConnectionManager/control",
                     "ConnectionManager/event",
                     "ConnectionManager/presentation",
                     "ConnectionManager/scpd.xml")
    cm.set_service_id("22")
    cm.set_service_type_version("ConnectionManager:1")
    cm.add_description('media_services/connmgr/connection-manager-scpd.xml')
    cm.insertControl(ConnectionManagerControl())

    return cds, cm


def load_shared_content_to_db(sharedURL):
    #Configure DB
    dbpath = brisa_config.get_parameter('persistence', 'connection')
    dbpath = dbpath.split(':')[1]
    if os.path.exists(dbpath):
        Facade.sync_database()
    else:
        first_server_init()

    #Creating plugin Manager and Adding/Config plugins
    plugin_manager = PluginManager()
    plugin_manager.setup_plugins()
    thread.start_new_thread(plugin_manager.exec_plugins, ())

    Facade.notify_shared_url(sharedURL)


def create_server_watcher():
    sw = ServerWatcher()
    import thread
    thread.start_new_thread(sw.run, ())


class ServerResource(resource.Resource):
    def __init__(self):

        resource.Resource.__init__(self)

    def getChildWithDefault(self, path, request):
        item = Facade.selectItemById(path)
        if item:
            local_path = item.local_path
            return static.File(local_path)
        else:
            return static.File("/dev/null")


def create_resource():
    return ServerResource()


def main():
    #import time
    #start = time.time()
    (options, listenAddr, listenPort, listenURL) = parse_args()
    serverName = "Brisa MediaServer at " + socket.gethostname()
    show_startup_message(listenURL)

    # Create the root device object which will represent the device \
    #description
    root_device = create_root_device(listenURL, serverName)

    # Create services 'content directory' and 'connection manager'
    (cds, cm) = create_server_service(listenURL)

    # Adding the services into the device
    root_device.add_service(cds)
    root_device.add_service(cm)

    # Create a content of resource and share the media content
    content_name = "content"
    content = create_resource()
    shared_url = listenURL + "/" + content_name
    load_shared_content_to_db(shared_url)

    # Create a dirs watcher to be notified of new files add to any shared dir
    create_server_watcher()

    # Inserting "extra" web resources to be added in the root of devices \
    #webserver
    root_device.add_webResource((content_name, content))

    # Creating a handler for the Device
    deviceHandler = DeviceHandler()

    #or you can use the reactor from Twisted. You just need to configuring \
    #the device and then start the reactor
    deviceHandler.config_device(root_device, options.verbose, serverName)
    #end = time.time()
    #ti = end - start
    #print "Time of initialization: " + str(ti)
    reactor.run()

if __name__ == "__main__":
    main()