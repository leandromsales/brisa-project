#!/usr/bin/python
import os
import runTest

outDir = "/tmp/benchmark"
servers = [("brisa", "http://192.168.1.86:12742"),
           ("gupnp", "http://192.168.1.86:29199")]
#runTest.main(1, 10, 1, 1, servers, outDir)
runTest.main(2, 10, 3, 2, servers, outDir)
#runTest.main(3, 10, 1, 1, servers, outDir)
#runTest.main(3, 2, 2, 1, servers, outDir)
#runTest.main(3, 1, 2, 0, servers, outDir)
