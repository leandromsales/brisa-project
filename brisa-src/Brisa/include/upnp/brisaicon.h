#ifndef BRISAICON_H
#define BRISAICON_H

#include <QObject>
#include <QString>
#include <QMetaType>
#include <QtGui/QIcon>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkReply>
#include <QDebug>

namespace Brisa {

class BrisaIcon: public QObject {
    Q_OBJECT
public:
    BrisaIcon(QString mimetype = "", QString width = "", QString height = "",
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

#endif /* BRISAICON_H_ */
