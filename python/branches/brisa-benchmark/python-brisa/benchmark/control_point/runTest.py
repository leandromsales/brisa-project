#!/usr/bin/python
'''
Created on Apr 15, 2009

@author: felipelc
'''
import os
import time
import shutil
import datetime
import control_point_benchmark
import memory_log_filter
import cp_graphs

def main(n, t, tr, i, servers, outDir):

    if (n == None) or (t == None) or (tr == None) or (i == None) or (servers == None) or (outDir == None):
        raise Exception, "Missing parameters."
    
    brisadir = os.path.expanduser('~') + "/.brisa/"
    outDir += "/" + str(datetime.datetime.now()) + "/"
    outDir = outDir.replace(" ", "_")
    if not os.path.isdir(outDir):
        os.mkdir(outDir)

    # Removing logs
    removePath(brisadir + "brisa.log")
    removePath(brisadir + "cp-memoryLog.txt")
    removePath(brisadir + "cp-log.txt")
    
    #Run cp
#    ADDR = "192.168.1.86"
#    mediaServerAddr = "http://%s:%d" % (ADDR, servers)
    control_point_benchmark.main(n, t, tr, i, servers)
    
    #Create cp memory log    
    time.sleep(2)
    mlog = memory_log_filter.main(brisadir + "cp-memoryLog.txt", "brisa")
    file = open(outDir + "cp-memoryLog.txt", "w")
    file.write(mlog)
    file.close()
    
    shutil.copy(brisadir + "cp-log.txt", outDir)
    
    #Create graphic
    cp_graphs.main(outDir + "cp-log.txt", outDir + "gnuplot-script", outDir + "cp", servers)
    
def removePath(file):
    try:
        os.remove(file)
    except:
        pass 
    