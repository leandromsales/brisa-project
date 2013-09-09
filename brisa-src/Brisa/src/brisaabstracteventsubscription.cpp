#include "brisaabstracteventsubscription.h"

using namespace Brisa;

BrisaAbstractEventSubscription::BrisaAbstractEventSubscription(
        const QString &sid, const QStringList &callbackUrls,
        const int &timeout, QObject *parent) :
    WebService(parent),
    SID(sid),
    CALLBACK_URLS(callbackUrls),
    date(QDateTime::currentDateTime()), timeout(timeout),
    lastSeq(0), firstMessageSent(false) { }

void BrisaAbstractEventSubscription::renew(const int &newTimeout) {
    this->date = QDateTime::currentDateTime();
    this->timeout = newTimeout;
}

bool BrisaAbstractEventSubscription::hasExpired() const {
    return (timeout < 0)
            ? false
            : date.secsTo(QDateTime::currentDateTime()) >= this->timeout;
}

quint32 BrisaAbstractEventSubscription::getNextSeq() {
    if (!firstMessageSent) {
        firstMessageSent = true;
        return 0;
    }

    if (this->lastSeq == 0xFFFFFFFF) /*4294967295L*/
        this->lastSeq = 0;

    return ++(this->lastSeq);
}

QString BrisaAbstractEventSubscription::getSid() const {
    return this->SID;
}

QStringList BrisaAbstractEventSubscription::getCallbackUrls() const {
    return this->CALLBACK_URLS;
}

QUrl BrisaAbstractEventSubscription::getUrl() {
    QString callback;
    if (!CALLBACK_URLS.empty()) {
        callback = CALLBACK_URLS[0];
    }

    return QUrl(callback);
}

