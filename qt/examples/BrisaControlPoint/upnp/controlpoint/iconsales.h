#ifndef ICONSALES_H
#define ICONSALES_H

#include <QObject>
#include <QUrl>
#include <QHash>

class QNetworkReply;
class QNetworkAccessManager;
class QTemporaryFile;

namespace brisa {
namespace upnp {
namespace controlpoint {

class IconSales : public QObject
{
    Q_OBJECT
public:
    explicit IconSales(QObject *parent = 0);
    explicit IconSales(const QString &,
                  const QString &,
                  const QString &,
                  const QString &,
                  const QUrl &,
                  QObject *parent = 0);
    explicit IconSales(const IconSales &);
    ~IconSales();
    IconSales & operator=(const IconSales &);

    void setBaseUrl(const QUrl &);
    QUrl getBaseUrl();
    bool downloadIt();
    QUrl getUrl();
    QTemporaryFile *getContent();

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &);
    QHash<QString, QString> getAttributes() const;

    typedef enum {
        CREATE_TMP_ICON_ERROR = 100
    } errorType;

private:
    QUrl baseUrl;
    QTemporaryFile *content;
    QHash<QString, QString> attributes;

    QNetworkAccessManager *downloader;

signals:
    void iconDownloaded(IconSales *);
    void errorDownloadingIcon(IconSales *, quint8);

public slots:
    void replyFinished(QNetworkReply *);

};

}
}
}

#endif // ICONSALES_H
