#include "filesystem.h"

#include <QDebug>
#include <QDir>

#include <iostream>
#include <fstream>

#include "../common/logger.h"
#include "main_fuse.h"

using namespace std;

extern DBusClient *client;
extern ofstream *out;
extern struct upnpfs_param param;

Logger *logger_ptr;

#define logger (*logger_ptr)

FileSystem::FileSystem()
{
    logger_ptr = new Logger(*out);
}

/**
 *
 */
void FileSystem::umounted()
{
    if(client!=NULL) {
        client->fsUmounted();
        time_t fim = time(0);
        logger << "\n\n===== UPnP-FS Finished at " << ctime(&fim) << endl;
    }
}

/**
 * Returns the full file path
 *  All the paths I see are relative to the root of the mounted
 *  filesystem.  In order to get to the underlying filesystem, I need to
 *  have the mountpoint.  I'll save it away early on in main(), and then
 *  whenever I need a path for something I'll call this to construct
 *  it.
*/
string FileSystem::fullPath(const char* path)
{
    string fpath;
    fpath += fs_data->rootdir;
    fpath += path;

    return fpath;
}

/**
 * Log a number
 */
void FileSystem::logres(int res)
{
    logger << "\tres: " << res << endl;
}


/** Get file system statistics
 *
 * The 'f_frsize', 'f_favail', 'f_fsid' and 'f_flag' fields are ignored
 *
 * Replaced 'struct statfs' parameter with 'struct statvfs' in
 * version 2.5
 */
int FileSystem::fs_statfs(const char *path, struct statvfs *stbuf)
{
    logger << "upnpfs_statfs" << endl;
    int res;
    string fpath = fullPath(path);
    logger << "\tstatvfs (" << fpath << ", stbuf)" << endl;
    res = statvfs(fpath.c_str(), stbuf);
    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Get file attributes.
 * Similar to stat().  The 'st_dev' and 'st_blksize' fields are
 * ignored.  The 'st_ino' field is ignored except if the 'use_ino'
 * mount option is given.
 */
int FileSystem::fs_getattr(const char *path, struct stat *stbuf)
{
    logger << "upnpfs_getattr" << endl;
    int res = 0;
    string fpath = fullPath(path);

    logger << "\tlstat (" << fpath << ", stbuf)" << endl;
    res = lstat(fpath.c_str(), stbuf);
    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Get attributes from an open file
 *
 * This method is called instead of the getattr() method if the
 * file information is available.
 *
 * Currently this is only called after the create() method if that
 * is implemented (see above).  Later it may be called for
 * invocations of fstat() too.
 */
int FileSystem::fs_fgetattr(const char *path, struct stat *stbuf,
                            struct fuse_file_info *fi)
{
    logger << "upnpfs_fgetattr" << endl;
    int res;
    (void) path;

    logger << "\tfstat(" << fi->fh << ", " << stbuf << ")" << endl;
    res = fstat(fi->fh, stbuf);
    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Open directory
 *
 * This method should check if the open operation is permitted for
 * this  directory
 */
int FileSystem::fs_opendir(const char *path, struct fuse_file_info *fi)
{
    logger << "upnpfs_opendir" << endl;
    string fpath = fullPath(path);

    logger << "\topendir(" << fpath << ")" << endl;
    DIR *dp = opendir(fpath.c_str());

    if (dp == NULL)
        return -errno;
    fi->fh = (unsigned long) dp;
    return 0;
}

/**
 * Read directory
 *
 * This supersedes the old getdir() interface.  New applications
 * should use this.
 *
 * The filesystem may choose between two modes of operation:
 *
 * 1) The readdir implementation ignores the offset parameter, and
 * passes zero to the filler function's offset.  The filler
 * function will not return '1' (unless an error happens), so the
 * whole directory is read in a single readdir operation.  This
 * works just like the old getdir() method.
 *
 * 2) The readdir implementation keeps track of the offsets of the
 * directory entries.  It uses the offset parameter and always
 * passes non-zero offset to the filler function.  When the buffer
 * is full (or an error happens) the filler function will return
 * '1'.
 */
#define get_dirp(f) ((DIR *) (uintptr_t) f->fh)
int FileSystem::fs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                           off_t offset, struct fuse_file_info *fi)
{

    logger << "upnpfs_readdir" << endl;
    logger << "\tgetdirp(fi)" << endl;
    DIR *dp = get_dirp(fi);
    struct dirent *de;
    struct stat st;
    seekdir(dp, offset);
    while ((de = readdir(dp)) != NULL) {
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        if (filler(buf, de->d_name, &st, telldir(dp)))
            break;
    }
    return 0;
}


/**
 * Release directory
 */
int FileSystem::fs_releasedir(const char *path, struct fuse_file_info *fi)
{
    logger << "upnpfs_releasedir" << endl;
    logger << "\tgetdirp(fi)" << endl;
    DIR *dp = get_dirp(fi);
    (void) path;
    closedir(dp);
    return 0;
}

/**
 * Create and open a file
 *
 * If the file does not exist, first create it with the specified
 * mode, and then open it.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the mknod() and open() methods
 * will be called instead.
 */
int FileSystem::fs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    logger << "upnpfs_create" << endl;

    string fpath = fullPath(path);

    logger << "\topen(" << fpath
           << ", " << fi->flags << ", " << mode << ")" << endl;
    int fd = open(fpath.c_str(), fi->flags, mode);
    if (fd == -1)
        return -errno;
    fi->fh = fd; // prover um descritor de arquivos é OPCIONAL
    return 0;
}

/**
 * Create a file node
 * There is no create() operation, mknod() will be called for
 * creation of all non-directory, non-symlink nodes.
 */
int FileSystem::fs_mknod(const char *path, mode_t mode, dev_t rdev)
{
    logger << "upnpfs_mknod" << endl;

    int res;
    string fpath = fullPath(path);

    logger << "\tmknod(" << fpath << ")" << endl;

    if (S_ISFIFO(mode))
        res = mkfifo(fpath.c_str(), mode);
    else
        res = mknod(fpath.c_str(), mode, rdev);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * File open operation
 *
 * No creation, or truncation flags (O_CREAT, O_EXCL, O_TRUNC)
 * will be passed to open().  Open should check if the operation
 * is permitted for the given flags.  Optionally open may also
 * return an arbitrary filehandle in the fuse_file_info structure,
 * which will be passed to all file operations.
 */
int FileSystem::fs_open(const char *path, struct fuse_file_info *fi)
{
    logger << "upnpfs_open" << endl;
    string fpath = fullPath(path);

    //Chama o cliente DBUS e avisa que o arquivo foi aberto.
    //O cliente DBUS avisara ao Brisa que o arquivo foi aberto.
    if((client != NULL) && param.onopen) {
        logger << "\tDBUS: client->fileOpen(" << path << ")" << endl;
        client->fileOpen(path);
    }

    logger << "\topen(" << fpath << ", " << fi->flags << ")" << endl;

    int fd = open(fpath.c_str(), fi->flags); // sem mode
    if (fd == -1)
        return -errno;
    fi->fh = fd; // fornecer um file handle é OPCIONAL

    return 0;
}

/** Read data from an open file
 *
 * Read should return exactly the number of bytes requested except
 * on EOF or error, otherwise the rest of the data will be
 * substituted with zeroes.  An exception to this is when the
 * 'direct_io' mount option is specified, in which case the return
 * value of the read system call will reflect the return value of
 * this operation.
 */
int FileSystem::fs_read(const char *path, char *buf, size_t size, off_t offset,
                        struct fuse_file_info *fi)
{
    logger << "upnpfs_read" << endl;

    //Chama o cliente DBUS e avisa que o arquivo foi aberto.
    //O cliente DBUS avisara ao Brisa que o arquivo foi aberto.
    if((client != NULL) && param.onopen) {
        logger << "\tDBUS: client->fileOpen(" << path << ")" << endl;
        client->fileOpen(path);
    }

    int res;
    (void) path;
    logger << "\tpread(" << fi->fh << ", buf, "
           << size << ", " << offset << ")" << endl;
    res = pread(fi->fh, buf, size, offset);
    logres(res);
    if (res == -1)
        res = -errno;
    return res;
}

/**
 * Write data to an open file
 *
 * Write should return exactly the number of bytes requested
 * except on error.  An exception to this is when the 'direct_io'
 * mount option is specified (see read operation).
 */
// Notifies changes to Control Point.
int FileSystem::fs_write(const char *path, const char *buf, size_t size,
                         off_t offset, struct fuse_file_info *fi)
{
    logger << "<<<<<<<<<< ! upnpfs_write ! >>>>>>>>>>" << endl;
    int res;
    (void) path;

    //Chama o cliente DBUS e avisa que o arquivo foi modificado.
    //O cliente DBUS avisara ao Brisa que o arquivo foi modificado.
    if(client != NULL) {
        logger << "\tclient->fileWrite(" << path << ")" << endl;
        client->fileWrite(path);
    }

    logger << "\tpwrite(" << fi->fh << ", " << buf
           << ", " <<  size << ", " <<  offset;
    res = pwrite(fi->fh, buf, size, offset);

    logres(res);
    if (res == -1)
        res = -errno;
    return res;
}

/**
 * Release an open file
 *
 * Release is called when there are no more references to an open
 * file: all file descriptors are closed and all memory mappings
 * are unmapped.
 *
 * For every open() call there will be exactly one release() call
 * with the same flags and file descriptor.  It is possible to
 * have a file opened more than once, in which case only the last
 * release will mean, that no more reads/writes will happen on the
 * file.  The return value of release is ignored.
 */
int FileSystem::fs_release(const char *path, struct fuse_file_info *fi)
{
    logger << "upnpfs_release" << endl;
    (void) path;
    logger << "\tclose(" << fi->fh << ")" << endl;
    close(fi->fh);
    return 0;
}

/**
 * Read the target of a symbolic link
 *
 * The buffer should be filled with a null terminated string.  The
 * buffer size argument includes the space for the terminating
 * null character.  If the linkname is too long to fit in the
 * buffer, it should be truncated.  The return value should be 0
 * for success.
 */
// the description given above doesn't correspond to the readlink(2)
// man page -- according to that, if the link is too long for the
// buffer, it ends up without the null termination
int FileSystem::fs_readlink(const char *path, char *buf, size_t size)
{
  logger << "upnpfs_readlink" << endl;
    string fpath = fullPath(path);
    logger << "\treadlink(" << fpath << ", "
           << buf << ", " <<  size-1 << ")" << endl;
    int res = readlink(fpath.c_str(), buf, size - 1);

    logres(res);
    if (res == -1)
        return -errno;
    buf[res] = '\0';
    return 0;
}

/**
 * Create a symbolic link
 */
int FileSystem::fs_symlink(const char *from, const char *to)
{
  logger << "upnpfs_symlink" << endl;
  logger << "\tsymlink(" <<  from << ", " <<  to << ")" << endl;
    int res = symlink(from, to);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
* Create a hard link to a file
*/
int FileSystem::fs_link(const char *from, const char *to)
{
  logger << "upnpfs_link" << endl;
  logger << "\tlink(" <<  from << ", " <<  to << ")" << endl;
    int res = link(from, to);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}


/**
 * Remove a file
 */
int FileSystem::fs_unlink(const char *path)
{

   logger << "upnpfs_unlink" << endl;
    string fpath = fullPath(path);
    logger << "\tunlink(" <<  fpath << ")" << endl;
    int res;
    res = unlink(fpath.c_str());

    logres(res);
    if (res == -1)
        return -errno;

    return 0;
}

/**
 * Rename a file
 */
// both path and newpath are fs-relative
//OBS: 'from' and 'to' both always in myfs
int FileSystem::fs_rename(const char *from, const char *to)
{
    logger << "<<<<<upnpfs_rename>>>" << endl;
    int res;

    string fullPathFrom = fullPath(from);
    string fullPathTo = fullPath(to);

    logger << "\trename(" <<  from << ", " <<  to << ")" << endl;
    res = rename(fullPathFrom.c_str(), fullPathTo.c_str());

    //Chama o cliente DBUS e avisa que o arquivo foi modificado.
    //O cliente DBUS avisara ao Brisa que o arquivo foi modificado.
    if(client != NULL) {
        logger << "\tclient->fileWrite(" << to << ")" << endl;
        client->fileWrite(to);
    }

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Change the permission bits of a file
 */
int FileSystem::fs_chmod(const char *path, mode_t mode)
{
   logger << "upnpfs_chmod" << endl;

    string fpath = fullPath(path);
    logger << "\tchmod(" <<  fpath << ", " << mode << ")" << endl;
    int res = chmod(fpath.c_str(), mode);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Change the owner and group of a file
 */
int FileSystem::fs_chown(const char *path, uid_t uid, gid_t gid)
{
    logger << "upnpfs_chown" << endl;
    string fpath = fullPath(path);
    logger << "\tlchow(" << fpath << ", "
           << uid << ", " << gid << ")" << endl;
    int res = lchown(fpath.c_str(), uid, gid);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

//Muda atributos de data/hora com precisão de nanosegundos
int FileSystem::fs_utimens(const char *path, const struct timespec ts[2])
{
    logger << "upnpfs_opentimens" << endl;
    int res;
    struct timeval tv[2];
    tv[0].tv_sec = ts[0].tv_sec;
    tv[0].tv_usec = ts[0].tv_nsec / 1000;
    tv[1].tv_sec = ts[1].tv_sec;
    tv[1].tv_usec = ts[1].tv_nsec / 1000;

    string fpath = fullPath(path);
    logger << "\tutimes(" <<  fpath << ", " << tv << ")" << endl;
    res = utimes(fpath.c_str(), tv);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Create a directory
 */
int FileSystem::fs_mkdir(const char *path, mode_t mode)
{
    logger << "upnpfs_mkdir" << endl;
    string fpath = fullPath(path);
    logger << "\tmkdir(" << fpath << ", " << mode << ")" << endl;
    int res = mkdir(fpath.c_str(), mode);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}


/**
 * Remove a directory
 */
int FileSystem::fs_rmdir(const char *path)
{
    logger << "upnpfs_rmdir" << endl;
    string fpath = fullPath(path);
    logger << "\trmdir(" << fpath << ")" << endl;
    int res = rmdir(fpath.c_str());

    logres(res);
    if (res == -1)
        return -errno;
     return 0;
}

/**
 * Check file access permissions
 *
 * This will be called for the access() system call.  If the
 * 'default_permissions' mount option is given, this method is not
 * called.
 *
 * This method is not called under Linux kernel versions 2.4.x
 */
int FileSystem::fs_access(const char *path, int mask)
{
    logger << "upnpfs_access" << endl;
    int res;
    string fpath = fullPath(path);
    logger << "\taccess(" << fpath << ", " << mask << ")"<< endl;
    res = access(fpath.c_str(), mask);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Change the size of a file
 */
int FileSystem::fs_truncate(const char *path, off_t size)
{

    logger << "upnpfs_truncate" << endl;
    string fpath = fullPath(path);
     logger << "\ttruncate(" << fpath << ", " << size << ")"<< endl;
    int res = truncate(fpath.c_str(), size);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Change the size of an open file
 *
 * This method is called instead of the truncate() method if the
 * truncation was invoked from an ftruncate() system call.
 *
 * If this method is not implemented or under Linux kernel
 * versions earlier than 2.6.15, the truncate() method will be
 * called instead.
 */
int FileSystem::fs_ftruncate(const char *path, off_t size,
                             struct fuse_file_info *fi)
{
    logger << "upnpfs_ftruncate" << endl;
    logger << "\tftruncate(" << fi->fh << ", " << size << ")"<< endl;
    int res = ftruncate(fi->fh, size);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
* Set extended attributes
*/
int FileSystem::fs_setxattr(const char *path, const char *name, const char *value,
                            size_t size, int flags)
{
    logger << "upnpfs_setxattr" << endl;
    string fpath = fullPath(path);
    logger << "\tlsetxattr(" << fpath << ", " << name << ", "
           << value << ", " << size << ", " << flags << ")" << endl;
    int res = lsetxattr(fpath.c_str(), name, value, size, flags);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
* Get extended attributes
*/
int FileSystem::fs_getxattr(const char *path, const char *name, char *value,
                            size_t size)
{
    logger << "upnpfs_getxattr" << endl;
    string fpath = fullPath(path);
    logger << "\tlgetxattr(" << fpath << ", " << name
           << ", value, " << size << ")" << endl;
    int res = lgetxattr(fpath.c_str(), name, value, size);

    logres(res);
    if (res == -1)
        return -errno;
    return res;
}

/**
* List extended attributes
*/
int FileSystem::fs_listxattr(const char *path, char *list, size_t size)
{
   logger << "upnpfs_listxattr" << endl;
    string fpath = fullPath(path);
    logger << "\tlistxattr(" << fpath << ", "
           << list << ", "<< size << ")" << endl;
    int res = llistxattr(fpath.c_str(), list, size);

    logres(res);
    if (res == -1)
        return -errno;
    return res;
}

/**
*  Remove extended attributes
*/
int FileSystem::fs_removexattr(const char *path, const char *name)
{
    logger << "upnpfs_removexattr" << endl;
    string fpath = fullPath(path);
    logger << "\tremovexattr(" << fpath << ", " << name << ")" << endl;
    int res = lremovexattr(fpath.c_str(), name);

    logres(res);
    if (res == -1)
        return -errno;
    return 0;
}

/**
 * Synchronize file contents
 *
 * If the datasync parameter is non-zero, then only the user data
 * should be flushed, not the meta data.
 */
int FileSystem::fs_fsync(const char *path, int datasync,
                         struct fuse_file_info *fi)
{
    logger << "upnpfs_fsync" << endl;
    (void) path;
    (void) fi;

    int res;

#ifndef HAVE_FDATASYNC
    (void) datasync;
#else
    if (isdatasync)
        res = fdatasync(fi->fh);
    else
#endif
        res = fsync(fi->fh);

    logres(res);
    if (res == -1)
        return -errno;

    return 0;
}

/**
 * Possibly flush cached data
 *
 * BIG NOTE: This is not equivalent to fsync().  It's not a
 * request to sync dirty data.
 *
 * Flush is called on each close() of a file descriptor.  So if a
 * filesystem wants to return write errors in close() and the file
 * has cached dirty data, this is a good place to write back data
 * and return any errors.  Since many applications ignore close()
 * errors this is not always useful.
 *
 * NOTE: The flush() method may be called more than once for each
 * open().  This happens if more than one file descriptor refers
 * to an opened file due to dup(), dup2() or fork() calls.  It is
 * not possible to determine if a flush is final, so each flush
 * should be treated equally.  Multiple write-flush sequences are
 * relatively rare, so this shouldn't be a problem.
 *
 * Filesystems shouldn't assume that flush will always be called
 * after some writes, or that if will be called at all.
 *
 */
int FileSystem::fs_flush(const char *path, struct fuse_file_info *fi)
{
    /* This is called from every close on an open file, so call the
               close on the underlying filesystem.	But since flush may be
               called multiple times for an open file, this must not really
               close the file.  This is important if used on a network
               filesystem like NFS which flush the data/metadata on close()
 */

  logger << "upnpfs_flush" << endl;
    (void) path;
    (void) fi;
    return (0);
}

//Was not implemented because resulted in compilation errors:

//Gerencia trava de arquivo, segundo o padrão POSIX
//int FileSystem::fs_lock(const char *path, struct fuse_file_info *fi, int cmd,
//                        struct flock *lock)
//{
//    (void) path;
//    return ulockmgr_op(fi->fh, cmd, lock, /*(const void *)*/ &fi->lock_owner,
//                       sizeof(fi->lock_owner));
//}
