#ifndef DEVICESALES_H
#define DEVICESALES_H

#include <QObject>

#include "iconsales.h"
#include "servicesales.h"

class QHttp;
class QNetworkReply;
class QNetworkRequest;
class QNetworkAccessManager;
class QTemporaryFile;

namespace brisa {
namespace upnp {
namespace controlpoint {

class DeviceSales : public QObject
{
    Q_OBJECT
public:
    explicit DeviceSales(QObject *parent = 0);
    explicit DeviceSales(const QString &,
                    const QString &,
                    QObject *parent = 0);
    explicit DeviceSales(const DeviceSales &);
    ~DeviceSales();
    DeviceSales & operator=(const DeviceSales &);

    void setBaseUrl(const QUrl &);
    QUrl getBaseUrl() const;

    void parseDescription();

    /*!
     * Add an attribute and your value in QHash \a attributes.
     */
    bool setAttribute(const QString &, const QString &);
    /*!
     * Return value of \param attributeName.
     */
    QString getAttribute(const QString &) const;
    /*!
     * Return a QHash contains all attributes.
     */
    QHash<QString, QString> getAttributes() const;

    /*!
     * Return a QHash contains all services.
     */
    QHash<QString, ServiceSales *> getServices() const;
    /*!
     * Return service with name equal of param.
     */
    ServiceSales *getService(const QString &) const;

    /*!
     * Return a QList contains all devices.
     */
    QList<DeviceSales *> getDevices() const;

    /*!
     * Return a QHash contains all icons.
     */
    QHash<QString, IconSales *> getIcons() const;
    /*!
     * Return icon with name equal of param.
     */
    IconSales *getIcon(const QString &);

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
    QHash<QString, IconSales*> iconList;
    QHash<QString, ServiceSales*> serviceList;
    QList<DeviceSales*> embeddedDeviceList;

    QNetworkAccessManager *downloader;

    uint serviceProcessedCounter;
    uint serviceCount;

signals:
    void deviceDescriptionReady(DeviceSales *);
    void errorParsingDeviceDescription(DeviceSales *, quint8);

public slots:
    void replyFinished(QNetworkReply *);
    void handleServiceDescriptionReady(ServiceSales *);
    void handleErrorParsingServiceDescription(ServiceSales *, quint8);
    void handleIconDownloaded(IconSales *);
    void handleErrorDownloadingIcon(IconSales *, quint8);
    //void handleExecutionActionReply(QString, QHash<QString, ActionArgument *>);
};

}
}
}

#endif // DEVICESALES_H
