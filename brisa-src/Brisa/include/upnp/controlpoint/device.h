#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>

#include "icon.h"
#include "service.h"

class QHttp;
class QNetworkReply;
class QNetworkRequest;
class QNetworkAccessManager;
class QTemporaryFile;

namespace brisa {
namespace upnp {
namespace controlpoint {

class Device : public QObject
{
    Q_OBJECT
public:
    explicit Device(QObject *parent = 0);
    explicit Device(const QString &,
                    const QString &,
                    QObject *parent = 0);
    explicit Device(const Device &);
    ~Device();
    Device & operator=(const Device &);

    void setBaseUrl(const QUrl &);
    QUrl getBaseUrl() const;

    void parseDescription();

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &) const;
    QHash<QString, QString> getAttributes() const;

    QHash<QString, Service *> getServices() const;
    Service *getService(const QString &) const;

    QList<Device *> getDevices() const;

    QHash<QString, Icon *> getIcons() const;
    Icon *getIcon(const QString &);

    quint8 parseXMLDescription(QTemporaryFile *description);

    typedef enum {
        CREATE_TMP_XML_ERROR = 100,
        WRONG_MAJOR_MINOR_VERSION_ERROR,
        MALFORMED_XML_DESCRIPTION_ERROR,
        INCOMPLETED_DEVICE_DESCRIPTION_ERROR
    } errorType;

private:

    QUrl baseUrl;

    QHash<QString, QString> attributes;
    QHash<QString, Icon*> iconList;
    QHash<QString, Service*> serviceList;
    QList<Device*> embeddedDeviceList;

    QNetworkAccessManager *downloader;

    uint serviceProcessedCounter;
    uint serviceCount;

signals:
    void deviceDescriptionReady(Device *);
    void errorParsingDeviceDescription(Device *, quint8);

public slots:
    void replyFinished(QNetworkReply *);
    void handleServiceDescriptionReady(Service *);
    void handleErrorParsingServiceDescription(Service *, quint8);
    void handleIconDownloaded(Icon *);
    void handleErrorDownloadingIcon(Icon *, quint8);
    //void handleExecutionActionReply(QString, QHash<QString, ActionArgument *>);
};

}
}
}

#endif // DEVICE_H
