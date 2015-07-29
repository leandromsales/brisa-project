# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import commands

if __name__ == "__main__":
    #outtext = commands.getoutput("cd ../../ && python test/pep8/pep8.py\
#-v --show-source \
    #    --show-pep8 *.py")
    print "Running pep8 test, please wait...",
    outtext = commands.getoutput("find ../../ -name '*.py' -type f -exec \
    python pep8.py -v --statistics --show-source --show-pep8 {} \;")

    print outtext
    #filename = "/tmp/test-pep8.txt"
    #FILE = open(filename,"w")
    #FILE.write(outtext)
    #FILE.close() # this is icin

    #print "done!"
    #print "Pep8 test goes to /tmp/test-pep8.txt"

    #find . -name '*.py' -type f -exec dos2unix -v '{}' \;
