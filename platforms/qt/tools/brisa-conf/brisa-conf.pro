TEMPLATE = app
CONFIG += QT BRISA

QMAKE_CLEAN += file.dat
BRISA += core
QT += network core
QXT += core web

# Input
SOURCES += brisa-conf.cpp

OTHER_FILES +=

HEADERS += \
    tclap/XorHandler.h \
    tclap/Visitor.h \
    tclap/VersionVisitor.h \
    tclap/ValuesConstraint.h \
    tclap/ValueArg.h \
    tclap/UnlabeledValueArg.h \
    tclap/UnlabeledMultiArg.h \
    tclap/SwitchArg.h \
    tclap/StdOutput.h \
    tclap/OptionalUnlabeledTracker.h \
    tclap/MultiSwitchArg.h \
    tclap/MultiArg.h \
    tclap/IgnoreRestVisitor.h \
    tclap/HelpVisitor.h \
    tclap/DocBookOutput.h \
    tclap/Constraint.h \
    tclap/CmdLineOutput.h \
    tclap/CmdLineInterface.h \
    tclap/CmdLine.h \
    tclap/ArgException.h \
    tclap/Arg.h
