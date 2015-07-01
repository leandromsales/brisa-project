#!/usr/bin/python
import os

testCmd = "./runTest.py --mediaServerPort=%d --outDir=%s --numberOfCp=%d --cpTimeRunning=%d --browserTime=%d --cpStartTime=%d" 

def runTest(cmd):
    print "\tRunning:", cmd
    os.system(cmd)
    print  
    print

outDir = "/tmp/benchmark"
runTest(testCmd % (66667, outDir, 1, 10, 1, 1))
runTest(testCmd % (66666, outDir, 2, 10, 1, 1))
runTest(testCmd % (66667, outDir, 3, 10, 1, 1))
runTest(testCmd % (66666, outDir, 3, 2, 2, 1))
runTest(testCmd % (66667, outDir, 3, 1, 2, 0))
