#!/usr/bin/python
import sys
import getopt

keywords = ['filename=', 'module=']

try:
    opts, extraparams = getopt.getopt(sys.argv[1:], "", keywords)
except getopt.GetoptError, err:
    print str(err)
    sys.exit(2) 

filename = None
module = None

for o,p in opts:
    if o in ['--filename']:
        filename = p
    elif o in ['--module']:
        module = p

if (filename == None) or (module == None):
    print "Missing parameters."
    print "Example:", sys.argv[0], "--filename=memoryLog.txt --module=brisa.services"
    sys.exit(2)


file = open(filename)

line = file.readline()
while line and line.find("Partition of a set of") == -1:
    line = file.readline()

line = file.readline()
print line

sizeCount = 0
while True:
    lastLine = line
    line = file.readline()
    if not line:
        break
    if line.find(module) != -1:
        if line.find("     0") == -1:
            line = lastLine[:-1] + " " + line.strip()
        if line.find("=") == -1:
            nextLine = file.readline()
            line += " " + nextLine.strip()
        print line
        sizeCount += int(line[line.find("=")+1:line.find("bytes.")-1])

print "\nTotal size:", str(sizeCount), "bytes."