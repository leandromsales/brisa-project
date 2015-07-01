# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from media_services.cds.plugins.plugin import Plugin
from persistence import Facade
import flickr
import os
from commands import getoutput
import logging

import brisa_config

class FlickrPlugin(Plugin):
    '''
    Plugin to acess an flickr account.
    '''
    name ='flickr'
    username=''
    private = 'no'
    use = brisa_config.get_parameter('flickr', 'usage')

    def __init__(self, *args, **kwargs):
        '''
        Initialize the flickr plugin.
        Arguments:
        args - The configuration parameters to the loadded plugin.
        '''
        Plugin.__init__(self, *args, **kwargs)
        self.username=brisa_config.get_parameter('flickr', 'username')
        self.private=brisa_config.get_parameter('flickr', 'private')
        flickr.email=brisa_config.get_parameter('flickr', 'username')
        flickr.CACHE_DIR=os.path.expanduser('~')+ '/.brisa/'
        flickr.API_KEY = 'f544dbc725689997313a6437e1068b5a'
        flickr.SECRET = '3281387735c48555'
        flickr.call_browser_method=self.browser_terminal

    def load(self):
        '''
        Load the plugin media information into the database.
        '''
        logging.info("Loading Flickr Plugin")
        user=flickr.people_findByUsername(self.username)
        #print user
        #print self.private
        if self.private=='yes':
            #print 'get private photos'
            photos = user.get_photos()
            #print 'got private photos'
        else:
            #print 'get public photos'
            photos = user.get_public_photos()
            #print 'got public photos'
#        print len(photos)
#        for photo in photos:
#            print photo

        photoAlbumContainer = Facade.selectContainerByTitle("Internet Photos")[0]
        photoAlbum = Facade.addPhotoAlbum(title="Flickr", container=photoAlbumContainer)
        #print 'go photos in photos'
        for photo in photos:
            #print photos
            #Get the date taken
            date_taken=photo.datetaken.split(' ')[0]
            #print date_taken
            photo_url=photo.getURL(urlType='source')
            #print photo_url
            photo_title=photo.title
            #print photo.title
            photo = Facade.addPhoto(
                            title=photo_title,
                            local_path=photo_url,
                            protocolInfo="http-get:*:image/jpeg:*",
                            mediaType='image/jpeg',
                            dateTaken=date_taken
                            )
            photoAlbum.addItem(photo)

    def unload(self):
        '''
        Load the plugin media information into the database.
        '''
        print self.name + ': Unload unimplemented'

    def getUsage(self):
        '''
        Return a usage message containing the structure of the
        args argument of the construct
        '''
        usage = 'username: the flickr user name'
        return usage

    def browser_terminal(self, url):
        print 'You will now will be redirected to the flickr website.'
        print "After garanted acess close the browser"
        command = '/usr/bin/browser --url '+'"'+url+'"'
        getoutput(command)
