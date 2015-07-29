import os
import cherrypy

from brisa.services.web_server import WebServer, StaticFile, CustomResource
from brisa.threading import thread_manager


# Setup webserver
webserver = WebServer()


# Write and serve a sample file
f = open('/tmp/hello_world', 'w')
f.write('Hello World!')
f.close()

webserver.add_static_file('hello_world', StaticFile('/tmp/hello_world'))


# Serve a resource


class Greeter(CustomResource):

    def getChildWithDefault(self, uri, params):
        return self

    def say_hello(self, name):
        return 'Hello %s!' % name

    def render(self, uri, request, response):
        params = cherrypy.request.params

        if 'name' in params:
            # http://addr:port/Greet?name=Someone
            return self.say_hello(params['name'])
        else:
            return 'Hello!'

webserver.add_resource('Greet', Greeter())


# Start the webserver
webserver.start()

print 'Webserver listening on', webserver.listen_url
print 'File URL: %s/hello_world' % (webserver.listen_url)
print 'Res URL: %s/Greet' % (webserver.listen_url)


# Block so that the program doesn't quit
thread_manager.main_loop()

os.remove('/tmp/hello_world')
