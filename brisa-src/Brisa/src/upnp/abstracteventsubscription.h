#ifndef ABSTRACTEVENTSUBSCRIPTION_H
#define ABSTRACTEVENTSUBSCRIPTION_H

#include <QDateTime>
#include <QList>
#include <QString>
#include <QObject>
#include <QtCore>

#include "../shared/webserver/webserver.h"
#include "../shared/webserver/webservice.h"

namespace brisa {
namespace upnp {

/*!
 * \class Brisa::BrisaAbstractEventSubscription brisaabstracteventsubscription.h
 * BrisaUpnp/BrisaAbstractEventSubscription
 *
 * \brief Abstract class that represents an event subscription
 */
class AbstractEventSubscription: public brisa::shared::webserver::WebService {
    Q_OBJECT
public:

    /*!
     * Construtcs an abstract event subscription with given \param sid, list of
     * \param callbackUrls, \param timeout and \param parent. \param timeout less
     * than 0 means infinite.
     */
    AbstractEventSubscription(const QString &sid,
            const QStringList &callbackUrls, const int &timeout = -1,
            QObject *parent = 0);

    /*!
     * Renews the subscription for the given \param newTimeout. \param newTimeout
     * less than 0 means infinite.
     */
    virtual void renew(const int &newTimeout = -1) = 0;

    /*!
     * Checks if the subscription has already expired. Returns true if it has
     * expired, else returns false.
     */
    bool hasExpired() const;

    /*!
     * Returns the next event key for this subscription.
     */
    quint32 getNextSeq();

    /*!
     * Returns this subscription's SID.
     */
    QString getSid() const;

    /*!
     * Returns this subscription's list of callback URLs.
     */
    QStringList getCallbackUrls() const;

    /*!
     * Returns this subscription's first callback URL.
     */
    QUrl getUrl();

protected:
    QString SID;

    const QStringList CALLBACK_URLS;

    QDateTime date;
    int timeout;

    quint32 lastSeq;
    bool firstMessageSent;
};

}
}
#endif /* _ABSTRACTEVENTSUBSCRIPTION_H */
