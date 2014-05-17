#ifndef ICON_H
#define ICON_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include <QtGui/QIcon>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QDebug>

namespace brisa {
namespace upnp {

class Icon: public QObject {
    Q_OBJECT
public:
    Icon(QString mimetype = "", QString width = "", QString height = "",
            QString depth = "", QString url = "", QObject *parent = 0);

    typedef enum {
        Mimetype, Width, Height, Depth, Url
    } xmlIconTags;

    void setAttribute(xmlIconTags key, QString v);
    void setIcon(QIcon icon);
    QIcon getIcon();
    void clear();
    QString getAttribute(xmlIconTags key);
    void downloadIcon(QString deviceUrl);

signals:
    void iconDownloadFinished();

public slots:
    void downloadFinished(QNetworkReply*);

private:
    QString mimetype;
    QString width;
    QString height;
    QString depth;
    QString url;
    QString attribute;
    QIcon icon;
};

}
}

#endif /* ICON_H_ */
