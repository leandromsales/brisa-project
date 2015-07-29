#ifndef LOGGER_H
#define LOGGER_H

#include <QDebug>
#include <QString>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <typeinfo>

#define DEFAULT_MAIN_LOG "/.debug/upnpfs.log"
#define DEFAULT_FUSE_LOG "/.debug/fuse_upnpfs.log"
#define DEFAULT_BRISA_LOG "/.debug/brisa_upnpfs.log"

using namespace std;

template<typename Char, typename Traits = char_traits<Char> >
class logger_stream {

    typedef std::basic_ostream<Char, Traits> ostream_type;
    typedef ostream_type& (*manip_type)(ostream_type&);

public:
    logger_stream(ostream_type& os) :
        os(os) {
    }

    logger_stream &operator<<(manip_type pfn) {
        os << pfn;
        //if(pfn != static_cast<manip_type> (std::endl))
           // qDebug() << pfn;
        return *this;
    }

    template<typename T>
    logger_stream &operator<<(T const& t) {
        os << t;
        qDebug() << t;
        return *this;
    }

    logger_stream &operator<<(string const& t) {
        os << t;
        qDebug() << t.c_str();
        return *this;
    }

    logger_stream &operator<<(QString const& t) {
        os << t.toStdString();
        qDebug() << t;
        return *this;
    }

    /// To be compatible with the FUSE ///
    /// @deprecated
    void getLogger(string filename);
    void superlog(const char *format, ...) const;
    void log(const char *format, ...) const;


private:
    ostream_type &os;
    FILE *logfile; //to be compatible with the FUSE

};

typedef logger_stream<char> Logger;


#endif // LOGGER_H
