# validate.py
# A Validator object
# Copyright (C) 2005 Michael Foord, Mark Andrews, Nicola Larosa
# E-mail: fuzzyman AT voidspace DOT org DOT uk
#         mark AT la-la DOT com
#         nico AT tekNico DOT net

# This software is licensed under the terms of the BSD license.
# http://www.voidspace.org.uk/python/license.shtml
# Basically you're free to copy, modify, distribute and relicense it,
# So long as you keep a copy of the license with it.

# Scripts maintained at http://www.voidspace.org.uk/python/index.shtml
# For information about bugfixes, updates and support, please join the
# ConfigObj mailing list:
# http://lists.sourceforge.net/lists/listinfo/configobj-develop
# Comments, suggestions and bug reports welcome.

"""
    The Validator object is used to check that supplied values
    conform to a specification.

    The value can be supplied as a string - e.g. from a config file.
    In this case the check will also *convert* the value to
    the required type. This allows you to add validation
    as a transparent layer to access data stored as strings.
    The validation checks that the data is correct *and*
    converts it to the expected type.

    Some standard checks are provided for basic data types.
    Additional checks are easy to write. They can be
    provided when the ``Validator`` is instantiated or
    added afterwards.

    The standard functions work with the following basic data types :

    * integers
    * floats
    * booleans
    * strings
    * ip_addr

    plus lists of these datatypes

    Adding additional checks is done through coding simple functions.

    The full set of standard checks are :

    * 'integer': matches integer values (including negative)
                 Takes optional 'min' and 'max' arguments : ::

                   integer()
                   integer(3, 9)  # any value from 3 to 9
                   integer(min=0) # any positive value
                   integer(max=9)

    * 'float': matches float values
               Has the same parameters as the integer check.

    * 'boolean': matches boolean values - ``True`` or ``False``
                 Acceptable string values for True are :
                   true, on, yes, 1
                 Acceptable string values for False are :
                   false, off, no, 0

                 Any other value raises an error.

    * 'ip_addr': matches an Internet Protocol address, v.4, represented
                 by a dotted-quad string, i.e. '1.2.3.4'.

    * 'string': matches any string.
                Takes optional keyword args 'min' and 'max'
                to specify min and max lengths of the string.

    * 'list': matches any list.
              Takes optional keyword args 'min', and 'max' to specify min and
              max sizes of the list.

    * 'int_list': Matches a list of integers.
                  Takes the same arguments as list.

    * 'float_list': Matches a list of floats.
                    Takes the same arguments as list.

    * 'bool_list': Matches a list of boolean values.
                   Takes the same arguments as list.

    * 'ip_addr_list': Matches a list of IP addresses.
                     Takes the same arguments as list.

    * 'string_list': Matches a list of strings.
                     Takes the same arguments as list.

    * 'mixed_list': Matches a list with different types in
                    specific positions. List size must match
                    the number of arguments.

                    Each position can be one of :
                    'integer', 'float', 'ip_addr', 'string', 'boolean'

                    So to specify a list with two strings followed
                    by two integers, you write the check as : ::

                      mixed_list('string', 'string', 'integer', 'integer')

    * 'pass': This check matches everything ! It never fails
              and the value is unchanged.

              It is also the default if no check is specified.

    * 'option': This check matches any from a list of options.
                You specify this check with : ::

                  option('option 1', 'option 2', 'option 3')

    You can supply a default value (returned if no value is supplied)
    using the default keyword argument.

    You specify a list argument for default using a list constructor syntax in
    the check : ::

        checkname(arg1, arg2, default=list('val 1', 'val 2', 'val 3'))

    A badly formatted set of arguments will raise a ``VdtParamError``.
"""

__docformat__ = "restructuredtext en"

__version__ = '0.2.2'

__revision__ = '$Id: validate.py 123 2005-09-08 08:54:28Z fuzzyman $'

__all__ = (
    '__version__',
    'dottedQuadToNum',
    'numToDottedQuad',
    'ValidateError',
    'VdtUnknownCheckError',
    'VdtParamError',
    'VdtTypeError',
    'VdtValueError',
    'VdtValueTooSmallError',
    'VdtValueTooBigError',
    'VdtValueTooShortError',
    'VdtValueTooLongError',
    'VdtMissingValue',
    'Validator',
    'is_integer',
    'is_float',
    'is_bool',
    'is_list',
    'is_ip_addr',
    'is_string',
    'is_int_list',
    'is_bool_list',
    'is_float_list',
    'is_string_list',
    'is_ip_addr_list',
    'is_mixed_list',
    'is_option',
    '__docformat__',
)

import sys
INTP_VER = sys.version_info[:2]
if INTP_VER < (2, 2):
    raise RuntimeError("Python v.2.2 or later needed")

import re
StringTypes = (str, unicode)


_list_arg = re.compile(r'''
    (?:
        ([a-zA-Z_][a-zA-Z0-9_]*)\s*=\s*list\(
            (
                (?:
                    \s*
                    (?:
                        (?:".*?")|              # double quotes
                        (?:'.*?')|              # single quotes
                        (?:[^'",\s\)][^,\)]*?)  # unquoted
                    )
                    \s*,\s*
                )*
                (?:
                    (?:".*?")|              # double quotes
                    (?:'.*?')|              # single quotes
                    (?:[^'",\s\)][^,\)]*?)  # unquoted
                )?                          # last one
            )
        \)
    )
''', re.VERBOSE)    # two groups

_list_members = re.compile(r'''
    (
        (?:".*?")|              # double quotes
        (?:'.*?')|              # single quotes
        (?:[^'",\s=][^,=]*?)       # unquoted
    )
    (?:
    (?:\s*,\s*)|(?:\s*$)            # comma
    )
''', re.VERBOSE)    # one group

_paramstring = r'''
    (?:
        (
            (?:
                [a-zA-Z_][a-zA-Z0-9_]*\s*=\s*list\(
                    (?:
                        \s*
                        (?:
                            (?:".*?")|              # double quotes
                            (?:'.*?')|              # single quotes
                            (?:[^'",\s\)][^,\)]*?)       # unquoted
                        )
                        \s*,\s*
                    )*
                    (?:
                        (?:".*?")|              # double quotes
                        (?:'.*?')|              # single quotes
                        (?:[^'",\s\)][^,\)]*?)       # unquoted
                    )?                              # last one
                \)
            )|
            (?:
                (?:".*?")|              # double quotes
                (?:'.*?')|              # single quotes
                (?:[^'",\s=][^,=]*?)|       # unquoted
                (?:                         # keyword argument
                    [a-zA-Z_][a-zA-Z0-9_]*\s*=\s*
                    (?:
                        (?:".*?")|              # double quotes
                        (?:'.*?')|              # single quotes
                        (?:[^'",\s=][^,=]*?)       # unquoted
                    )
                )
            )
        )
        (?:
            (?:\s*,\s*)|(?:\s*$)            # comma
        )
    )
    '''

_matchstring = '^%s*' % _paramstring

# Python pre 2.2.1 doesn't have bool
try:
    bool
except NameError:
    def bool(val):
        """Simple boolean equivalent function. """
        if val:
            return 1
        else:
            return 0

def dottedQuadToNum(ip):
    """
    Convert decimal dotted
