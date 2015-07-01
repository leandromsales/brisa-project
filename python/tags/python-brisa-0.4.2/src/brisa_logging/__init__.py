# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

import logging
import brisa_config
import traceback
import os


def show_exception(message, e, haltSignal=None):
    logging.error(message)
    logging.error("Exception type / message: " +str(type(e)) + " / " + str(e))
    logging.error("Stack trace: ")
    logging.error(traceback.print_exc())
    if isinstance(haltSignal, int):
        logging.error("System will be halted with signal " + str(haltSignal) +\
                      " due to exception.")
        os._exit(haltSignal)

level = ''
if brisa_config.contains(section = 'logging', parameter='level'):
    level = brisa_config.get_parameter(section = 'logging', parameter='level')

if level=='CRITICAL':
    log_level=logging.CRITICAL
elif level=='ERROR':
    log_level=logging.ERROR
elif level=='WARNING':
    log_level=logging.WARNING
elif level=='DEBUG':
    log_level=logging.DEBUG
else:
    log_level=logging.INFO

if brisa_config.contains(section = 'logging', parameter='output'):
    output = brisa_config.get_parameter(section = 'logging', \
                                        parameter='output')

logging.filename = "stdout"
if output == "file":
    logging.filename = os.path.join(brisa_config.brisa_home, "brisa.log")
    logging.basicConfig(level=log_level, filename=logging.filename)
else:
    logging.basicConfig(level=log_level)

logging.show_exception = show_exception

#logging.basicConfig(level=logging.DEBUG),
#                    format='%(asctime)s %(levelname)-8s %(message)s',
#                    datefmt='%a, %d %b %Y %H:%M:%S',
#                    filename='brisalog.txt',
#                    filemode='w')