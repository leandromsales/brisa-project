# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>
import exceptions

import os
import string
import filesystem

class Remove(object):

    def whitespace_remove(self, dir_name):
        '''
        Remove right whitespaces from all files into
        directory. This necesssary because PEP8 stile
        code.
        '''
        print 'Removing right whitespace of files...'
        print 'Please wait'

        file_list = filesystem.list_directory(dir_name)
        files = []
        for file in file_list:
            (root, ext) = os.path.splitext(file)
            if ext == '.py':
                files.append(file)
                print file

        for file in files:
            flag = 0
            lines = []
            input = open(file, 'r')
            for line in input:
                print flag, line
                (root, ext) = os.path.splitext(file)
                line = line.rstrip()
                lines.append(line+'\n')
                output = open(root+'t'+ext, 'w')
                output.writelines(lines)
                flag += 1
                os.rename(root+'t'+ext, root+ext)

removing = Remove()
dir_name = '/home/embedded/Desktop/Brisa/Brisa/src/test/pep8/test_replace'
diretorios = removing.whitespace_remove(dir_name)

if __name__ == "__main__":
    print 'Finish!'