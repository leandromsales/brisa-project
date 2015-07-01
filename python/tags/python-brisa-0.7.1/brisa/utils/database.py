# Licensed under the MIT license
# http://opensource.org/licenses/mit-license.php or see LICENSE file.
# Copyright 2007-2008 Brisa Team <brisa-develop@garage.maemo.org>

""" Retrieves database paths.
"""

from os.path import join, exists
from brisa import config


def resolve_database(database_uri, database_name, protocol="sqlite"):
    """ Resolves the database path. If the database doesn't exist, returns
    "protocol:{brisa_home}/{database_name}".

    @param database_uri: database URI (protocol:/path/to/database.db)
    @param database_name: name of the database (database.db)
    @param protocol: database protocol (for example, sqlite)

    @type database_uri: string
    @type database_name: string
    @type protocol: string

    @return: database path
    @rtype: string
    """
    if ':' in database_uri:
        db_filesystem_path = database_uri.split(':')[1]
    else:
        db_filesystem_path = database_uri
    if not exists(db_filesystem_path):
        database_uri = join(config.brisa_home, database_name)
        database_uri = "%s:%s" % (protocol, database_uri)
    return db_filesystem_path
