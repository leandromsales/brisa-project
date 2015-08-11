# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007 Brisa Team <brisa-develop@garage.maemo.org>

# Implementation of a simple daemon Application of UPnP Control Point

from media_devices.control_point import control_point_gui


if __name__ == '__main__':
    control_point_gui.main()