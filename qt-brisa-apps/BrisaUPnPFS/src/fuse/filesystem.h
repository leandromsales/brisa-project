#ifndef fs_H
#define fs_H

#include <QFile>
#include "fusexx.hpp"
#include <fuse.h>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string>
#include <ulockmgr.h>

#include <sys/time.h>
#include <sys/xattr.h>

#include "dbusclient.h"

struct fs_state {
    char *rootdir;
    char *mountdir;
};

extern struct fs_state *fs_data;

#define FS_DATA ((struct fs_state *) fuse_get_context()->private_data)

using namespace std;

class FileSystem : public fusexx::fuse<FileSystem> {

public:

    FileSystem();

    static int fs_statfs(const char *path, struct statvfs *stbuf);
    static int fs_getattr(const char *path, struct stat *stbuf);

    static int fs_fgetattr(const char *path, struct stat *stbuf,
                               struct fuse_file_info *fi);

    static int fs_opendir(const char *path, struct fuse_file_info *fi);

    static int fs_readdir(const char *path, void *buf,
                              fuse_fill_dir_t filler,
                              off_t offset, struct fuse_file_info *fi);

    static int fs_releasedir(const char *path, struct fuse_file_info *fi);

    static int fs_create(const char *path, mode_t mode,
                             struct fuse_file_info *fi);

    static int fs_mknod(const char *path, mode_t mode, dev_t rdev);
    static int fs_open(const char *path, struct fuse_file_info *fi);

    static int fs_read(const char *path, char *buf, size_t size,
                           off_t offset, struct fuse_file_info *fi);

    static int fs_write(const char *path, const char *buf, size_t size,
                            off_t offset, struct fuse_file_info *fi);

    static int fs_release(const char *path, struct fuse_file_info *fi);
    static int fs_readlink(const char *path, char *buf, size_t size);
    static int fs_symlink(const char *from, const char *to);
    static int fs_link(const char *from, const char *to);
    static int fs_unlink(const char *path);
    static int fs_rename(const char *from, const char *to);
    static int fs_chmod(const char *path, mode_t mode);
    static int fs_chown(const char *path, uid_t uid, gid_t gid);
    static int fs_utimens(const char *path, const struct timespec ts[2]);
    static int fs_mkdir(const char *path, mode_t mode);
    static int fs_rmdir(const char *path);
    static int fs_access(const char *path, int mask);
    static int fs_truncate(const char *path, off_t size);

    static int fs_ftruncate(const char *path, off_t size,
                                struct fuse_file_info *fi);

    static int fs_setxattr(const char *path, const char *name,
                               const char *value, size_t size, int flags);

    static int fs_getxattr(const char *path, const char *name, char *value,
                               size_t size);

    static int fs_listxattr(const char *path, char *list, size_t size);
    static int fs_removexattr(const char *path, const char *name);

    static int fs_fsync(const char *path, int datasync,
                            struct fuse_file_info *fi);

    static int fs_flush(const char *path, struct fuse_file_info *fi);

    //Was not implemented because resulted in compilation errors

//        static int fs_lock(const char *path, struct fuse_file_info *fi,
//                                   int cmd, struct flock *lock);


    static void umounted();

private:
    static string fullPath(const char* path);
    static void logres(int);


};


#endif // fs_H
