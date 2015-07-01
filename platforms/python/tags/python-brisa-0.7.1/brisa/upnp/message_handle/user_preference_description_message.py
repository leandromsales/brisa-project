# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from xml.etree import ElementTree
from xml.etree.ElementTree import Element, SubElement, dump
from brisa.upnp.upnp_defaults import UPnPDefaults
from brisa import upnp
from brisa.services.up_profile.persistence.database import User
from brisa.services.up_profile.persistence.database import Relationship
from brisa import config
from os import path


class UserPreferenceDescriptionMessage(object):

    def __init__(self, username, password):
        self.xml = ""

        count = 1
        countR = 1
        list_artist = []

        relation = Relationship.select()
        artist_number = relation.count()

        while countR <= artist_number:
            list_artist.append(Relationship.get(countR))
            countR += 1

        user = User.select()
        user_number = user.count()
        user_authentication = None

        while count <= user_number:
            if User.get(count).username == username and \
               User.get(count).password == password:
                user_authentication = User.get(count)
            count +=1

        userID = user_authentication.id
        dict_category = {}
        dict_genre = {}

        for category in user_authentication.profile.category:
            dict_category["id"] = category.categoryID
            dict_category["name"] = category.name

            for categoryField in category.categoryFields:
                if categoryField.type == "Genre":
                    for userCategoryField in categoryField.userCategoryFields:
                        name = userCategoryField.name
                        id = userCategoryField.id
                        list_artist_name = []
                        for artist in list_artist:
                            if id == artist.genre.id:
                                list_artist_name.append(artist.artist.name)

                        dict_genre[name]=list_artist_name


        self.userID = str(userID)
        self.category = dict_category
        self.genre = dict_genre

    def create_xml_elements(self):
        self.xml = Element("root", xmlns = "urn:schemas-upnp-org:" +
                           UPnPDefaults.SCHEMA_VERSION)
        specVersion_element = SubElement(self.xml, "specVersion")
        element = SubElement(specVersion_element, "major")
        element.text = UPnPDefaults.SCHEMA_VERSION_MAJOR
        element = SubElement(specVersion_element, "minor")
        element.text = UPnPDefaults.SCHEMA_VERSION_MINOR
        userProfile_element = SubElement(self.xml, "userProfile",
                                          id = self.userID)
        elementCategoryList = SubElement(userProfile_element,
                                         "upnpCategoryList")
        elementCategory = SubElement(elementCategoryList, "category",
                                     id = self.category["id"],
                                     name = self.category["name"])
        self.elementGenreList = SubElement(elementCategory, "genreList")

        for key in self.genre.keys():
            title = key
            list_artist = self.genre[title]
            self.create_genre(title, list_artist)

    def create_genre(self, title, list_artist):
        self.title = title
        self.list_artist = list_artist
        self.elementGenre = SubElement(self.elementGenreList, "genre",
                                       title = self.title)
        for artist in self.list_artist:
            self.elementArtist = SubElement(self.elementGenre, "artist")
            self.elementArtist.text = artist

    def dump_xml(self):
        self.create_xml_elements()
        self.to_xml(path.join(config.conf.brisa_home, '"userPreference"'))
        xml = ElementTree.tostring(self.xml, encoding='utf-8')
        return xml

    def to_xml(self, location):
        ElementTree.ElementTree(self.xml).write(location)
