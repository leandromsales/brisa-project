#!/usr/bin/python
'''
Generate gnuplot scripts

@author: felipelc
'''
import os

def getBrowserNumbers(logFile):
    file = open(logFile)

    line = file.readline()
    cpNum = -1
    browserNumbers = []
    while line:
        if line.find("#") == -1:
            tokens = line.split("\t")
            if tokens[4] == "1":
                browserNumbers.append(1)
                cpNum += 1
            else:
                browserNumbers[cpNum] += 1
        line = file.readline()

    return browserNumbers

def gnuplotScript(logFile, browserNum):
    script = "set encoding iso_8859_1\n"
    script += "set terminal svg size 800,500\n"
    script += "set output 'cp-requestNumber-X-AnswerTime.svg'\n"

    script += "set xlabel 'Requisicao'\n"
    script += "set ylabel 'Tempo de resposta'\n"
    script += "set grid\n"
    script += "set datafile commentschars '#'\n"
    script += "set datafile separator '\\t'\n"

    script += "plot "
    
    line = 0
    numberOfCp = len(browserNum)
    for cp in range(numberOfCp - 1):
        reqNumber = browserNum[cp]
        script += "'%s' every ::%d::%d using 6:8 title '%s' with linespoints, " % (logFile, line, line + reqNumber - 1, "Cp " + str(cp))
        line += reqNumber
    reqNumber = browserNum[numberOfCp - 1]
    script += "'%s' every ::%d::%d using 6:8 title '%s' with linespoints" % (logFile, line, line + reqNumber - 1, "Cp " + str(numberOfCp - 1))
        

    return script

def main():
    logFile = "cp-log.txt"
    scriptName = "cp-gnuplot-script.tmp"
    
    browserNum = getBrowserNumbers(logFile)
    script = gnuplotScript(logFile, browserNum)

    outFile = open(scriptName, "w")
    outFile.write(script)
    outFile.close()
    
    os.system("gnuplot " + scriptName)
    
main()    
