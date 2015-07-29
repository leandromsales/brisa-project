# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>
import os
import sys
from os import path

from sqlobject import *

UP_dir = os.path.expanduser('~') + '/.brisa_userprofile'
if not os.path.isdir(UP_dir):
    os.mkdir(UP_dir)
session_db = 'sqlite://' + UP_dir + '/database.db'
sqlhub.processConnection = connectionForURI(session_db)


class User(SQLObject):

    class sqlmeta:
        lazyUpdate = True
        cacheValues = False

    firstName = StringCol(length=50)
    lastName = StringCol(length=50)
    email = StringCol(length=100)
    username = StringCol(length=50)
    password = StringCol(length=10)
    uuid = StringCol(length=100)
    session = MultipleJoin('Session')
    profile = SingleJoin('Profile')


class Session(SQLObject):
    user = ForeignKey('User')
    lastAuth = StringCol(length=50)
    lastRenewSession = StringCol(length=50)


class Profile(SQLObject):
    user = ForeignKey('User')
    category = MultipleJoin('Category')


class Category(SQLObject):
    profile = ForeignKey('Profile')
    name = StringCol(length=50)
    categoryID = StringCol(length=10)
    categoryFields = MultipleJoin('CategoryFields')


class CategoryFields(SQLObject):
    category = ForeignKey('Category')
    type = StringCol(length=50)
    userCategoryFields = MultipleJoin('UserCategoryFields')


class UserCategoryFields(SQLObject):
    categoryFields = ForeignKey('CategoryFields')
    name = StringCol(length=50)
    relationship = MultipleJoin('Relationship')


class Relationship(SQLObject):
    artist = ForeignKey('UserCategoryFields')
    genre = ForeignKey('UserCategoryFields')


class Data:

    def __init__(self, *args, **kwargs):
        User.createTable(ifNotExists=1)
        Session.createTable(ifNotExists=1)
        Profile.createTable(ifNotExists=1)
        Category.createTable(ifNotExists=1)
        CategoryFields.createTable(ifNotExists=1)
        UserCategoryFields.createTable(ifNotExists=1)
        Relationship.createTable(ifNotExists=1)
