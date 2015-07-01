from media_services.cds.plugins.plugin import Plugin
from persistence import Facade
import flickr
import os
from commands import getoutput

class FlickrPlugin(Plugin):
    '''
    Plugin to acess an flickr account.
    '''
    name ='flickr'
    username=''
    private = 'no'

    def __init__(self, *args, **kwargs):
        '''
        Initialize the flickr plugin.
        Arguments:
        args - The configuration parameters to the loadded plugin.
        '''
        self.username=kwargs['username']
        self.private=kwargs['private']
        flickr.email=kwargs['username']
        flickr.CACHE_DIR=os.path.expanduser('~')+ '/.brisa/'
        flickr.API_KEY = 'f544dbc725689997313a6437e1068b5a'
        flickr.SECRET = '3281387735c48555'
        flickr.call_browser_method=self.browser_terminal

    def load(self):
        '''
        Load the plugin media information into the database.
        '''
        print "Loading Flickr Plugin"
        user=flickr.people_findByUsername(self.username)
#        print self.private
        if self.private=='yes':
            photos = user.get_photos()
        else:
            photos = user.get_public_photos()
#        print len(photos)
#        for photo in photos:
#            print photo

        photoAlbumContainer = Facade.selectContainerByTitle("Internet Photos")[0]
        photoAlbum = Facade.addPhotoAlbum(title="Flickr", container=photoAlbumContainer)
        for photo in photos:
            #Get the date taken
            date_taken=photo.datetaken.split(' ')[0]
            #print date_taken
            photo_url=photo.getURL(urlType='source')
            #print photo_url
            photo_title=photo.title
            #print photo.title
            photo = Facade.addPhoto(
                            title=photo_title,
                            path=photo_url,
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
