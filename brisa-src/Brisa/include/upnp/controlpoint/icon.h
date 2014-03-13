#ifndef ICON_H
#define ICON_H

#include <QObject>
#include <QUrl>
#include <QHash>

class QNetworkReply;
class QNetworkAccessManager;
class QTemporaryFile;

namespace brisa {
namespace upnp {
namespace controlpoint {

class Icon : public QObject
{
    Q_OBJECT
public:
    explicit Icon(QObject *parent = 0);
    explicit Icon(const QString &,
                  const QString &,
                  const QString &,
                  const QString &,
                  const QUrl &,
                  QObject *parent = 0);
    explicit Icon(const Icon &);
    ~Icon();
    Icon & operator=(const Icon &);

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
    void iconDownloaded(Icon *);
    void errorDownloadingIcon(Icon *, quint8);

public slots:
    void replyFinished(QNetworkReply *);

};

}
}
}

#endif // ICON_H
