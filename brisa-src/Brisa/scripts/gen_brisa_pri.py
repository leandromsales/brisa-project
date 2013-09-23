import os

file_content = """#This file was auto-generated. Don't edit this.
BASEDIR = {0}  
device {{
    QT += network xml
    INCLUDEPATH += $$BASEDIR/include
    LIBS += -lBrisa
    LIBS +=  -L$$BASEDIR/arm/so.le-v7-g    
}}\n""".format(str(os.getcwd()))

#print (file_content)
f = open(os.path.join(os.getcwd(), "brisa.pri"), "w")
f.write(file_content)
f.close()