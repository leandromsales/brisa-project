# Creating the applicationinfo.hpp file
info.target = applicationinfo.hpp
info.commands = "python  ../build/build_counter.py"
QMAKE_EXTRA_TARGETS += info
PRE_TARGETDEPS += applicationinfo.hpp
