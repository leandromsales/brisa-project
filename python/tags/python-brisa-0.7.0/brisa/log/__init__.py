# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

"""
BRisa logging API provides to the developer logging levels for messages.


Basic usage
===========
After setting the log level, it's almost transparent the way you use the logging
API. If you want to have some message as a debug message, just use
log.debug(msg) and the log module will print your message as a debug.

>>> from brisa import log
>>> log.debug('This is my debug message')
>>> log.info('This is my info message')

"""

import os
import logging
import traceback

from logging import getLogger, debug, info, warning, error

from brisa import config


# Logger levels
log_dict = {'CRITICAL': logging.CRITICAL, 'ERROR': logging.ERROR,
            'WARNING': logging.WARNING, 'DEBUG': logging.DEBUG,
            'NOTSET': logging.NOTSET, 'INFO': logging.INFO}


def show_exception(message, e, haltSignal=None):
    """ Shows the exception with a custom message with a specific given
    halt signal.

    @param message: custom message
    @param e: exception to be raised
    @param haltSignal: halt signal

    @type message: string
    @type e: Exception
    @type haltSignal: integer
    """
    print message
    print ("%s%s%s%s") % ("Exception type / message: ", str(type(e)), " / ",
                          e.message)
    traceback.print_exc()
    if isinstance(haltSignal, int):
        print ("%s%s%s") % ("System will be halted with signal ",
                            str(haltSignal), " due to exception.")
        os._exit(haltSignal)


def set_level(level, filename):
    """ Sets the level of logging into the specified config file.

    @param level: logging level (from log_dict)
    @param filename: config filename

    @type level: constant
    @type filename: string
    """
    log_level = log_dict.get(level)

    if not log_level:
        log_level = log_dict.get('NOTSET')

    logging.filename = filename

    if filename == 'stdout':
        filename = None
    else:
        filename = os.path.join(config.brisa_home, "brisa.log")
        logging.filename = filename

    logging.basicConfig(level=log_level, filename=filename)


output = 'stdout'
if config.contains(section='logging', parameter='output'):
    output = config.get_parameter(section='logging', parameter='output')

level = ''
if config.contains(section='logging', parameter='level'):
    level = config.get_parameter(section='logging', parameter='level')

set_level(level, output)
filename = logging.filename
logging.show_exception = show_exception
