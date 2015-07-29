# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Provides thread management.
"""

from brisa.threading.thread_manager import ThreadManager
from brisa.threading.thread_object import ThreadObject
from brisa.threading.threaded_call import ThreadedCall


def run_async_call(function, success_callback=None, error_callback=None,
                   success_callback_cargo=None, error_callback_cargo=None,
                   delay=0, *args, **kwargs):
    """ Interface for running an asynchronous call.

    @param function: function to be called passing *args and **kwargs
    @param success_callback: called in case of success, receives call result
    @param error_callback: called in case of error, receives call result
    @param success_callback_cargo: success callback additional parameters
    @param error_callback_cargo: error callback additional parameters
    @param delay: time to be wait before performing the call
    @param args: arguments to the function
    @param kwargs: arguments to the function

    @type function: callable
    @type success_callback: callable
    @type error_callback: callable
    @type delay: float

    @return: object for monitoring the call
    @rtype: ThreadedCall
    """
    t = ThreadedCall(function, success_callback, error_callback,
                     success_callback_cargo, error_callback_cargo,
                     delay, *args, **kwargs)

    # Perform the call and return the object
    t.start()
    return t
