#!/usr/bin/env python

import sys

from circuits import Debugger
from circuits.drivers import INotifyDriver

driver = INotifyDriver() + Debugger()
driver.add(sys.argv[1])
driver.run()
