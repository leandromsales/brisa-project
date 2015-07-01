#!/usr/bin/python

def main(filename, module):
    
    if (filename == None) or (module == None):
        raise Exception, "Missing parameters."

    file = open(filename)

    line = file.readline()
    while line and line.find("Partition of a set of") == -1:
        line = file.readline()

    line = file.readline()
    outStr = line

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
            outStr += "\n" + line
            sizeCount += int(line[line.find("=")+1:line.find("bytes.")-1])
    
    outStr += "\nTotal size: " + str(sizeCount) + " bytes."
    
    return outStr