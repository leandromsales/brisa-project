#ifndef RES_H
#define RES_H

#include <QObject>
#include <QHash>

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {
namespace didl {

/*!
 * Class Res is an implementation of element res, that identifies a
 * Resource. The equivalent MPEG21 DIDL element is 'RESOURCE'.
 */
class Res : public QObject
{
    Q_OBJECT
public:
    explicit Res(QObject *parent = 0);
    explicit Res(const Res &);
    ~Res();
    Res & operator=(const Res &);

    /*!
     * Put value of QString in /a value.
     */
    void setValue(const QString &);
    /*!
     * Returns /a value.
     */
    QString getValue();

    /*!
     * Add a property to resource. This property will be save on QHash.
     * If arguments are empty strings, function returns false. Otherwise,
     * returns true.
     */
    bool addProperty(const QString &, const QString &);
    /*!
     * Return property called \param name.
     */
    QString getProperty(const QString &name);
    /*!
     * Return QHash contains all properties.
     */
    QHash<QString, QString> getProperties() const;

private:

    QString value;
    QHash<QString, QString> properties;

signals:

public slots:

};

}  // namespace didl
}  // namespace av
}
}
}

#endif // RES_H
