# Module:	cgifs
# Date:		3rd February 2009
# Author:	James Mills, prologic at shortcircuit dot net dot au

"""CGI Field Storage

This module implements a custom CGI Field Storage class.
"""

import cgi

class FieldStorage(cgi.FieldStorage):

    def read_lines_to_eof(self):
        """Internal: read lines until EOF."""
        while 1:
            line = self.fp.readline()
            if not line:
                self.done = -1
                break
            self.__write(line)
    
    def read_lines_to_outerboundary(self):
        """Internal: read lines until outerboundary."""
        next = "--" + self.outerboundary
        last = next + "--"
        delim = ""
        last_line_lfend = True
        while 1:
            line = self.fp.readline()
            if not line:
                self.done = -1
                break
            if line[:2] == "--" and last_line_lfend:
                strippedline = line.strip()
                if strippedline == next:
                    break
                if strippedline == last:
                    self.done = 1
                    break
            odelim = delim
            if line[-2:] == "\r\n":
                delim = "\r\n"
                line = line[:-2]
                last_line_lfend = True
            elif line[-1] == "\n":
                delim = "\n"
                line = line[:-1]
                last_line_lfend = True
            else:
                delim = ""
                last_line_lfend = False
            self.__write(odelim + line)
    
    def skip_lines(self):
        """Internal: skip lines until outer boundary if defined."""
        if not self.outerboundary or self.done:
            return
        next = "--" + self.outerboundary
        last = next + "--"
        last_line_lfend = True
        while 1:
            line = self.fp.readline()
            if not line:
                self.done = -1
                break
            if line[:2] == "--" and last_line_lfend:
                strippedline = line.strip()
                if strippedline == next:
                    break
                if strippedline == last:
                    self.done = 1
                    break
            if line.endswith('\n'):
                last_line_lfend = True
            else:
                last_line_lfend = False
