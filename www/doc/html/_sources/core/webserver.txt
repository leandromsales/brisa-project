:mod:`webserver` --- Webserver facilities
==========================================

.. module: webserver
    :synopsis: Wrapping module of cherrypy

:ref:`python-brisa <python-brisa>` provides a WSGI application-side to
which any WSGI server can be plugged in.

Along with the WSGI app-side, we provide three adapters for known python
webservers:

    * **CherrypyAdapter** - `Cherrypy <http://www.cherrypy.org>`_
    * **CircuitsWebAdapter** - `Circuits.web <http://trac.softcircuit.com.au/circuits>`_
    * **PasteAdapter** - `Python-paste <http://pythonpaste.org>`_

In the simplest case, all you need is to have **at least one** of them
installed, and use the webserver without any worry about WSGI, adapters or
anything else. See the illustrative code below and the example of the next
section::

    (...)
    from brisa.core.webserver import WebServer

    webserver = WebServer('MyServerName', 'localhost', 1234)

    # Add files, resources
    (...)

    webserver.start()


If you have more than one installed and you want to explicitly use one of them,
there are two ways:

    1. Set a configuration entry::

        $ brisa-conf -s brisa -p webserver_adapter X

       where X is the adapter name (``cherrypy``, ``circuits`` or ``circuits.web``).

    2. Choose during runtime::

        from brisa.core.webserver import WebServer, CircuitsWebAdapter

        webserver = WebServer(adapter=CircuitsWebAdapter)


For performance reasons we recommend the CircuitsWeb adapter.


Creating a simple webserver
----------------------------
This example shows the webserver usage, along with files and resources. You can find this source :download:`here <../example_code/webserver.py>`.


In a first moment, we setup the reactor, the webserver and add a file to the
webserver on the relative path ``/hello_world``::

    from brisa.core.reactors import install_default_reactor
    reactor = install_default_reactor()

    import os

    from brisa.core.webserver import WebServer, StaticFile, CustomResource

    # Setup webserver
    webserver = WebServer()

    # Write and serve a sample file
    f = open('/tmp/hello_world', 'w')
    f.write('Hello World!')
    f.close()

    webserver.add_static_file('hello_world', StaticFile('/tmp/hello_world'))


The resource class can be used for more complex requests. We create here a
Greeter which will greet someone given its name. It is added to the webserver 
on the relative path ``/Greet``::

    # Serving a resource
    class Greeter(CustomResource):

        def get_render(self, uri, params):
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

In the final step, we start the webserver and print the test URLs that can be
used to verify the functionality::

    # Starting the webserver
    webserver.start()

    print 'Webserver listening on', webserver.get_listen_url()
    print 'File URL: %s/hello_world' % webserver.get_listen_url()
    print 'Res URL: %s/Greet' % webserver.get_listen_url()
    print 'Res test URL: %s/Greet?name=you' % webserver.get_listen_url()

    # Block so that the program doesn't quit

    reactor.add_after_stop_func(webserver.stop)
    reactor.main()

    os.remove('/tmp/hello_world')
