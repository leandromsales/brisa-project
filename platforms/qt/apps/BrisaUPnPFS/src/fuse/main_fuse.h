#ifndef MAIN_FUSE_H
#define MAIN_FUSE_H

#define PROGRAM ("UPnP-FS")
#define PROGRAM_BIN ("upnpfs")
#define BRISA_BIN ("brisa_upnpfs")
#define VERSION ("0.0.1")

#define KEY_HELP (0)
#define KEY_VERSION (1)
#define KEY_NO_SEARCH (2)
#define KEY_ON_OPEN (3)
#define KEY_ON_SAVE (4)

static int process_arg(void*, const char*, int, struct fuse_args*);
void help();
//void usage();
void version();
void startControlPoint(const QString);

/**
 * Parameters for command-line argument processing function
 */
struct upnpfs_param {

    bool help;
    bool version;
    bool nosearch;
    bool onopen;
    int strArgCount;
    char *mountpoint;
};

#endif // MAIN_FUSE_H
