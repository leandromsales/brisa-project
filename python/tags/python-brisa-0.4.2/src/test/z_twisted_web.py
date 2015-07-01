from twisted.web import server
from twisted.web.resource import Resource
from twisted.internet import reactor

class Hello(Resource):

    def getChild(self, name, request):

        if name == '':
            return self
        return Resource.getChild(
            self, name, request)

    def render_GET(self, request):
            return """<html>
      Hello, world! I am located at %r.
    </html>""" % (request.prepath)

class Simple(Resource):

    isLeaf = True

    def render_GET(self, request):

        return "<html>Hello, world! %r</html>" % request.path

class Other(Resourse):

    def __init__(self):

        local_path='test_file.txt'
        cself.putChild(path, static.File(local_path))

if __name__ == "__main__":
    site = server.Site(Other())
    reactor.listenTCP(8080, site)
    reactor.run()