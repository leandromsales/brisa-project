#ifndef BRISAABSTRACTEVENTSUBSCRIPTION_H
#define BRISAABSTRACTEVENTSUBSCRIPTION_H

#include <QDateTime>
#include <QList>
#include <QString>
#include <QObject>
#include <QtCore>

#include "shared/webserver/webserver.h"
#include "shared/webserver/webservice.h"

namespace brisa {
namespace upnp {

/*!
 * \class Brisa::BrisaAbstractEventSubscription brisaabstracteventsubscription.h BrisaUpnp/BrisaAbstractEventSubscription
 *
 * \brief Abstract class that represents an event subscription
 */
class BrisaAbstractEventSubscription: public brisa::shared::webserver::WebService {
    Q_OBJECT
public:

    /*!
     * Construtcs an abstract event subscription with given \a sid, list of
     * \a callbackUrls, \a timeout and \a parent. \a timeout less than 0 means
     * infinite.
     */
    BrisaAbstractEventSubscription(const QString &sid,
            const QStringList &callbackUrls, const int &timeout = -1, // <0 = infinite
            QObject *parent = 0);

    /*!
     * Renews the subscription for the given \a newTimeout.
     */
    virtual void renew(const int &newTimeout = -1) = 0; // <0 = infinite

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
#endif /* _BRISAABSTRACTEVENTSUBSCRIPTION_H */
