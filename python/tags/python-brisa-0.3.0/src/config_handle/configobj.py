# configobj.py
# A config file reader/writer that supports nested sections in config files.
# Copyright (C) 2005-2006 Michael Foord, Nicola Larosa
# E-mail: fuzzyman AT voidspace DOT org DOT uk
#         nico AT tekNico DOT net

# ConfigObj 4
# http://www.voidspace.org.uk/python/configobj.html

# Released subject to the BSD License
# Please see http://www.voidspace.org.uk/python/license.shtml

# Scripts maintained at http://www.voidspace.org.uk/python/index.shtml
# For information about bugfixes, updates and support, please join the
# ConfigObj mailing list:
# http://lists.sourceforge.net/lists/listinfo/configobj-develop
# Comments, suggestions and bug reports welcome.

from __future__ import generators

import sys
INTP_VER = sys.version_info[:2]
if INTP_VER < (2, 2):
    raise RuntimeError("Python v.2.2 or later needed")

import os, re
import compiler
from types import StringTypes
from warnings import warn
from codecs import BOM_UTF8, BOM_UTF16, BOM_UTF16_BE, BOM_UTF16_LE

# A dictionary mapping BOM to
# the encoding to decode with, and what to set the
# encoding attribute to.
BOMS = {
    BOM_UTF8: ('utf_8', None),
    BOM_UTF16_BE: ('utf16_be', 'utf_16'),
    BOM_UTF16_LE: ('utf16_le', 'utf_16'),
    BOM_UTF16: ('utf_16', 'utf_16'),
    }
# All legal variants of the BOM codecs.
# TODO: the list of aliases is not meant to be exhaustive, is there a
#   better way ?
BOM_LIST = {
    'utf_16': 'utf_16',
    'u16': 'utf_16',
    'utf16': 'utf_16',
    'utf-16': 'utf_16',
    'utf16_be': 'utf16_be',
    'utf_16_be': 'utf16_be',
    'utf-16be': 'utf16_be',
    'utf16_le': 'utf16_le',
    'utf_16_le': 'utf16_le',
    'utf-16le': 'utf16_le',
    'utf_8': 'utf_8',
    'u8': 'utf_8',
    'utf': 'utf_8',
    'utf8': 'utf_8',
    'utf-8': 'utf_8',
    }

# Map of encodings to the BOM to write.
BOM_SET = {
    'utf_8': BOM_UTF8,
    'utf_16': BOM_UTF16,
    'utf16_be': BOM_UTF16_BE,
    'utf16_le': BOM_UTF16_LE,
    None: BOM_UTF8
    }

try:
    from validate import VdtMissingValue
except ImportError:
    VdtMissingValue = None

try:
    enumerate
except NameError:
    def enumerate(obj):
        """enumerate for Python 2.2."""
        i = -1
        for item in obj:
            i += 1
            yield i, item

try:
    True, False
except NameError:
    True, False = 1, 0


__version__ = '4.3.2'

__revision__ = '$Id: configobj.py 156 2006-01-31 14:57:08Z fuzzyman $'

__docformat__ = "restructuredtext en"

# NOTE: Does it make sense to have the following in __all__ ?
# NOTE: DEFAULT_INDENT_TYPE, NUM_INDENT_SPACES, MAX_INTERPOL_DEPTH
# NOTE: If used via ``from configobj import...``
# NOTE: They are effectively read only
__all__ = (
    '__version__',
    'DEFAULT_INDENT_TYPE',
    'NUM_INDENT_SPACES',
    'MAX_INTERPOL_DEPTH',
    'ConfigObjError',
    'NestingError',
    'ParseError',
    'DuplicateError',
    'ConfigspecError',
    'ConfigObj',
    'SimpleVal',
    'InterpolationError',
    'InterpolationDepthError',
    'MissingInterpolationOption',
    'RepeatSectionError',
    'UnreprError',
    'UnknownType',
    '__docformat__',
    'flatten_errors',
)

DEFAULT_INDENT_TYPE = ' '
NUM_INDENT_SPACES = 4
MAX_INTERPOL_DEPTH = 10

OPTION_DEFAULTS = {
    'interpolation': True,
    'raise_errors': False,
    'list_values': True,
    'create_empty': False,
    'file_error': False,
    'configspec': None,
    'stringify': True,
    # option may be set to one of ('', ' ', '\t')
    'indent_type': None,
    'encoding': None,
    'default_encoding': None,
    'unrepr': False,
    'write_empty_values': False,
}


def getObj(s):
    s = "a=" + s
    p = compiler.parse(s)
    return p.getChildren()[1].getChildren()[0].getChildren()[1]

class UnknownType(Exception):
    pass

class Builder(object):

    def build(self, o):
        m = getattr(self, 'build_' + o.__class__.__name__, None)
        if m is None:
            raise UnknownType(o.__class__.__name__)
        return m(o)

    def build_List(self, o):
        return map(self.build, o.getChildren())

    def build_Const(self, o):
        return o.value

    def build_Dict(self, o):
        d = {}
        i = iter(map(self.build, o.getChildren()))
        for el in i:
            d[el] = i.next()
        return d

    def build_Tuple(self, o):
        return tuple(self.build_List(o))

    def build_Name(self, o):
        if o.name == 'None':
            return None
        if o.name == 'True':
            return True
        if o.name == 'False':
            return False

        # An undefinted Name
        raise UnknownType('Undefined Name')

    def build_Add(self, o):
        real, imag = map(self.build_Const, o.getChildren())
        try:
            real = float(real)
        except TypeError:
            raise UnknownType('Add')
        if not isinstance(imag, complex) or imag.real != 0.0:
            raise UnknownType('Add')
        return real+imag

    def build_Getattr(self, o):
        parent = self.build(o.expr)
        return getattr(parent, o.attrname)

    def build_UnarySub(self, o):
        return -self.build_Const(o.getChildren()[0])

    def build_UnaryAdd(self, o):
        return self.build_Const(o.getChildren()[0])

def unrepr(s):
    if not s:
        return s
    return Builder().build(getObj(s))

def _splitlines(instring):
    """Split a string on lines, without losing line endings or truncating."""


class ConfigObjError(SyntaxError):
    """
    This is the base class for all errors that ConfigObj raises.
    It is a subclass of SyntaxError.
    """
    def __init__(self, message='', line_number=None, line=''):
        self.line = line
        self.line_number = line_number
        self.message = message
        SyntaxError.__init__(self, message)

class NestingError(ConfigObjError):
    """
    This error indicates a level of nesting that doesn't match.
    """

class ParseError(ConfigObjError):
    """
    This error indicates that a line is badly written.
    It is neither a valid ``key = value`` line,
    nor a valid section marker line.
    """

class DuplicateError(ConfigObjError):
    """
    The keyword or section specified already exists.
    """

class ConfigspecError(ConfigObjError):
    """
    An error occured whilst parsing a configspec.
    """

class InterpolationError(ConfigObjError):
    """Base class for the two interpolation errors."""

class InterpolationDepthError(InterpolationError):
    """Maximum interpolation depth exceeded in string interpolation."""

    def __init__(self, option):
        InterpolationError.__init__(
            self,
            'max interpolation depth exceeded in value "%s".' % option)

class RepeatSectionError(ConfigObjError):
    """
    This error indicates additional sections in a section with a
    ``__many__`` (repeated) section.
    """

class MissingInterpolationOption(InterpolationError):
    """A value specified for interpolation was missing."""

    def __init__(self, option):
        InterpolationError.__init__(
            self,
            'missing option "%s" in interpolation.' % option)

class UnreprError(ConfigObjError):
    """An error parsing in unrepr mode."""


class Section(dict):
    """
    A dictionary-like object that represents a section in a config file.

    It does string interpolation if the 'interpolate' attribute
    of the 'main' object is set to True.

    Interpolation is tried first from the 'DEFAULT' section of this object,
    next from the 'DEFAULT' section of the parent, lastly the main object.

    A Section will behave like an ordered dictionary - following the
    order of the ``scalars`` and ``sections`` attributes.
    You can use this to change the order of members.

    Iteration follows the order: scalars, then sections.
    """

    _KEYCRE = re.compile(r"%\(([^)]*)\)s|.")

    def __init__(self, parent, depth, main, indict=None, name=None):
        """
        * parent is the section above
        * depth is the depth level of this section
        * main is the main ConfigObj
        * indict is a dictionary to initialise the section with
        """
        if indict is None:
            indict = {}
        dict.__init__(self)
        # used for nesting level *and* interpolat
