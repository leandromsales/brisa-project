#ifndef HTTPVERSION_H
#define HTTPVERSION_H

#include <QByteArray>
#include <QDebug>

#ifdef major
#undef major
#endif

#ifdef minor
#undef minor
#endif

namespace brisa {
namespace shared {
namespace webserver {
namespace http {

class HttpVersion
{
public:
    HttpVersion(int httpVersionMajor = 1, int httpVersionMinor = 1);
    // str must have the format "HTTP/1.1", or a HTTP 0.0 will be created
//    HttpVersion(const HttpVersion &) = default;
    HttpVersion(const QByteArray &str);

    int major() const;
    int minor() const;

    void setMajor(int);
    void setMinor(int);

//    bool operator ==(const HttpVersion &) = default;
    bool operator ==(const HttpVersion &);

    // TODO: fix
    bool operator ==(qreal) const;
    bool operator !=(qreal) const;


    bool operator >(const HttpVersion &o) const;
    bool operator <(const HttpVersion &o) const;
    bool operator >=(const HttpVersion &o) const;
    bool operator <=(const HttpVersion &o) const;
//    HttpVersion &operator =(qreal) = default;
    HttpVersion &operator =(qreal);
    operator QByteArray() const;
    operator bool() const;

private:
    int m_major;
    int m_minor;
};

}  // namespace http
}  // namespace webserver
}  // namespace shared
} // namespace Brisa

inline brisa::shared::webserver::http::HttpVersion::HttpVersion(int httpVersionMajor, int httpVersionMinor) :
        m_major(httpVersionMajor),
        m_minor(httpVersionMinor)
{
}

inline brisa::shared::webserver::http::HttpVersion::HttpVersion(const QByteArray &str) :
        m_major(0),
        m_minor(0)
{
    if (!str.startsWith("HTTP/"))
        return;
    int indexOfSlash = str.indexOf('/');
    if (indexOfSlash != -1) {
        int indexOfDot = str.indexOf('.', indexOfSlash);
        if (indexOfDot != -1) {
            bool ok[2];

            int tmp[] =
            {str.mid(indexOfSlash + 1, indexOfDot - indexOfSlash - 1).toInt(ok),
             str.mid(indexOfDot + 1).toInt(ok + 1)};

            if (ok[0] && ok[1]) {
                m_major = tmp[0];
                m_minor = tmp[1];
            }
        }
    }
}

inline brisa::shared::webserver::http::HttpVersion &brisa::shared::webserver::http::HttpVersion::operator =(qreal version)
{
    m_major = version;
    m_minor = static_cast<int>(version * 10) % 10;
    return *this;
}

inline int brisa::shared::webserver::http::HttpVersion::minor() const
{
    return m_minor;
}

inline int brisa::shared::webserver::http::HttpVersion::major() const
{
    return m_major;
}

inline bool brisa::shared::webserver::http::HttpVersion::operator ==(const HttpVersion &v)
{
    return m_major == v.m_major && m_minor == v.m_minor;
}

inline bool brisa::shared::webserver::http::HttpVersion::operator ==(qreal r) const
{
    qreal self = static_cast<qreal> (m_major) + (m_minor / 10.);
    return self == r;
}

inline bool brisa::shared::webserver::http::HttpVersion::operator !=(qreal r) const
{
    qreal self = static_cast<qreal> (m_major) + (m_minor / 10.);
    return self != r;
}

inline bool brisa::shared::webserver::http::HttpVersion::operator >(const HttpVersion &o) const
{
    if (m_major > o.m_major) {
        return true;
    } else if (m_major < o.m_major) {
        return false;
    } else {
        return m_minor > o.m_minor;
    }
}

inline bool brisa::shared::webserver::http::HttpVersion::operator <(const HttpVersion &o) const
{
    if (m_major < o.m_major) {
        return true;
    } else if (m_major > o.m_major) {
        return false;
    } else {
        return m_minor < o.m_minor;
    }
}

inline bool brisa::shared::webserver::http::HttpVersion::operator >=(const HttpVersion &o) const
{
    if (m_major > o.m_major) {
        return true;
    } else if (m_major < o.m_major) {
        return false;
    } else {
        return m_minor >= o.m_minor;
    }
}

inline bool brisa::shared::webserver::http::HttpVersion::operator <=(const HttpVersion &o) const
{
    if (m_major < o.m_major) {
        return true;
    } else if (m_major > o.m_major) {
        return false;
    } else {
        return m_minor <= o.m_minor;
    }
}

inline brisa::shared::webserver::http::HttpVersion::operator QByteArray() const
{
    QByteArray str;
    // "HTTP/x.x" // 8 chars
    str.reserve(8);
    str += "HTTP/";
    str += QByteArray::number(m_major);
    str += ".";
    str += QByteArray::number(m_minor);
    return str;
}

inline brisa::shared::webserver::http::HttpVersion::operator bool() const
{
    return m_major || m_minor;
}

inline QDebug &operator<<(QDebug dbg, const brisa::shared::webserver::http::HttpVersion &version)
{
    dbg.nospace() << "HTTP/" << version.major() << '.' << version.minor();
    return dbg.maybeSpace();
}

#endif // HTTPVERSION_H
