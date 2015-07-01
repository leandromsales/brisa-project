from twisted.web import server
from twisted.web.resource import Resource
from twisted.internet import reactor
from twisted.web import resource, static

import os

class Other(Resource):
    def __init__(self):
       Resource.__init__(self)
       local_path='test file.txt'
       file=local_path
       self.putChild(os.path.basename(file).replace(" ", "_"), static.File(file))
       print self.listEntities()

    def getChildWithDefault(self, path, request):
        print 'getChildWithDefault'
        print (self, path, request)
        local_path='test file.txt'
        file=local_path
        return static.File(file)

if __name__ == "__main__":
    content = Other()
    site = server.Site(content)
    reactor.listenTCP(8080, site)
    print 'running ...'
    reactor.run()
