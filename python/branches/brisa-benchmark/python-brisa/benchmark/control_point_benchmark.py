#!/usr/bin/python
'''
Created on Apr 7, 2009

@author: felipelc
'''

import sys
import getopt
import time
import random
import os
import guppy
from datetime import datetime
import time
from brisa.threading import ThreadObject
from brisa.control_point.control_point_av import ControlPointAV
from brisa.threading import thread_manager

homedir = os.path.expanduser('~')

class Benchmark:

    def __init__(self, mediaServerAddr, numberOfCP, 
                 timeRunning, waitTimeBetweenBrowsers, waitTimeBetweenCP):

        if numberOfCP <= 0:
            raise Exception, "Invalid number of control points."
        
        if timeRunning <= 0:
            raise Exception, "Invalid time running."

        if waitTimeBetweenBrowsers < 0:
            raise Exception, "Invalid browser gap."

        if waitTimeBetweenCP < 0:
            raise Exception, "Invalid control point gap."
        
        if timeRunning < ((numberOfCP - 1) * waitTimeBetweenCP):
            raise Exception, "Small time running."        
        
        self.control_points = []
        memoryLog = MemoryLog(homedir + "/.brisa/cp-memoryLog.txt")
        
        try:
            cpPoll = CPPoll(mediaServerAddr, numberOfCP, timeRunning, waitTimeBetweenBrowsers, waitTimeBetweenCP)
        except Exception, e:
            thread_manager.main_loop_quit()
            raise e
        
        print "Starting clock: %d seconds" % (timeRunning)
        cpPoll.start()
        time.sleep(timeRunning)
        cpPoll.stop()
        
        thread_manager.main_loop_quit()
        print "Creating log."
        memoryLog.create_log()

class CPPoll(ThreadObject):
    
    def __init__(self, mediaServerAddr, numberOfThreads, timeRunning, waitTimeBetweenBrowsers, waitTimeBetweenCP):
        ThreadObject.__init__(self)
        self.control_points = []
        self.waitTimeBetweenCP = waitTimeBetweenCP
        
        cp = ControlPointBenchmark(0, timeRunning, waitTimeBetweenBrowsers, waitTimeBetweenCP)
        thread_manager.main_loop_async()    
        if not cp.try_set_server(mediaServerAddr):
            raise Exception, "Couldn't find the media server."
        self.control_points.append(cp)

        for i in range(1, numberOfThreads):
            cp = ControlPointBenchmark(i, timeRunning, waitTimeBetweenBrowsers, waitTimeBetweenCP)
            if not cp.try_set_server(mediaServerAddr):
                raise Exception, "Couldn't find the media server."
            self.control_points.append(cp)
    
    def run(self):
        for i in range(0, len(self.control_points) - 1):
            cp = self.control_points[i]
            cp.start_thread()
            time.sleep(self.waitTimeBetweenCP)
        cp = self.control_points[len(self.control_points)-1]
        cp.start_thread()
        
        while self.is_running():
            pass
        
        for cp in self.control_points:
            cp.stop_thread()

class ControlPointBenchmark(ThreadObject):
    
    def __init__(self, cpNumber, timeRunning, waitTimeBetweenBrowsers, waitTimeBetweenCP):
        '''
        Constructor
        '''
        ThreadObject.__init__(self)
        self.cp = ControlPointAV()
        self._initial_subscribes()
        self.devices_found = []
        self._search()
        
        self.cpNumber = cpNumber
        self.timeRunning = timeRunning
        self.waitTimeBetweenCP = waitTimeBetweenCP
        self.waitTimeBetweenBrowsers = waitTimeBetweenBrowsers
        
    def _initial_subscribes(self):
        self.cp.subscribe('new_device_event', self.on_new_device)
        self.cp.subscribe('remove_device_event', self.on_remove_device)

    def on_new_device(self, dev):
        self.devices_found.append(dev)

    def on_remove_device(self, udn):
        for dev in self.devices:
            if dev.udn == udn:
                self.devices_found.remove(dev)
                break
            
    def _search(self):
        self.cp.start_search(600, 'upnp:rootdevice')
        
    def try_set_server(self, listenAddress):
        max_try = 5
        while max_try > 0:
            max_try -= 1
            time.sleep(2)
            for dev in self.devices_found:
                if dev.location.find(listenAddress) != -1:
                    self.cp.current_server = dev
                    return True
            
        return False
    
    def _browse(self, id):
        try:
            beg = time.time()
#            beg = datetime.now()
            result = self.cp.browse(id, 'BrowseDirectChildren', '*', 0, 10, cpId=self.cpNumber)['Result']
        except Exception, e:
            msg = e.message
            end = time.time()
#            end = datetime.now()
            log.insert_row(n=self.cpNumber, t=self.timeRunning, 
                           tr=self.waitTimeBetweenBrowsers, i=0, 
                           begTime=beg, endTime=end, error=msg)
            return []            

#        end = datetime.now()
        end = time.time()
        log.insert_row(n=self.cpNumber, t=self.timeRunning, 
                           tr=self.waitTimeBetweenBrowsers, i=self.waitTimeBetweenCP,
                           begTime=beg, endTime=end, error="")            
        
        ids = []
        for d in result:
            ids.append(d.id)
            if str(d.upnp_class).find("object.item") != -1:
                return []

        return ids


    def run(self):
        
        while self.is_running():
            ids = self._browse(0)
            time.sleep(self.waitTimeBetweenBrowsers)
        
            while True:
                if not self.is_running():
                    break
                if len(ids) == 0:
                    break
                random_index = random.randint(0, len(ids) -1)
                ids = self._browse(ids[random_index])
                time.sleep(self.waitTimeBetweenBrowsers)

    def start_thread(self):
        print "Start browsing at thread %d" % (self.cpNumber)
        ThreadObject.start(self)
        
    def stop_thread(self):
        print "Stop browsing at thread %d" % (self.cpNumber)
        ThreadObject.stop(self)
        

class MemoryLog:
    
    def __init__(self, fileName):
        self.hp = guppy.hpy()
        self.file = open(fileName, "w")
        self.hp.setrelheap()

    def create_log(self):
        h = self.hp.heap()
        for i in range(0,len(h)):
            self.file.write(str(h[i]))

        self.file.close()
        
class Log:
    
    def __init__(self, fileName):
        self.lines = []
        self.file = open(fileName, "w")        

    def insert_row(self, n, t, tr, i, begTime, endTime, error):
        time_delta = endTime - begTime
#        microseconds = time_delta.microseconds
        microseconds = time_delta
        self.lines.append((n, t, tr, i, begTime, endTime, microseconds, error))
        
    def create_log(self):
        line = "#N" + "\tT" + "\tTR" + "\tI" + "\tReqNumber" + "\tBegTime" + "\tEndTime" + "\tDiffTime(Sec)" + "\tError" + "\n"
        self.file.write(line)
        
        self.lines.sort(self._sort_lines)
        
        index = 0
        count = 0
        for n, t, tr, i, begTime, endTime, difTime, errorMsg in self.lines:
            if n == index:
                count += 1
            else:
                index = n
                count = 1
            line = str(n) + "\t" + str(t) + "\t" + str(tr) +    \
                "\t" + str(i) + "\t" + str(count) + "\t" +     \
                str(begTime) + "\t" + str(endTime) + "\t" + str(difTime) + "\t" + str(errorMsg) + "\n"
            self.file.write(line)
                
        self.file.close()
        
    def _sort_lines(self, x, y):
        nX, tX, trX, iX, begTimeX, endTimeX, difTimeX, errorMsgX = x
        nY, tY, trY, iY, begTimeY, endTimeY, difTimeY, errorMsgY = y
        
        if nX > nY:
            return 1
        elif nX == nY:
            if begTimeX > begTimeY:
                return 1
            else:
                return -1
        else:
            return -1 
        
        
### Script

keywords = ['mediaServerAddr=', 'numberOfCp=', 'cpTimeRunning=', 
            'browserTime=', 'cpStartTime=' ] # the = means that a value is expected after the keyword

try:
    opts, extraparams = getopt.getopt(sys.argv[1:], "", keywords)
except getopt.GetoptError, err:
    print str(err) # will print something like "option -a not recognized"
    sys.exit(2) 

# Now we simply have intialized or values and loop through the options passed by the user to see if we need to modify them
n = None
t = None
tr = None
i = None
mediaServerAddr = None

for o,p in opts:
    if o in ['--numberOfCp']:
        n = int(p)
    elif o in ['--cpTimeRunning']:
        t = int(p)
    elif o in ['--browserTime']:
        tr = int(p)
    elif o in ['--cpStartTime']:
        i = int(p)
    elif o in ['--mediaServerAddr']:
        mediaServerAddr = p

if (n == None) or (t == None) or (tr == None) or (i == None) or (mediaServerAddr == None):
    print "Missing parameters."
    print "Example:", sys.argv[0], "--mediaServerAddr=http://192.168.1.86:39051 --numberOfCp=1 --cpTimeRunning=10 --browserTime=1 --cpStartTime=0"
    sys.exit(2)


try:
    log = Log(homedir + "/.brisa/cp-log.txt")
    benchmark = Benchmark(mediaServerAddr, n, t, tr, i)
    log.create_log()
except Exception, err:
    print err.message
    sys.exit(2)
    
print "\tExiting..."
