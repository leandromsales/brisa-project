# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from upnp.UPnPMessageHandler import DeviceDescriptionMessage
from upnp import RootDevice, Device, DeviceIcon
from upnp import Service

if __name__ == "__main__":
    root_device = RootDevice("Cartola:1", "CanolaDevice", "udn", "location")
    root_device.set_manufacturer("teste_manufacturer")
    root_device.set_manufacturer_url("teste_manufacturer_url")
    root_device.set_model_description("teste_model_description")
    root_device.set_model_name("teste_model_name")
    root_device.set_model_number("teste_model_number")
    root_device.set_model_url("teste_model_url")
    root_device.set_serial_number("teste_serial_number")
    root_device.set_presentation_url("teste_presentation_url")
    root_device.set_upc("teste_upc")
    icon1 = DeviceIcon("image/png", "122", "122", "24", "imagens/icon.png")
    root_device.add_icon(icon1)
    icon2 = DeviceIcon("image/gif", "12", "12", "16", "imagens/icon.gif")
    root_device.add_icon(icon2)
    service = Service("http://www.cartola.com.br/", "MediaServer/control/", \
      "MediaServer/events/", "MediaServer/presentation/", "MediaServer/scdp/")
    service.set_service_type_version("MediaServer:2")
    service.set_service_id("10")
    root_device.add_service(service)
    #Creating a embedded device and add it into RootDevice created above
    emb_device = Device("CartolaEmbeddedDevice:1", "CartolaEmbeddedDevice", \
      "udn", "Cartola:1", "location")
    emb_device.set_manufacturer("emb_teste_manufacturer")
    emb_device.set_manufacturer_url("emb_teste_manufacturer_url")
    emb_device.set_model_description("emb_teste_model_description")
    emb_device.set_model_name("emb_teste_model_name")
    emb_device.set_model_number("emb_teste_model_number")
    emb_device.set_model_url("emb_teste_model_url")
    emb_device.set_serial_number("emb_teste_serial_number")
    emb_device.set_presentation_url("emb_teste_presentation_url")
    emb_device.set_upc("emb_teste_upc")
    root_device.add_device(emb_device)
    ddesc = DeviceDescriptionMessage(root_device, "http://baseurl.com.br/")
    ddesc.generateXML("deviceDescription.xml")