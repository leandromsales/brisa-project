# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from media_services.cds.plugins.plugin import Plugin
from persistence import Facade
import brisa_flickr
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
        brisa_flickr.username=brisa_config.get_parameter('flickr', 'username')
        brisa_flickr.CACHE_DIR=os.path.expanduser('~')+ '/.brisa/'
        brisa_flickr.API_KEY = 'f544dbc725689997313a6437e1068b5a'
        brisa_flickr.SECRET = '3281387735c48555'
        try:
            import hildon
            brisa_flickr.call_browser_method=self.browser_terminal
        except:
            logging.debug('FlickrPlugin>>Using firefox browser')

    def load(self):
        '''
        Load the plugin media information into the database.
        '''
        logging.info("Loading Flickr Plugin")
        isprivate = self.private=='yes'
        photos=brisa_flickr.get_user_photos(self.username,
                                            isprivate)

        photoAlbumContainer=Facade.selectContainerByTitle("Internet Photos")[0]
        photoAlbum=Facade.addPhotoAlbum(title="Flickr",
                                        container=photoAlbumContainer)
        #print 'go photos in photos'
        for photo in photos:
            #print photos
            photo_url=photo['url']
            #print photo_url
            photo_title=photo['title']
            #print photo.title
            photo = Facade.addPhoto(
                            title=photo_title,
                            local_path=photo_url,
                            protocolInfo="http-get:*:image/jpeg:*",
                            mediaType='image/jpeg',
                            dateTaken="1900-01-01")
            photoAlbum.addItem(photo)

    def browser_maemo(self, url):
        print 'You will now will be redirected to the flickr website.'
        print "After garanted acess close the browser"
        command = '/usr/bin/browser --url '+'"'+url+'"'
        getoutput(command)