# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

import os
import sys
from os import path

from sqlobject import *

session_db = 'sqlite://' + os.path.dirname(__file__) +'/up_database.db'


class User(SQLObject):
    _connection = connectionForURI(session_db)
    firstName = StringCol(length=50)
    lastName = StringCol(length=50)
    email = StringCol(length=100)
    username = StringCol(length=50)
    password = StringCol(length=10)
    uuid = StringCol(length=100)
    session = MultipleJoin('Session')
    profile = SingleJoin('Profile')


class Session(SQLObject):
    _connection = connectionForURI(session_db)
    user = ForeignKey('User')
    lastAuth = StringCol(length=50)
    lastRenewSession = StringCol(length=50)


class Profile(SQLObject):
    _connection = connectionForURI(session_db)
    user = ForeignKey('User')
    category = MultipleJoin('Category')


class Category(SQLObject):
    _connection = connectionForURI(session_db)
    profile = ForeignKey('Profile')
    name = StringCol(length=50)
    categoryID = StringCol(length=10)
    categoryFields = MultipleJoin('CategoryFields')


class CategoryFields(SQLObject):
    _connection = connectionForURI(session_db)
    category = ForeignKey('Category')
    type = StringCol(length=50)
    userCategoryFields = MultipleJoin('UserCategoryFields')


class UserCategoryFields(SQLObject):
    _connection = connectionForURI(session_db)
    categoryFields = ForeignKey('CategoryFields')
    name = StringCol(length=50)
    relationship = MultipleJoin('Relationship')


class Relationship(SQLObject):
    _connection = connectionForURI(session_db)
    artist = ForeignKey('UserCategoryFields')
    genre = ForeignKey('UserCategoryFields')


class Data:

    def __init__(self):
        User.createTable(ifNotExists=1)
        Session.createTable(ifNotExists=1)
        Profile.createTable(ifNotExists=1)
        Category.createTable(ifNotExists=1)
        CategoryFields.createTable(ifNotExists=1)
        UserCategoryFields.createTable(ifNotExists=1)
        Relationship.createTable(ifNotExists=1)

        self.create_database()

    def create_database(self):
        self.u1 = User(firstName = "Fernanda", lastName = "Campos",
                        email = "fernanda@oi.com", username = "nanda",
                        password = "123456", uuid = "")

        p1 = Profile(user = self.u1)

        c1 = Category(profile = p1, name = "AudioVideoImage",
                      categoryID = "10")

        cf10 = CategoryFields(category = c1, type = "Genre")
        cf11 = CategoryFields(category = c1, type = "Artist")

        ucf1 = UserCategoryFields(categoryFields = cf10, name = "MPB")
        ucf11 = UserCategoryFields(categoryFields = cf10, name = "Rock")
        ucf2 = UserCategoryFields(categoryFields = cf11,
                                  name = "Caetano Veloso")
        ucf22 = UserCategoryFields(categoryFields = cf11,
                                   name = "Guns N Roses")
        ucf23 = UserCategoryFields(categoryFields = cf11, name = "Gilberto")

        r1 = Relationship(artist = ucf2, genre = ucf1)
        r11 = Relationship(artist = ucf22, genre = ucf11)
        r12 = Relationship(artist = ucf23, genre = ucf1)


        self.u2 = User(firstName = "Taciana", lastName = "Rached",
                        email = "taciana@oi.com", username = "taci",
                        password = "654321", uuid = "")

        p2 = Profile(user = self.u2)

        c2 = Category(profile = p2, name = "AudioVideoImage",
                      categoryID = "10")

        cf20 = CategoryFields(category = c2, type = "Genre")
        cf21 = CategoryFields(category = c2, type = "Artist")

        ucf3 = UserCategoryFields(categoryFields = cf20, name = "MPB")
        ucf31 = UserCategoryFields(categoryFields = cf20, name = "Axe")
        ucf32 = UserCategoryFields(categoryFields = cf21,
                                   name = "Chiclete com Banana")

        r2 = Relationship(artist = ucf32, genre = ucf31)
