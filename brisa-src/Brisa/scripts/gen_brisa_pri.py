import os

file_content = """#This file was auto-generated. Don't edit this.

BASEDIR = {0}

INCLUDEPATH += $$BASEDIR/include
unix {{
    message("BRisa Unix config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/../build-brisa-Desktop_Qt_5_3_GCC_64bit-Debug
}}

device {{
    message("BRisa Device config.")
    QT += network xml
    LIBS += -lbrisa
    LIBS += -L$$BASEDIR/arm/so.le-v7-g
}}
""".format(str(os.getcwd()))

#print (file_content)
f = open(os.path.join(os.getcwd(), "brisa.pri"), "w")
f.write(file_content)
f.close()
