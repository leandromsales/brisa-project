#!/usr/bin/python
'''
Generate gnuplot scripts

@author: felipelc
'''
import os

def getBrowserNumbers(msAddr, logFile):
    file = open(logFile)

    #Skip comments
    line = file.readline()    
    line = file.readline()
    
    # Find the ms
    while line and line.find(msAddr) == -1:
        line = file.readline()
        
    cpNum = -1
    browserNumbers = []
    while line and line.find(msAddr) != -1:
        tokens = line.split("\t")
        if tokens[5] == "1":
            browserNumbers.append(1)
            cpNum += 1
        else:
            browserNumbers[cpNum] += 1
        line = file.readline()

    return browserNumbers

def gnuplotScript(logFile, browserNum, imageName, begLine):
    script = "set encoding iso_8859_1\n"
    script += "set terminal svg size 800,500\n"
    script += "set output '%s.svg'\n" % (imageName)

    script += "set xlabel 'Requisicao'\n"
    script += "set ylabel 'Tempo de resposta'\n"
    script += "set grid\n"
    script += "set datafile commentschars '#'\n"
    script += "set datafile separator '\\t'\n"

    script += "plot "
    
    line = begLine
    numberOfCp = len(browserNum)
    for cp in range(numberOfCp - 1):
        reqNumber = browserNum[cp]
        script += "'%s' every ::%d::%d using 7:9 title '%s' with linespoints, " % (logFile, line, line + reqNumber - 1, "Cp " + str(cp))
        line += reqNumber
    reqNumber = browserNum[numberOfCp - 1]
    script += "'%s' every ::%d::%d using 7:9 title '%s' with linespoints" % (logFile, line, line + reqNumber - 1, "Cp " + str(numberOfCp - 1))
        

    return script

def main(logFile, scriptName, imageName, servers):
    
    numberOfLogLines = 0
    for msName, msAddr in servers:
        browserNum = getBrowserNumbers(msAddr, logFile)
        script = gnuplotScript(logFile, browserNum, imageName + "-X-ms-" + msName, numberOfLogLines)
    
        newScriptName = scriptName + "-" + msName + ".plt"
        outFile = open(newScriptName, "w")
        outFile.write(script)
        outFile.close()
        
        os.system("gnuplot " + newScriptName)
        
        for n in browserNum:
            numberOfLogLines += n