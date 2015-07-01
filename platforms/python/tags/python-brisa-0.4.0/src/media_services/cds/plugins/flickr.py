"""
    flickr.py
    Copyright 2004-6 James Clarke <james@jamesclarke.info>

THIS SOFTWARE IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, AND MAY BE
COPIED, MODIFIED OR DISTRIBUTED IN ANY WAY, AS LONG AS THIS NOTICE
AND ACKNOWLEDGEMENT OF AUTHORSHIP REMAIN.

2006-12-19
Applied patches from Berco Beute and Wolfram Kriesing.
TODO list below is out of date!

2005-06-10
TOOD list:
* flickr.blogs.*
* flickr.contacts.getList
* flickr.groups.browse
* flickr.groups.getActiveList
* flickr.people.getOnlineList
* flickr.photos.getContactsPhotos
* flickr.photos.getContactsPublicPhotos
* flickr.photos.getContext
* flickr.photos.getCounts
* flickr.photos.getExif
* flickr.photos.getNotInSet
* flickr.photos.getPerms
* flickr.photos.getRecent
* flickr.photos.getUntagged
* flickr.photos.setDates
* flickr.photos.setPerms
* flickr.photos.licenses.*
* flickr.photos.notes.*
* flickr.photos.transform.*
* flickr.photosets.getContext
* flickr.photosets.orderSets
* flickr.reflection.* (not important)
* flickr.tags.getListPhoto
* flickr.urls.*
"""

__author__ = "James Clarke <james@jamesclarke.info>"
__version__ = "$Rev: 23 $"
__date__ = "$Date: 2006-12-19 15:10:06 +0000 (Tue, 19 Dec 2006) $"
__copyright__ = "Copyright 2004-6 James Clarke"

from urllib import urlencode, urlopen
from xml.dom import minidom
from commands import getoutput
import os
import md5

HOST = 'http://flickr.com'
API = '/services/rest'
AUTH_API = '/services/auth'
#AUTH=False

#set these here or using flickr.API_KEY in your application
API_KEY = 'kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk'
SECRET = 'ssssssssssssss'
CACHE_DIR = os.path.expanduser('~')+ "/.flickr"
email = None
password = None

class FlickrError(Exception): pass

class Photo(object):
    """Represents a Flickr Photo."""

    __readonly = ['id', 'secret', 'server', 'isfavorite', 'license', 'rotation',
                  'owner', 'dateposted', 'datetaken', 'takengranularity',
                  'title', 'description', 'ispublic', 'isfriend', 'isfamily',
                  'cancomment', 'canaddmeta', 'comments', 'tags', 'permcomment',
                  'permaddmeta']

    #XXX: Hopefully None won't cause problems
    def __init__(self, id, owner=None, dateuploaded=None, \
                 title=None, description=None, ispublic=None, \
                 isfriend=None, isfamily=None, cancomment=None, \
                 canaddmeta=None, comments=None, tags=None, secret=None, \
                 isfavorite=None, server=None, license=None, rotation=None):
        """Must specify id, rest is optional."""
        self.__loaded = False
        self.__cancomment = cancomment
        self.__canaddmeta = canaddmeta
        self.__comments = comments
        self.__dateuploaded = dateuploaded
        self.__description = description
        self.__id = id
        self.__license = license
        self.__isfamily = isfamily
        self.__isfavorite = isfavorite
        self.__isfriend = isfriend
        self.__ispublic = ispublic
        self.__owner = owner
        self.__rotation = rotation
        self.__secret = secret
        self.__server = server
        self.__tags = tags
        self.__title = title

        self.__dateposted = None
        self.__datetaken = None
        self.__takengranularity = None
        self.__permcomment = None
        self.__permaddmeta = None

    def __setattr__(self, key, value):
        if key in self.__class__.__readonly:
            raise AttributeError("The attribute %s is read-only." % key)
        else:
            super(Photo, self).__setattr__(key, value)

    def __getattr__(self, key):
        #print "getting "+str(key)
        #print self.__ispublic
        auth=True
        #print self.__ispublic=='1'
        if self.__ispublic=='1':
            auth=False
        #print str(key)+str(auth)
        if not self.__loaded:
            #print 'loading '+str(key)
            self._load_properties(auth)
        if key in self.__class__.__readonly:
            return super(Photo, self).__getattribute__("_%s__%s" % (self.__class__.__name__, key))
        else:
            return super(Photo, self).__getattribute__(key)

    def _load_properties(self,_auth=False):
        """Loads the properties from Flickr."""
        self.__loaded = True

        method = 'flickr.photos.getInfo'
        data = _doget(method, photo_id=self.id,auth=_auth)

        photo = data.rsp.photo

        self.__secret = photo.secret
        self.__server = photo.server
        self.__isfavorite = photo.isfavorite
        self.__license = photo.license
        self.__rotation = photo.rotation



        owner = photo.owner
        self.__owner = User(owner.nsid, username=owner.username,\
                          realname=owner.realname,\
                          location=owner.location)

        self.__title = photo.title.text
        self.__description = photo.description.text
        self.__ispublic = photo.visibility.ispublic
        self.__isfriend = photo.visibility.isfriend
        self.__isfamily = photo.visibility.isfamily

        self.__dateposted = photo.dates.posted
        self.__datetaken = photo.dates.taken
        self.__takengranularity = photo.dates.takengranularity

        self.__cancomment = photo.editability.cancomment
        self.__canaddmeta = photo.editability.canaddmeta
        self.__comments = photo.comments.text

        try:
            self.__permcomment = photo.permissions.permcomment
            self.__permaddmeta = photo.permissions.permaddmeta
        except AttributeError:
            self.__permcomment = None
            self.__permaddmeta = None

        #TODO: Implement Notes?
        if hasattr(photo.tags, "tag"):
            if isinstance(photo.tags.tag, list):
                self.__tags = [Tag(tag.id, User(tag.author), tag.raw, tag.text) \
                               for tag in photo.tags.tag]
            else:
                tag = photo.tags.tag
                self.__tags = [Tag(tag.id, User(tag.author), tag.raw, tag.text)]


    def __str__(self):
        return '<Flickr Photo %s>' % self.id


    def setTags(self, tags):
        """Set the tags for current photo to list tags.
        (flickr.photos.settags)
        """
        method = 'flickr.photos.setTags'
        tags = uniq(tags)
        _dopost(method, auth=True, photo_id=self.id, tags=tags)
        self._load_properties()


    def addTags(self, tags):
        """Adds the list of tags to current tags. (flickr.photos.addtags)
        """
        method = 'flickr.photos.addTags'
        if isinstance(tags, list):
            tags = uniq(tags)

        _dopost(method, auth=True, photo_id=self.id, tags=tags)
        #load properties again
        self._load_properties()

    def removeTag(self, tag):
        """Remove the tag from the photo must be a Tag object.
        (flickr.photos.removeTag)
        """
        method = 'flickr.photos.removeTag'
        tag_id = ''
        try:
            tag_id = tag.id
        except AttributeError:
            raise FlickrError, "Tag object expected"
        _dopost(method, auth=True, photo_id=self.id, tag_id=tag_id)
        self._load_properties()


    def setMeta(self, title=None, description=None):
        """Set metadata for photo. (flickr.photos.setMeta)"""
        method = 'flickr.photos.setMeta'

        if title is None:
            title = self.title
        if description is None:
            description = self.description

        _dopost(method, auth=True, title=title, \
               description=description, photo_id=self.id)

        self.__title = title
        self.__description = description


    def getURL(self, size='Medium', urlType='url'):
        """Retrieves a url for the photo.  (flickr.photos.getSizes)

        urlType - 'url' or 'source'
        'url' - flickr page of photo
        'source' - image file
        """
        method = 'flickr.photos.getSizes'
        _auth=True
        if self.__ispublic=='1':
            _auth=False
#        print _auth
        data = _doget(method, photo_id=self.id,auth=_auth)
        for psize in data.rsp.sizes.size:
            if psize.label == size:
                return getattr(psize, urlType)
        for psize in data.rsp.si
