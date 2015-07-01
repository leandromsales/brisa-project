:mod:`log` --- Logging facilities
==================================

.. module: log
    :synopsis: Provides logging facility with a colored logging feature.

:ref:`python-brisa <python-brisa>` provides a logging module with a colored
logging feature.

The logging module is configured by the default manager
described on :mod:`config` under the default *brisa*
section.

The two main parameters that can be adjusted are:

* brisa.logging, determines the highest logging level (ERROR, INFO, CRITICAL, DEBUG, WARNING)
* brisa.logging_output, which determines where the log should appear (commonly used values are *stdout*, *file* - located at ~/brisa.log).

These parameters can be modified through the config module or using the ``brisa-conf``
tool. Try ``brisa-conf -h`` for usage.

Global Logger
-------------

This module provides a global (or root) logger, which can be used simply by::

    from brisa.core import log

    log.debug('My debug message.')
    log.error('My error message.')
    log.info('My info message.')


Loggers
-------

For retrieving loggers other than the global/root one, use the ``log.getLogger()``
function as follows::

    from brisa.core import log

    mylogger = log.getLogger('my_logger')


