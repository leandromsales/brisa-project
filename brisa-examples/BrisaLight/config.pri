# Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR =  $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        INCLUDEPATH +=  $$quote(/home/wendell/git/brisa-project/brisa-src/Brisa/include)

        DEPENDPATH +=  $$quote(/home/wendell/git/brisa-project/brisa-src/Brisa/include)

        LIBS += -lBrisa

        LIBS +=  $$quote(-L/home/wendell/git/brisa-project/brisa-src/Brisa/arm/so.le-v7-g)

        PRE_TARGETDEPS +=  $$quote(/home/wendell/git/brisa-project/brisa-src/Brisa/arm/so.le-v7-g)

        SOURCES +=  $$quote($$BASEDIR/src/BinaryLight.cpp) \
                 $$quote($$BASEDIR/src/SwitchPower.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/BinaryLight.h) \
                 $$quote($$BASEDIR/src/SwitchPower.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp)
    }

    CONFIG(release, debug|release) {
        INCLUDEPATH +=  $$quote(/home/wendell/git/brisa-project/brisa-src/Brisa/include)

        DEPENDPATH +=  $$quote(/home/wendell/git/brisa-project/brisa-src/Brisa/include)

        LIBS += -lBrisa

        LIBS +=  $$quote(-L/home/wendell/git/brisa-project/brisa-src/Brisa/arm/so.le-v7-g)

        PRE_TARGETDEPS +=  $$quote(/home/wendell/git/brisa-project/brisa-src/Brisa/arm/so.le-v7-g)

        SOURCES +=  $$quote($$BASEDIR/src/BinaryLight.cpp) \
                 $$quote($$BASEDIR/src/SwitchPower.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/BinaryLight.h) \
                 $$quote($$BASEDIR/src/SwitchPower.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp)
    }
}

simulator {
    CONFIG(debug, debug|release) {
        INCLUDEPATH +=  $$quote(/home/wendell/git/brisa-project/brisa-src/Brisa/include)

        DEPENDPATH +=  $$quote(/home/wendell/git/brisa-project/brisa-src/Brisa/include)

        LIBS += -lBrisa

        SOURCES +=  $$quote($$BASEDIR/src/BinaryLight.cpp) \
                 $$quote($$BASEDIR/src/SwitchPower.cpp) \
                 $$quote($$BASEDIR/src/applicationui.cpp) \
                 $$quote($$BASEDIR/src/main.cpp)

        HEADERS +=  $$quote($$BASEDIR/src/BinaryLight.h) \
                 $$quote($$BASEDIR/src/SwitchPower.hpp) \
                 $$quote($$BASEDIR/src/applicationui.hpp)
    }
}

INCLUDEPATH +=  $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER =  $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES +=  $$quote($$BASEDIR/../src/*.c) \
             $$quote($$BASEDIR/../src/*.c++) \
             $$quote($$BASEDIR/../src/*.cc) \
             $$quote($$BASEDIR/../src/*.cpp) \
             $$quote($$BASEDIR/../src/*.cxx) \
             $$quote($$BASEDIR/../assets/*.qml) \
             $$quote($$BASEDIR/../assets/*.js) \
             $$quote($$BASEDIR/../assets/*.qs)

    HEADERS +=  $$quote($$BASEDIR/../src/*.h) \
             $$quote($$BASEDIR/../src/*.h++) \
             $$quote($$BASEDIR/../src/*.hh) \
             $$quote($$BASEDIR/../src/*.hpp) \
             $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS =  $$quote($${TARGET}.ts)
