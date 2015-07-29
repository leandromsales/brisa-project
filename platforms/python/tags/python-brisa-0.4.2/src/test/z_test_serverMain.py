# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a simple Application that uses the UPnP Framework
# It is based on the Twisted Framework


import random
import string
import sys
import time

from upnp import RootDevice
from upnp import Service
from upnp import DeviceHandler

from media_services.cds.FSStorage import FSDirectory

from media_services.cds import ContentDirectoryControl
from media_devices.media_server.myControl import MyControl

from twisted.python import log
from twisted.internet import reactor
from twisted.web import resource

if __name__ == '__main__':
    print "Cartola UPnP Media Server v. 0.01"
    print "Type the local IP Address:"
    listenAddr = raw_input("---> ")

    print "Type a listen port:"
    listenPort = raw_input("---> ")

    print "Type the media content directory (default is media):"
    mediaPath = raw_input("---> ")

    if listenAddr == "":
        listenAddr = "192.168.1.155"
    if mediaPath == "":
        mediaPath = "/home/adrian/Musicas/Alejandro Sanz"
    if listenPort == "":
        listenPort = "11223"


    print "Enable log (y) or (n)?"
    logOn = raw_input("---> ")

    if logOn == "":
        logOn = "n"

    if logOn == "y":
        logOn = True
    elif logOn == "n":
        logOn = False
    else:
        print "Not valid statement, log will be set ON"
        logOn = True

    listenURL = "http://" + listenAddr +":"+ listenPort

    print "now, starting the server at " + listenURL
    # Criando uma udn aleatoriamente
    min = random.choice(string.letters)
    max = xrange(20)
    uuid = ''.join(['uuid:'] + map(lambda x: min, max))
    #uuid = '123438asdkf'
    # Criando meu Device e setando algumas variaveis
    myDevice = RootDevice("MediaServer:1", "Cartola MediaServer", uuid,\
listenURL)
    myDevice.set_manufacturer("teste_manufacturer")
    myDevice.set_manufacturer_url("http://www.percomp.org")
    myDevice.set_model_description("Our first MediaServer Implementation")
    #myDevice.set_model_name("teste_model_name")
    myDevice.set_model_number("model 01")
    #myDevice.set_model_url("teste_model_url")
    myDevice.set_serial_number("0000001")


    # Criando um servico
    myService = Service(listenURL, "ContentDirectory/control",    \
    "ContentDirectory/event", "ContentDirectory/presentation", \
    "ContentDirectory/scpd.xml")
    myService.set_service_id("11")
    myService.set_service_type_version("ContentDirectory:1")

    # Inserindo um control para lidar com os actions
    content = resource.Resource()
    myService.add_description('content-directory-scpd.xml')
    sdl =[
          {'klass': FSDirectory, 'title': 'Alejandro Sanz', 'path': \
           "/home/adrian/Musicas/Alejandro Sanz"},
          {'klass': FSDirectory, 'title': 'All Music', 'path': \
           "/home/adrian/Musicas/"},
          ]
    control = ContentDirectoryControl('My Media Server',
                                      storageDirectoryList = sdl, \
                                      urlbase = listenURL+"/content",\
                                      webbase = content)
#
    myService.insertControl(control)

    # Criando um servico
    myService2 = Service(listenURL, "ConnectionManager/control", \
    "ConnectionManager/event", "ConnectionManager/presentation", \
    "ConnectionManager/scpd.xml")
    myService2.set_service_id("22")
    myService2.set_service_type_version("ConnectionManager:1")


    # Inserindo um control (vazio) para lidar com os actions
    myService2.add_description('connection-manager-scpd.xml')
    myService2.insertControl(MyControl())

    # Inserindo o servico no device
    myDevice.add_service(myService)
    myDevice.add_service(myService2)

    #inserindo resources extras para o webserver
    myDevice.add_webResource(('content', content))

    # Criando o handler para device
    deviceHandler = DeviceHandler()

    # iniciando o device
    deviceHandler.config_device(myDevice, logOn)
    #thread.start_new_thread(DeviceHandler.startDevice, (deviceAPI,myDevice))
    reactor.run()