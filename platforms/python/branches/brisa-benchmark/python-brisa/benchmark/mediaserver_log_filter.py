#!/usr/bin/python
import sys
import getopt
import os

homedir = os.path.expanduser('~')

def findFileBrowser(file):
    line = file.readline()
    while line and line.find("BENCHMARK Browser BEG:") == -1:
        line = file.readline()
    beg = line

    while line and line.find("BENCHMARK Browser CPID:") == -1:
        line = file.readline()
    cpId = line
    
    while line and line.find("BENCHMARK Browser END:") == -1:
        line = file.readline()
    end = line
    
    while line and line.find("BENCHMARK Browser DIFF:") == -1:
        line = file.readline()
    diff = line    
    
    return (beg, end, diff, cpId)

class Log:
    
    def __init__(self, fileName):
        self.lines = []
        self.file = open(fileName, "w")        

    def insert_row(self, cpUuid, begTime, endTime, diffTime):
        self.lines.append((cpUuid, begTime, endTime, diffTime))
        
    def create_log(self):
        line = "#UUID" + "\tBegTime" + "\tEndTime" + "\tDiffTime(Sec)" + "\n"
        self.file.write(line)
        
        for cpUuid, begTime, endTime, diffTime in self.lines:
            line = str(cpUuid)[1:-1] + "\t" +     \
                str(begTime) + "\t" + str(endTime) + "\t" + str(diffTime) + "\n"
            self.file.write(line)

        self.file.close()


keywords = ['filename=']

try:
    opts, extraparams = getopt.getopt(sys.argv[1:], "", keywords)
except getopt.GetoptError, err:
    print str(err)
    sys.exit(2) 

filename = None

for o,p in opts:
    if o in ['--filename']:
        filename = p

if (filename == None):
    print "Missing parameters."
    print "Example:", sys.argv[0], "--filename=brisa.log"
    sys.exit(2)


file = open(filename)
msLog = Log(homedir + "/.brisa/media-server-log.txt")

begLine, endLine, diffLine, cpId = findFileBrowser(file)
while begLine and endLine and diffLine:
    begLine = str(begLine).split(" ")
#    begTime = begLine[-2].strip() + " "+ begLine[-1].strip()
    begTime = begLine[-1].strip()
    endLine = str(endLine).split()
#    endTime = endLine[-2].strip() + " " + endLine[-1].strip()
    endTime = endLine[-1].strip()
    diffLine = str(diffLine).split()
    diffTime = diffLine[-1].strip()
    cpId = str(cpId).split()
    cpId = cpId[-1].strip()                                            
    msLog.insert_row(cpId, begTime, endTime, diffTime)
    begLine, endLine, diffLine, cpId = findFileBrowser(file)

msLog.create_log()

print "\nFinish."