#include "logger.h"

/// @deprecated
template<>
void Logger::getLogger(string filename)
{

    // very first thing, open up the logfile and mark that we got in
    // here.  If we can't open the logfile, we're dead.
    logfile = fopen(filename.c_str(), "w");
    if (logfile == NULL) {
        qFatal("Error creating logfile!");
        exit(EXIT_FAILURE);
    }

    // set logfile to line buffering
    setvbuf(logfile, NULL, _IOLBF, 0);
}

/// @deprecated
template<>
void Logger::superlog(const char *format, ...) const
{
    va_list ap;

    string formatx(format);
    formatx += "\n";

    va_start(ap, format);

    fflush(logfile);
    vfprintf(logfile, formatx.c_str(), ap);
    fflush(logfile);
}

/// @deprecated
template<>
void Logger::log(const char *format, ...) const
{
    va_list ap;

    string formatx("\t");
    formatx += format;
    formatx += "\n";

    va_start(ap, format);

    fflush(logfile);
    vfprintf(logfile, formatx.c_str(), ap);
    fflush(logfile);
}
