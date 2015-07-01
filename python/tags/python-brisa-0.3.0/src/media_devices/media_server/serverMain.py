# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team


# Implementation of a simple Application that uses the UPnP Framework
# It is based on the Twisted Framework


import random
import string
import sys
import time
import signal, os

#from twisted.internet import reactor

from upnp import RootDevice, Service, DeviceHandler

from media_services.cds.FSStorage import FSDirectory
from media_services.cds import ContentDirectoryControl

from media_services.connmgr import ConnectionManagerControl

from twisted.python import log
from twisted.web import resource


if __name__ == '__main__':

    print "Brisa UPnP Media Server v. 0.1"

    print "Type the local IP Address:"
    listenAddr = raw_input("---> ")

    print "Type a listen port:"
    listenPort = raw_input("---> ")

    print "How many directory you want to share? (default is one)"
    numberOfDirectories = raw_input("---> ")

    if numberOfDirectories is "":
        numberOfDirectories = 1

    list_of_sdl = []

    for i in range(int(numberOfDirectories)):
        print "Type the media content directory path number "+str(i+1)+":"
        mediaPath = raw_input("---> ")
        print "Type the name you want this directory to be displayed:"
        mediaPathName = raw_input("---> ")
        list_of_sdl.append({'klass':FSDirectory, 'path':mediaPath,'title':mediaPathName})

    #print "Type the media content directory (default is media):"
    #mediaPath = raw_input("---> ")

    if mediaPath == "":
        mediaPath = "media"

    print "Enable log (y) or (n)?"
    logOn = raw_input("---> ")

    if logOn == "y":
        logOn = True
    elif logOn == "n":
        logOn = False
    else:
        print "Not a valid statement, log will be set ON"
        logOn = True

    listenURL = "http://" + listenAddr + ":" + listenPort

    print "now, starting the server at " + listenURL
    print ("type any key to stop!")

    # Creating a random udn
    uuid = ''.join(['uuid:'] + [ random.choice(string.letters) for x in xrange(20) ])
    #uuid = '123438asdkf'

    # Creating a root device and setting some features
    myDevice = RootDevice("MediaServer:1", "Brisa MediaServer", uuid, listenURL)

    myDevice.set_manufacturer("Brisa Team");
    myDevice.set_manufacturer_url("http://www.percomp.org");
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
    #sdl = [{'klass':FSDirectory, 'path':mediaPath,'title':'Media'}]
    control = ContentDirectoryControl('My Media Server',
                                      storageDirectoryList = list_of_sdl,
                                      urlbase = listenURL+"/content",
                                      webbase = content)
    myService.insertControl(control)

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
    deviceHandler = DeviceHandler()

    # Starting the device with its own thread
    deviceHandler.startDevice(myDevice, logOn)

    #or you can use the reactor from Twisted. You just need to configuring the device and then start the reactor
    #deviceHandler.config_device(myDevice, logOn)
    #reactor.run()

    # Just some test code
    #print("going to sleep...")
    #sleep(10)
    #print ("bye, bye")

    # some POG Code !! to deal with deviceHandler Thread
    nothing = raw_input("---> ")

    deviceHandler.stopDevice()
