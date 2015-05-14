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
#include <QIcon>

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

    /*!
     * Set attribute \param key with value of \param v.
     */
    void setAttribute(xmlIconTags key, QString v);
    /*!
     * Set variable icon with value of \param icon.
     */
    void setIcon(QIcon icon);
    /*!
     * Return a QIcon.
     */
    QIcon getIcon();
    /*!
     * Clear all variables.
     */
    void clear();
    /*!
     * Get value of attribute \param key.
     */
    QString getAttribute(xmlIconTags key);
    /*!
     * Download QIcon.
     */
    void downloadIcon(QString deviceUrl);

signals:
    /*!
     * This signal is emitted when download of icon is finished.
     */
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
