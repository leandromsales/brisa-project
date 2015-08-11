import logging
import thread
import os
import sys


def thr():

    logging.info(x)

r, w = os.pipe()

pid = os.fork()

if pid:
    print "eu sou o principal"
    os.close(w) # use os.close() to close a file descriptor
    r = os.fdopen(r) # turn r into a file object
    print "parent: reading"
    txt = r.read()
    os.waitpid(pid, 0) # make sure the child process gets cleaned up
else:
    print "eu sou o filho"
    # we are the child
    os.close(r)
    w = os.fdopen(w, 'w')
    print "child: writing"
    w.write("here's some text from the child")
    w.close()
    print "child: closing"
    sys.exit(0)

print "parent: got it; text =", txt