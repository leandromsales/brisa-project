# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
#
# Copyright 2006 John-Mark Gurney <gurney_j@resnet.uoregon.edu>
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

from twisted.internet import reactor

# Make sure to update the global line when adding a new function
__all__ = ['appendnamespace', 'insertnamespace', 'insertringbuf']

DEFAULTRINGSIZE = 50
appendnamespace = lambda k, v: []
insertnamespace = lambda k, v: None
insertringbuf = lambda k, l = DEFAULTRINGSIZE: None

class RingBuffer(object):

    def __init__(self, size_max):
        self.max = size_max
        self.data = []

    def append(self, x):
        """append an element at the end of the buffer"""
        self.data.append(x)
        if len(self.data) == self.max:
            self.cur=0
            self.__class__ = RingBufferFull

    def get(self):
        """return a list of elements from the oldest to the newest"""
        return self.data


class RingBufferFull(object):

    def __init__(self, n):
        raise "you should use RingBuffer"

    def append(self, x):
        self.data[self.cur]=x
        self.cur=(self.cur+1) % self.max

    def get(self):
        return self.data[self.cur:] + self.data[: self.cur]


def doDebugging(opt):
    if not opt:
        return

    global insertnamespace, appendnamespace, insertringbuf

    def insertnamespace(k, v):
        assert isinstance(k, basestring)
        sf.namespace[k] = v

    def appendnamespace(k, v):
        try:
            sf.namespace[k].append(v)
        except KeyError:
            sf.namespace[k] = [v]

    def insertringbuf(k, l = DEFAULTRINGSIZE):
        insertnamespace(k, RingBuffer(l))

    from twisted.manhole import telnet


    class Debug(telnet.Shell):

        def welcomeMessage(self):
            data = ['', 'Brisa Debugging Console', '', '']
            return '\r\n'.join(data)

    sf = telnet.ShellFactory()
    sf.protocol = Debug
    reactor.listenTCP(56283, sf)