#!/usr/bin/python
# pep8.py - Check Python source code formatting, according to PEP 8
# Copyright (C) 2006 Johann C. Rocholl <johann@browsershots.org>
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation files
# (the "Software"), to deal in the Software without restriction,
# including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software,
# and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

"""
Check Python source code formatting, according to PEP 8:
http://www.python.org/dev/peps/pep-0008/

For usage and a list of options, try this:
$ python pep8.py -h

This program and its regression test suite live here:
http://svn.browsershots.org/trunk/devtools/pep8/
http://trac.browsershots.org/browser/trunk/devtools/pep8/

Groups of errors and warnings:
E errors
W warnings
100 indentation
200 whitespace
300 blank lines
400 imports
500 line length
600 deprecation
700 statements

You can add checks to this program by writing plugins. Each plugin is
a simple function that is called for each line of source code, either
physical or logical.

Physical line:
- Raw line of text from the input file.

Logical line:
- Multi-line statements converted to a single line.
- Stripped left and right.
- Contents of strings replaced with 'xxx' of same length.
- Comments removed.

The check function requests physical or logical lines by the name of
the first argument:

def maximum_line_length(physical_line)
def extraneous_whitespace(logical_line)
def indentation(logical_line, indent_level, state)

The last example above demonstrates how check plugins can request
additional information with extra arguments. All attributes of the
Checker object are available. Some examples:

lines: a list of the raw lines from the input file
tokens: the tokens that contribute to this logical line
state: dictionary for passing information across lines
indent_level: indentation (with tabs expanded to multiples of 8)

The docstring of each check function shall be the relevant part of
text from PEP 8. It is printed if the user enables --show-pep8.

"""

import os
import sys
import re
import time
import inspect
import tokenize
from optparse import OptionParser
from keyword import iskeyword
from fnmatch import fnmatch

__version__ = '0.2.0'
__revision__ = '$Rev$'

default_exclude = '.svn,CVS,*.pyc,*.pyo'

indent_match = re.compile(r'([ \t]*)').match
raise_comma_match = re.compile(r'raise\s+\w+\s*(,)').match

operators = """
+  -  *  /  %  ^  &  |  =  <  >  >>  <<
+= -= *= /= %= ^= &= |= == <= >= >>= <<=
!= <> :
in is or not and
""".split()

options = None
args = None


##############################################################################
# Plugins (check functions) for physical lines
##############################################################################


def tabs_or_spaces(physical_line, state):
    """
    Never mix tabs and spaces.

    The most popular way of indenting Python is with spaces only.  The
    second-most popular way is with tabs only.  Code indented with a mixture
    of tabs and spaces should be converted to using spaces exclusively.  When
    invoking the Python command line interpreter with the -t option, it issues
    warnings about code that illegally mixes tabs and spaces.  When using -tt
    these warnings become errors.  These options are highly recommended!
    """
    indent = indent_match(physical_line).group(1)
    if not indent:
        return
    if 'indent_char' in state:
        indent_char = state['indent_char']
    else:
        indent_char = indent[0]
        state['indent_char'] = indent_char
    for offset, char in enumerate(indent):
        if char != indent_char:
            return offset, "E101 indentation contains mixed spaces and tabs"


def tabs_obsolete(physical_line):
    """
    For new projects, spaces-only are strongly recommended over tabs.  Most
    editors have features that make this easy to do.
    """
    indent = indent_match(physical_line).group(1)
    if indent.count('\t'):
        return indent.index('\t'), "W191 indentation contains tabs"


def trailing_whitespace(physical_line):
    """
    JCR: Trailing whitespace is superfluous.
    """
    physical_line = physical_line.rstrip('\n') # chr(10), newline
    physical_line = physical_line.rstrip('\r') # chr(13), carriage return
    physical_line = physical_line.rstrip('\x0c') # chr(12), form feed, ^L
    stripped = physical_line.rstrip()
    if physical_line != stripped:
        return len(stripped), "W291 trailing whitespace"


def maximum_line_length(physical_line):
    """
    Limit all lines to a maximum of 79 characters.

    There are still many devices around that are limited to 80 character
    lines; plus, limiting windows to 80 characters makes it possible to have
    several windows side-by-side.  The default wrapping on such devices looks
    ugly.  Therefore, please limit all lines to a maximum of 79 characters.
    For flowing long blocks of text (docstrings or comments), limiting the
    length to 72 characters is recommended.
    """
    length = len(physical_line.rstrip())
    if length > 79:
        return 79, "E501 line too long (%d characters)" % length


##############################################################################
# Plugins (check functions) for logical lines
##############################################################################


def blank_lines(logical_line, state, indent_level):
    """
    Separate top-level function and class definitions with two blank lines.

    Method definitions inside a class are separated by a single blank line.

    Extra blank lines may be used (sparingly) to separate groups of related
    functions.  Blank lines may be omitted between a bunch of related
    one-liners (e.g. a set of dummy implementations).

    Use blank lines in functions, sparingly, to indicate logical sections.
    """
    line = logical_line
    blank_lines = state.get('blank_lines', 0)
    if line.startswith('def '):
        if indent_level > 0 and blank_lines != 1:
            return 0, "E301 expected 1 blank line, found %d" % blank_lines
        if indent_level == 0 and blank_lines != 2:
            return 0, "E302 expected 2 blank lines, found %d" % blank_lines
    if blank_lines > 2:
        return 0, "E303 too many blank lines (%d)" % blank_lines


def extraneous_whitespace(logical_line):
    """
    Avoid extraneous whitespace in the following situations:

    - Immediately inside parentheses, brackets or braces.

    - Immediately before a comma, semicolon, or colon.
    """
    line = logical_line
    for char in '([{':
        found = line.find(char + ' ')
        if found > -1:
            return found + 1, "E201 whitespace after '%s'" % char
    for char in '}])':
        found = line.find(' ' + char)
        if found > -1 and line[found - 1] != ',':
            return found, "E202 whitespace before '%s'" % char
    for char in ',;:':
        found = line.find(' ' + char)
        if found > -1:
            return found, "E203 whitespace before '%s'" % char


def missing_whitespace(logical_line):
    """
    JCR: Each comma, semicolon or colon should be followed by whitespace.
    """
    line = logical_line
    for index in range(len(line) - 1):
        for char in ',;:':
            if line[index] == char and line[index + 1] != ' ':
                return index, "E204 missing whitespace after '%s'" % char


def indentation(logical_line, indent_level, state):
    """
    Use 4 spaces per indentation level.

    For really old code that you don't want to mess up, you can continue to
    use 8-space tabs.
    """
    line = logical_line
    previous_level = state.get('indent_level', 0)
    indent_expect = state.get('indent_expect', False)
    state['indent_expect'] = line.rstrip('#').rstrip().endswith(':')
    indent_char = state.get('indent_char', ' ')
    state['indent_level'] = indent_level
    if indent_char == ' ' and indent_level % 4:
        return 0, "E111 indentation is not a multiple of four"
    if indent_expect and indent_level <= previous_level:
        return 0, "E112 expected an indented block"
    if not indent_expect and indent_level > previous_level:
        return 0, "E113 unexpected indentation"


def whitespace_before_parameters(logical_line, tokens):
    """
    Avoid extraneous whitespace in the following situations:

    - Immediately before the open parenthesis that starts the argument
      list of a function call.

    - Immediately before the open parenthesis that starts an indexing or
      slicing.
    """
    prev_type = tokens[0][0]
    prev_text = tokens[0][1]
    prev_end = tokens[0][3]
    for index in range(1, len(tokens)):
        token_type, text, start, end, line = tokens[index]
        if (token_type == tokenize.OP and
            text in '([' and
            start != prev_end and
            prev_type == tokenize.NAME and
            (index < 2 or tokens[index - 2][1] != 'class') and
            (not iskeyword(prev_text))):
            return prev_end, "E211 whitespace before '%s'" % text
        prev_type = token_type
        prev_text = text
        prev_end = end


def whitespace_around_operator(logical_line):
    """
    Avoid extraneous whitespace in the following situations:

    - More than one space around an assignment (or other) operator to
      align it with another.
    """
    line = logical_line
    for operator in operators:
        found = line.find('  ' + operator)
        if found > -1:
            return found, "E221 multiple spaces before operator"
        found = line.find('\t' + operator)
        if found > -1:
            return found, "E222 tab before operator"


def imports_on_separate_lines(logical_line):
    """
    Imports should usually be on separate lines.
    """
    line = logical_line
    if line.startswith('import '):
        found = line.find(',')
        if found > -1:
            return found, "E401 multiple imports on one line"


def compound_statements(logical_line):
    """
    Compound statements (multiple statements on the same line) are
    generally discouraged.
    """
    line = logical_line
    found = line.find(':')
    if -1 < found < len(line) - 1:
        before = line[: found]
        if before.count('{') <= before.count('}'):
            return found, "E701 multiple statements on one line (colon)"
    found = line.find(';')
    if -1 < found:
        return found, "E702 multiple statements on one line (semicolon)"


def python_3000_has_key(logical_line):
    """
    The {}.has_key() method will be removed in the future version of
    Python. Use the 'in' operation instead, like:
    d = {"a": 1, "b": 2}
    if "b" in d:
        print d["b"]
    """
    pos = logical_line.find('.has_key(')
    if pos > -1:
        return pos, "W601 .has_key() is deprecated, use 'in'"


def python_3000_raise_comma(logical_line):
    """
    When raising an exception, use "raise ValueError('message')"
    instead of the older form "raise ValueError, 'message'".

    The paren-using form is preferred because when the exception arguments
    are long or include string formatting, you don't need to use line
    continuation characters thanks to the containing parentheses.  The older
    form will be removed in Python 3000.
    """
    match = raise_comma_match(logical_line)
    if match:
        return match.start(1), "W602 deprecated form of raising exception"


##############################################################################
# Helper functions
##############################################################################


def expand_indent(line):
    """
    Return the amount of indentation.
    Tabs are expanded to the next multiple of 8.

    >>> expand_indent('    ')
    4
    >>> expand_indent('\\t')
    8
    >>> expand_indent('    \\t')
    8
    >>> expand_indent('       \\t')
    8
    >>> expand_indent('        \\t')
    16
    """
    result = 0
    for char in line:
        if char == '\t':
            result = result / 8 * 8 + 8
        elif char == ' ':
            result += 1
        else:
            break
    return result


##############################################################################
# Framework to run all checks
##############################################################################


def message(text):
    """Print a message."""
    # print >> sys.stderr, options.prog + ': ' + text
    # print >> sys.stderr, text
    print text


def find_checks(argument_name):
    """
    Find all globally visible functions where the first argument name
    starts with argument_name.
    """
    checks = []
    function_type = type(find_checks)
    for name, function in globals().iteritems():
        if type(function) is function_type:
            args = inspect.getargspec(function)[0]
            if len(args) >= 1 and args[0].startswith(argument_name):
                checks.append((name, function, args))
    checks.sort()
    return checks


def mute_string(text):
    """
    Replace contents with 'xxx' to prevent syntax matching.

    >>> mute_string('"abc"')
    '"xxx"'
    >>> mute_string("'''abc'''")
    "'''xxx'''"
    >>> mute_string("r'abc'")
    "r'xxx'"
    """
    start = 1
    end = len(text) - 1
    # String modifiers (e.g. u or r)
    if text.endswith('"'):
        start += text.index('"')
    elif text.endswith("'"):
        start += text.index("'")
    # Triple quotes
    if text.endswith('"""') or text.endswith("'''"):
        start += 2
        end -= 2
    return text[: start] + 'x' * (end - start) + text[end: ]


class Checker:
    """
    Load a Python source file, tokenize it, check coding style.
    """

    def __init__(self, filename):
        self.filename = filename
        self.lines = file(filename).readlines()
        self.physical_checks = find_checks('physical_line')
        self.logical_checks = find_checks('logical_line')
        options.counters['physical lines'] = \
            options.counters.get('physical lines', 0) + len(self.lines)

    def readline(self):
        """
        Get the next line from the input buffer.
        """
        self.line_number += 1
        if self.line_number > len(self.lines):
            return ''
        return self.lines[self.line_number - 1]

    def readline_check_physical(self):
        """
        Check and return the next physical line. This method can be
        used to feed tokenize.generate_tokens.
        """
        line = self.readline()
        self.check_physical(line)
        return line

    def run_check(self, check, argument_names):
        """
        Run a check plugin.
        """
        arguments = []
        for name in argument_names:
            arguments.append(getattr(self, name))
        return check(*arguments)

    def check_physical(self, line):
        """
        Run all physical checks on a raw input line.
        """
        self.physical_line = line
        for name, check, argument_names in self.physical_checks:
            result = self.run_check(check, argument_names)
            if result is not None:
                offset, text = result
                self.report_error(self.line_number, offset, text, check)

    def build_tokens_line(self):
        """
        Build a logical line from tokens.
        """
        self.mapping = []
        logical = []
        length = 0
        previous = None
        for token in self.tokens:
            token_type, text = token[0: 2]
            if token_type in (tokenize.COMMENT, tokenize.NL,
                              tokenize.INDENT, tokenize.DEDENT,
                              tokenize.NEWLINE):
                continue
            if token_type == tokenize.STRING:
                text = mute_string(text)
            if previous:
                end_line, end = previous[3]
                start_line, start = token[2]
                if end_line != start_line: # different row
                    if self.lines[end_line - 1][end - 1] not in '{[(':
                        logical.append(' ')
                        length += 1
                elif end != start: # different column
                    fill = self.lines[end_line - 1][end: start]
                    logical.append(fill)
                    length += len(fill)
            self.mapping.append((length, token))
            logical.append(text)
            length += len(text)
            previous = token
        self.logical_line = ''.join(logical)

    def check_logical(self):
        """
        Build a line from tokens and run all logical checks on it.
        """
        options.counters['logical lines'] = \
            options.counters.get('logical lines', 0) + 1
        self.build_tokens_line()
        first_line = self.lines[self.mapping[0][1][2][0] - 1]
        indent = first_line[: self.mapping[0][1][2][1]]
        self.indent_level = expand_indent(indent)
        if options.verbose >= 2:
            print self.logical_line[: 80].rstrip()
        for name, check, argument_names in self.logical_checks:
            if options.verbose >= 3:
                print '   ', name
            result = self.run_check(check, argument_names)
            if result is not None:
                offset, text = result
                if type(offset) is tuple:
                    original_number, original_offset = offset
                else:
                    for token_offset, token in self.mapping:
                        if offset >= token_offset:
                            original_number = token[2][0]
                            original_offset = (token[2][1]
                                               + offset - token_offset)
                self.report_error(original_number, original_offset,
                                  text, check)

    def check_all(self):
        """
        Run all checks on the input file.
        """
        self.file_errors = 0
        self.line_number = 0
        self.state = {'blank_lines': 0}
        self.tokens = []
        parens = 0
        for token in tokenize.generate_tokens(self.readline_check_physical):
            # print tokenize.tok_name[token[0]], repr(token)
            self.tokens.append(token)
            token_type, text = token[0: 2]
            if token_type == tokenize.OP and text in '([{':
                parens += 1
            if token_type == tokenize.OP and text in '}])':
                parens -= 1
            if token_type == tokenize.NEWLINE and not parens:
                self.check_logical()
                self.state['blank_lines'] = 0
                self.tokens = []
            if token_type == tokenize.NL and len(self.tokens) == 1:
                self.state['blank_lines'] += 1
                self.tokens = []
        return self.file_errors

    def report_error(self, line_number, offset, text, check):
        """
        Report an error, according to options.
        """
        if options.quiet == 1 and not self.file_errors:
            message(self.filename)
        self.file_errors += 1
        code = text[: 4]
        options.counters[code] = options.counters.get(code, 0) + 1
        options.messages[code] = text[5: ]
        if options.quiet:
            return
        if options.testsuite:
            base = os.path.basename(self.filename)[: 4]
            if base == code:
                return
            if base[0] == 'E' and code[0] == 'W':
                return
        if ignore_code(code):
            return
        if options.counters[code] == 1 or options.repeat:
            message("%s:%s:%d: %s" %
                    (self.filename, line_number, offset + 1, text))
            if options.show_source:
                line = self.lines[line_number - 1]
                message(line.rstrip())
                message(' ' * offset + '^')
            if options.show_pep8:
                message(check.__doc__.lstrip('\n').rstrip())


def input_file(filename):
    """
    Run all checks on a Python source file.
    """
    if excluded(filename) or not filename_match(filename):
        return {}
    if options.verbose:
        message('checking ' + filename)
    options.counters['files'] = options.counters.get('files', 0) + 1
    errors = Checker(filename).check_all()
    if options.testsuite and not errors:
        message("%s: %s" % (filename, "no errors found"))


def input_dir(dirname):
    """
    Check all Python source files in this directory and all subdirectories.
    """
    dirname = dirname.rstrip('/')
    if excluded(dirname):
        return
    for root, dirs, files in os.walk(dirname):
        if options.verbose:
            message('directory ' + root)
        options.counters['directories'] = \
            options.counters.get('directories', 0) + 1
        dirs.sort()
        for subdir in dirs:
            if excluded(subdir):
                dirs.remove(subdir)
        files.sort()
        for filename in files:
            input_file(os.path.join(root, filename))


def excluded(filename):
    """
    Check if options.exclude contains a pattern that matches filename.
    """
    basename = os.path.basename(filename)
    for pattern in options.exclude:
        if fnmatch(basename, pattern):
            # print basename, 'excluded because it matches', pattern
            return True


def filename_match(filename):
    """
    Check if options.filename contains a pattern that matches filename.
    If options.filename is unspecified, this always returns True.
    """
    if not options.filename:
        return True
    for pattern in options.filename:
        if fnmatch(filename, pattern):
            return True


def ignore_code(code):
    """
    Check if options.ignore contains a prefix of the error code.
    """
    for ignore in options.ignore:
        if code.startswith(ignore):
            return True


def get_error_statistics():
    """Get error statistics."""
    return get_statistics("E")


def get_warning_statistics():
    """Get warning statistics."""
    return get_statistics("W")


def get_statistics(prefix=''):
    """
    Get statistics for message codes that start with the prefix.

    prefix='' matches all errors and warnings
    prefix='E' matches all errors
    prefix='W' matches all warnings
    prefix='E4' matches all errors that have to do with imports
    """
    stats = []
    keys = options.messages.keys()
    keys.sort()
    for key in keys:
        if key.startswith(prefix):
            stats.append('%-7s %s %s' %
                         (options.counters[key], key, options.messages[key]))
    return stats


def print_statistics(prefix=''):
    """Print overall statistics (number of errors and warnings)."""
    for line in get_statistics(prefix):
        print line


def print_benchmark(elapsed):
    """
    Print benchmark numbers.
    """
    print '%-7.2f %s' % (elapsed, 'seconds elapsed')
    keys = ['directories', 'files',
            'logical lines', 'physical lines']
    for key in keys:
        if key in options.counters:
            print '%-7d %s per second (%d total)' % (
                options.counters[key] / elapsed, key,
                options.counters[key])


def process_options(arglist=None):
    """
    Process options passed either via arglist or via command line args.
    """
    global options, args
    usage = "%prog [options] input ..."
    parser = OptionParser(usage)
    parser.add_option('-v', '--verbose', default=0, action='count',
                      help="print status messages, or debug with -vv")
    parser.add_option('-q', '--quiet', default=0, action='count',
                      help="report only file names, or nothing with -qq")
    parser.add_option('--exclude', metavar='patterns', default=default_exclude,
                      help="skip matches (default %s)" % default_exclude)
    parser.add_option('--filename', metavar='patterns',
                      help="only check matching files (e.g. *.py)")
    parser.add_option('--ignore', metavar='errors', default='',
                      help="skip errors and warnings (e.g. E4,W)")
    parser.add_option('--repeat', action='store_true',
                      help="show all occurrences of the same error")
    parser.add_option('--show-source', action='store_true',
                      help="show source code for each error")
    parser.add_option('--show-pep8', action='store_true',
                      help="show text of PEP 8 for each error")
    parser.add_option('--statistics', action='store_true',
                      help="count errors and warnings")
    parser.add_option('--benchmark', action='store_true',
                      help="measure processing speed")
    parser.add_option('--testsuite', metavar='dir',
                      help="run regression tests from dir")
    parser.add_option('--doctest', action='store_true',
                      help="run doctest on myself")
    options, args = parser.parse_args(arglist)
    if options.testsuite:
        args.append(options.testsuite)
    if len(args) == 0:
        parser.error('input not specified')
    options.prog = os.path.basename(sys.argv[0])
    options.exclude = options.exclude.split(',')
    for index in range(len(options.exclude)):
        options.exclude[index] = options.exclude[index].rstrip('/')
    if options.filename:
        options.filename = options.filename.split(',')
    if options.ignore:
        options.ignore = options.ignore.split(',')
    else:
        options.ignore = []
    options.counters = {}
    options.messages = {}

    return options, args


def _main():
    """
    Parse options and run checks on Python source.
    """
    options, args = process_options()
    if options.doctest:
        import doctest
        return doctest.testmod()
    start_time = time.time()
    for path in args:
        if os.path.isdir(path):
            input_dir(path)
        else:
            input_file(path)
    elapsed = time.time() - start_time
    if options.statistics:
        print_statistics()
    if options.benchmark:
        print_benchmark(elapsed)


if __name__ == '__main__':
    _main()
