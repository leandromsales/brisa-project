#!/usr/bin/python
'''
Created on Apr 15, 2009

@author: felipelc
'''
import os
from subprocess import Popen
import sys
import time
import getopt
import shutil
import datetime
import signal
from brisa.threading import ThreadObject

keywords = ['mediaServerPort=', 'outDir=', 'numberOfCp=', 'cpTimeRunning=', 
            'browserTime=', 'cpStartTime=' ] 

try:
    opts, extraparams = getopt.getopt(sys.argv[1:], "", keywords)
except getopt.GetoptError, err:
    print str(err) 
    sys.exit(2) 

# Now we simply have intialized or values and loop through the options passed by the user to see if we need to modify them
n = None
t = None
tr = None
i = None
mediaServerPort = None
outDir = None

for o,p in opts:
    if o in ['--numberOfCp']:
        n = int(p)
    elif o in ['--cpTimeRunning']:
        t = int(p)
    elif o in ['--browserTime']:
        tr = int(p)
    elif o in ['--cpStartTime']:
        i = int(p)
    elif o in ['--mediaServerPort']:
        mediaServerPort = int(p)
    elif o in ['--outDir']:
        outDir = p

if (n == None) or (t == None) or (tr == None) or (i == None) or (mediaServerPort == None) or (outDir == None):
    print "Missing parameters."
    print "Example:", sys.argv[0], "--mediaServerPort=39051 --outDir=/tmp --numberOfCp=1 --cpTimeRunning=10 --browserTime=1 --cpStartTime=0"
    sys.exit(2)


brisadir = os.path.expanduser('~') + "/.brisa/"
outDir += "/" + str(datetime.datetime.now()) + "/"
if not os.path.isdir(outDir):
    os.mkdir(outDir)

def removePath(file):
    try:
        os.remove(file)
    except:
        pass 
    
# Removing logs
removePath(brisadir + "brisa.log")
removePath(brisadir + "cp-memoryLog.txt")
removePath(brisadir + "cp-log.txt")
removePath(brisadir + "media-server-memoryLog.txt")
removePath(brisadir + "media-server-log.txt")

class MediaServer(ThreadObject):
    
    def __init__(self, addr, port):
        ThreadObject.__init__(self)
        self.addr = addr
        self.port = port
        
    def run(self):
        cmd = 'brisa-media-server -i %s -p %d &' % (self.addr, self.port)
        print "Running:", cmd
        p = Popen(cmd, shell=True)
        sts = os.waitpid(p.pid, 0)
        while self.is_running():
            pass
        print "Stoping media server."
        os.kill(p.pid + 1, signal.SIGINT)
        
    def stop(self):
        ThreadObject.stop(self)
        
ADDR = "192.168.1.86"

ms = MediaServer(ADDR, mediaServerPort)
ms.start()

time.sleep(3)
cpCmd = "./control_point_benchmark.py --mediaServerAddr=http://%s:%d --numberOfCp=%d --cpTimeRunning=%d --browserTime=%d --cpStartTime=%d" % (ADDR, mediaServerPort, n, t, tr, i)
print "Running:", cpCmd
os.system(cpCmd)
ms.stop()

time.sleep(2)
cmd = "./memory_log_filter.py --filename=%s --module=%s > %s" % (brisadir + "cp-memoryLog.txt",
                                                                 "brisa",
                                                                 "/tmp/cp-memoryLog.txt")
print "Running:", cmd
os.system(cmd)
shutil.copy("/tmp/cp-memoryLog.txt", outDir)

cmd = "./memory_log_filter.py --filename=%s --module=%s > %s" % (brisadir + "media-server-memoryLog.txt",
                                                                 "brisa",
                                                                 "/tmp/media-server-memoryLog.txt")
print "Running:", cmd
os.system(cmd)
shutil.copy("/tmp/media-server-memoryLog.txt", outDir)

cmd = "./mediaserver_log_filter.py --filename=%s" % (brisadir + "brisa.log")
print "Running:", cmd
os.system(cmd)
shutil.copy(brisadir + "media-server-log.txt", outDir)

shutil.copy(brisadir + "cp-log.txt", outDir)

shutil.copy(brisadir + "brisa.log", outDir)

sys.exit()