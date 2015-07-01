//============================================================================
// Name        : UPNPFS.cpp
// Author      : Wendell Silva Soares
// Version     :
// Copyright   :
// Description : UPNP file system
//============================================================================

#include <QtGui/QApplication>
#include <QProcess>
#include <QDebug>
#include <QDir>

#include <iostream>
#include <fstream>
#include <cstdio>

#include "../common/logger.h"
#include "filesystem.h"

#include <fuse_opt.h>

#include "main_fuse.h"

using namespace std;

struct upnpfs_param param; //GLOBAL para ser usado em Filesystem

static const struct fuse_opt upnpfs_opts[] = {

    FUSE_OPT_KEY("-h",          KEY_HELP),
    FUSE_OPT_KEY("--help",      KEY_HELP),
    FUSE_OPT_KEY("-V",          KEY_VERSION),
    FUSE_OPT_KEY("--version",   KEY_VERSION),
    FUSE_OPT_KEY("-n",  KEY_NO_SEARCH),
    FUSE_OPT_KEY("--nosearch",  KEY_NO_SEARCH),
    FUSE_OPT_KEY("--onOpen",  KEY_ON_OPEN),
    FUSE_OPT_KEY("--onlyOnSave",  KEY_ON_SAVE),

    FUSE_OPT_END
};

//This needs to be global. Cannot be placed in the main function
DBusClient *client;
ofstream *out;
struct fs_state *fs_data;

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    //Here I report the Fuse the path of the root directory of the file system
    QDir rootDir(QDir::home());
    rootDir.mkdir(".upnpfs");
    rootDir.cd(".upnpfs");
    rootDir.mkdir(".debug");

    out = new ofstream(rootDir.absolutePath().append(DEFAULT_FUSE_LOG).toAscii());
    if( !(*out) )
    {
        qFatal("The log file can not be opened!");
    }
    Logger log(*out);

    fs_data = new struct fs_state;
    if (fs_data == NULL) {
        qFatal("Error on create fs_state");
        abort();
    }

    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    //struct upnpfs_param param;  --GLOBAL
    param.help = false;
    param.version = false;
    param.nosearch = false;
    param.onopen = true;
    param.strArgCount = 0;
    param.mountpoint = "";

    //Checks the arguments from the command line
    if (fuse_opt_parse(&args, &param, upnpfs_opts, process_arg)) {
        fuse_opt_free_args(&args);
        //usage();
        return EXIT_FAILURE;
    }

    if(param.version == false && param.help == false && param.strArgCount) {
        if(param.nosearch == false)
        {
            log << "Starting UPnP-FS with BRisa Control Point" << endl;

            //startControlPoint("brisa_upnpfs");

            if(param.onopen == true)
                system("brisa_upnpfs --onOpen > /dev/null 2> /dev/null &");
            else
                system("brisa_upnpfs --onlyOnSave > /dev/null 2> /dev/null &");

        } else {
            log << "Starting UPnP-FS --without-- BRisa Control Point" << endl;
        }

        ///////////////////////
        int sz = rootDir.absolutePath().size();
        fs_data->rootdir = new char[sz+1];
        for (int j=0; j<sz; ++j) {
            fs_data->rootdir[j] = rootDir.absolutePath().at(j).toAscii();
        }
        fs_data->rootdir[sz] = '\0';
        fs_data->mountdir = realpath(argv[1], NULL);

        time_t inicio = time(0);
        log << "===== Starting UPnP-FS at " << ctime(&inicio) << endl << endl;

        client = new DBusClient;
        log << "Conecting DBus..." << endl;
        log << "Mounting filesytem at" << param.mountpoint << endl;
    }

    FileSystem fs;
    FileSystem::main(args.argc, args.argv, NULL, &fs);
    fuse_opt_free_args(&args);

    //=================================
    //ACTIONS PERFORMED AFTER umount:
    //Notify control point
    fs.umounted();

    //Kill the process
    exit(0);

    return a.exec();
}

/**
 * Function to process arguments (called from fuse_opt_parse).
 *
 * @param data  Pointer to upnpfs_param structure
 * @param arg is the whole argument or option
 * @param key determines why the processing function was called
 * @param outargs the current output argument list
 * @return -1 on error, 0 if arg is to be discarded, 1 if arg should be kept
 */
static int process_arg(void *data, const char *arg, int key,
                       struct fuse_args *outargs) {

    struct upnpfs_param *param = (upnpfs_param*)data;

    (void)outargs;

    // 'magic' fuse_opt_proc return codes
    const static int KEEP = 1;
    const static int DISCARD = 0;
    //const static int ERROR = -1;

    switch (key) {

    case KEY_HELP: {
        help();
        param->help = true;
        return KEEP;
    }

    case KEY_VERSION: {
        version();
        param->version = true;
        return KEEP;
    }

    case KEY_NO_SEARCH: {
        param->nosearch = true;
        return DISCARD;
    }

    case KEY_ON_OPEN: {
        param->onopen = true;
        qDebug() << "onOpen = True";
        return DISCARD;
    }

    case KEY_ON_SAVE: {
        qDebug() << "onOpen = False";
        qDebug() << "onlyOnSave = True";
        param->onopen = false;
        return DISCARD;
    }

    default: {
        ++param->strArgCount;
        param->mountpoint = new char[strlen(arg)+1];
        strcpy(param->mountpoint, arg);
        // mountpoint
        // keep it and then pass to FUSE initializer
        return KEEP;
    }
    }
}

//////////////////////////////////////////////
void help()
{
    version();
    //usage();

    qDebug() << "\nUPnP-FS options:";
    qDebug() << "    -n   --nosearch        Disable automatic device search\n\n";

    qDebug() << "    --onOpen               If a service does not require parameters,\n"
             << "                          UPnP-FS invokes the service when you open the file.\n"
             << "                          Don't works if --nosearch option is enabled.\n"
             << "                          Default option.\n";

    qDebug() << "    --onlyOnSave            Invokes services only if you write to the file.\n"
             << "                          Don't works if --nosearch option is enabled.\n";
}

/** Alguma funcao fantasma estah tomando o lugar desta...
void usage()
{
    qDebug() << "usage (WENDELL): " <<  PROGRAM_BIN << "mountpoint [options]\n";
}
*/


void version()
{
    qDebug() << PROGRAM << "version:" << VERSION;
}

/**
* Deprecated. Bug: 'upnpfs' process is still alive
*/
void startControlPoint(const QString cp)
{
    QString program = cp;
    QProcess *brisa = new QProcess();
    brisa->start(program);
}
