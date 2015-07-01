/*
* fusexx: a C++ wrapper for the FUSE API
* Copyright (C) 2007 John C. Matherly
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
* 02111-1307, USA.
*/

/*
 * Classe modificada por Wendell S. Soares
 *
 * Modificada por causa de problemas de compilação, causados
 * porque o nome original de muitos métodos da classe eram o mesmos nomes das
 * suas respectivas funcoes de bibliotecas que manipulam arquivos em
 * baixo nivel no linux.
 */

#ifndef FUSEXX_H_
#define FUSEXX_H_

#define FUSE_USE_VERSION 26 // earlier versions have deprecated functions

// C++ Headers
#include <string> // memset

// C Headers
#include <fuse.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace fusexx {

	/*
	* fuse
	* Manages all the fuse operations. A very simple interface to the C fuse_ops struct.
	*/
	template <class T>
	class fuse {
		/*
		* Public Methods
		*/
	public:
		/*
		* Main function of fusexx::fuse.
		* 
		* Calls the fuse 'fuse_main' macro.
		* 
		* @param argc the argument counter passed to the main() function
		* @param argv the argument vector passed to the main() function
		* @param user_data user data set in context for init() method
		* @return 0 on success, nonzero on failure
		*/
		static int main (int argc, char **argv, void *user_data, T *t) {
			// Zero the operations struct
			memset (&T::operations, 0, sizeof (struct fuse_operations));

			// Load the operations struct w/ pointers to the respective member functions
			T::loadOperations ();

			// The 'self' variable will be the equivalent of the 'this' pointer
			if (t == NULL)
				return -1;
			T::self = t;

			// Execute fuse_main
                        return fuse_main (argc, argv, &T::operations, user_data);
		}

		static struct fuse_operations operations;

		/*
		* Overload these functions
		*/
	public:
                static int fs_readlink (const char *, char *, size_t) { return 0; }
                static int fs_getattr (const char *, struct stat * ) { return 0; }
                static int fs_getdir (const char *, fuse_dirh_t, fuse_dirfil_t) { return 0; }
                static int fs_mknod (const char *, mode_t, dev_t) { return 0; }
                static int fs_mkdir (const char *, mode_t) { return 0; }
                static int fs_unlink (const char *) { return 0; }
                static int fs_rmdir (const char *) { return 0; }
                static int fs_symlink (const char *, const char *) { return 0; }
                static int fs_rename (const char *, const char *) { return 0; }
                static int fs_link (const char *, const char *) { return 0; }
                static int fs_chmod (const char *, mode_t) { return 0; }
                static int fs_chown (const char *, uid_t, gid_t) { return 0; }
                static int fs_truncate (const char *, off_t) { return 0; }
                static int fs_utime (const char *, struct utimbuf *) { return 0; }
                static int fs_open (const char *, struct fuse_file_info *) { return 0; }
                static int fs_read (const char *, char *, size_t, off_t, struct fuse_file_info *) { return 0; }
                static int fs_write (const char *, const char *, size_t, off_t,struct fuse_file_info *) { return 0; }
                static int fs_statfs (const char *, struct statvfs *) { return 0; }
                static int fs_flush (const char *, struct fuse_file_info *) { return 0; }
                static int fs_release (const char *, struct fuse_file_info *) { return 0; }
                static int fs_fsync (const char *, int, struct fuse_file_info *) { return 0; }
                static int fs_setxattr (const char *, const char *, const char *, size_t, int) { return 0; }
                static int fs_getxattr (const char *, const char *, char *, size_t) { return 0; }
                static int fs_listxattr (const char *, char *, size_t) { return 0; }
                static int fs_removexattr (const char *, const char *) { return 0; }
                /**
                       Deprecated functions in Fuse 2.8
                 */
                static int fs_readdir (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *) { return 0; }
                static int fs_opendir (const char *, struct fuse_file_info *) { return 0; }
                static int fs_releasedir (const char *, struct fuse_file_info *) { return 0; }
                static int fs_fsyncdir (const char *, int, struct fuse_file_info *) { return 0; }
                static void *init (struct fuse_conn_info *conn) { return NULL; }
                static void  destroy (void *) { }
                static int fs_access (const char *, int) { return 0; }
                static int fs_create (const char *, mode_t, struct fuse_file_info *) { return 0; }
                static int fs_ftruncate (const char *, off_t, struct fuse_file_info *) { return 0; }
                static int fs_fgetattr (const char *, struct stat *, struct fuse_file_info *) { return 0; }

		static void loadOperations () {
                        operations.readlink = T::fs_readlink;
                        operations.getattr = T::fs_getattr;
                        operations.getdir = T::fs_getdir;
                        operations.mknod = T::fs_mknod;
                        operations.mkdir = T::fs_mkdir;
                        operations.unlink = T::fs_unlink;
                        operations.rmdir = T::fs_rmdir;
                        operations.symlink = T::fs_symlink;
                        operations.rename = T::fs_rename;
                        operations.link = T::fs_link;
                        operations.chmod = T::fs_chmod;
                        operations.chown = T::fs_chown;
                        operations.truncate = T::fs_truncate;
                        operations.utime = T::fs_utime;
                        operations.open = T::fs_open;
                        operations.read = T::fs_read;
                        operations.write = T::fs_write;
                        operations.statfs = T::fs_statfs;
                        operations.flush = T::fs_flush;
                        operations.release = T::fs_release;
                        operations.fsync = T::fs_fsync;
                        operations.setxattr = T::fs_setxattr;
                        operations.getxattr = T::fs_getxattr;
                        operations.listxattr = T::fs_listxattr;
                        operations.removexattr = T::fs_removexattr;
                        /**
                            Deprecated functions in Fuse 2.8
                         */
                        operations.readdir = T::fs_readdir;
                        operations.opendir = T::fs_opendir;
                        operations.releasedir = T::fs_releasedir;
                        operations.fsyncdir = T::fs_fsyncdir;
                        operations.init = T::init;
                        operations.destroy = T::destroy;
                        operations.access = T::fs_access;
                        operations.create = T::fs_create;
                        operations.ftruncate = T::fs_ftruncate;
                        operations.fgetattr = T::fs_fgetattr;
		}

		/*
		* Protected variables
		*/
	protected:
		// allow static methods to access object methods/ variables using 'self' instead of 'this'
		static T *self;
	};

	template <class T> struct fuse_operations fuse<T> ::operations;
	template <class T> T * fuse<T> ::self;
}

#endif /*FUSEXX_H_*/
