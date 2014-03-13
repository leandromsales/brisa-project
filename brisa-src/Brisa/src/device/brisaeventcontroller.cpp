#include <QDebug>

#include "brisaeventcontroller.h"
#include "upnp/shared/webserver/webserversession.h"
#include "upnp/shared/webserver/webserver.h"

namespace brisa {
namespace upnp {
namespace device {

BrisaEventController::BrisaEventController(
        Webserver *sessionManager,
        QList<BrisaStateVariable *> *stateVariableList,
        QObject *parent) :
        WebService(parent),
        variableList(stateVariableList)
{
    udpSocket.bind(QHostAddress("239.255.255.246"), 7900);
}

void BrisaEventController::onRequest(const HttpRequest &r,
                                     WebserverSession *session)
{
    if (r.method() == "SUBSCRIBE") {
        subscribe(r, session);
    } else if (r.method() == "UNSUBSCRIBE") {
        unsubscribe(r, session);
    }
}

inline void BrisaEventController::subscribe(const HttpRequest &request,
                                            WebserverSession *session)
{
    const QHash<QByteArray, QByteArray> headers = request.headers();
    if (headers.contains("SID")) { //Then it's probably a renewal request.
        if (headers.contains("NT") || headers.contains("CALLBACK")) {
            session->respond(HttpResponse(request.httpVersion(),
                                          HttpResponse::BAD_REQUEST,
                                          true));
            return;
        }

        if (headers.value("SID").isEmpty()) {
            session->respond(HttpResponse(request.httpVersion(),
                                          HttpResponse::PRECONDITION_FAILED,
                                          true));
            return;
        }

        bool validSubscription = false;
        foreach (BrisaEventSubscription *current, subscriptions)
        {
            if (current->getSid() == headers.value("SID")) {
                current->renew(getTimeOut(headers.value("TIMEOUT")));

                qDebug()
                        << "BrisaEventController renewing subscription for "
                        << headers.value("SID");
                validSubscription = true;

                session->respond(current->getAcceptSubscriptionResponse());
            }
        }

        if (!validSubscription)
            session->respond(HttpResponse(request.httpVersion(),
                                          HttpResponse::PRECONDITION_FAILED));

        return;
    } else if (headers.contains("NT") && headers.contains("CALLBACK")) {

        if (headers.value("NT") != "upnp:event") {
            session->respond(HttpResponse(request.httpVersion(),
                                          HttpResponse::PRECONDITION_FAILED));
            return;
        }

        qDebug() << "BrisaEventController received a subscription request:"
            " Callback: " << headers.value("CALLBACK") << "- Timeout: "
                << headers.value("TIMEOUT");

        BrisaEventSubscription *newSubscriber = new BrisaEventSubscription(
                getUuid(), getEventUrls(headers.value("CALLBACK")),
                getTimeOut(headers.value("TIMEOUT")));

        subscriptions.append(newSubscriber);
        session->respond(newSubscriber->getAcceptSubscriptionResponse());

        BrisaEventMessage *message = new BrisaEventMessage(*newSubscriber,
                                                           this->variableList);
        sendEvent(*message, newSubscriber->getUrl());
        delete message;

        return;
    } else {
        session->respond(HttpResponse(request.httpVersion(),
                                      HttpResponse::PRECONDITION_FAILED));
    }
}

inline void BrisaEventController::unsubscribe(const HttpRequest &request,
                                              WebserverSession *session)
{
    const QHash<QByteArray, QByteArray> headers = request.headers();
    if (headers.contains("SID")) {
        if (headers.contains("NT") || headers.contains("CALLBACK")) {
            session->respond(HttpResponse(request.httpVersion(),
                                          HttpResponse::BAD_REQUEST));
            return;
        }

        if (headers.value("SID").isEmpty()) {
            session->respond(HttpResponse(request.httpVersion(),
                                          HttpResponse::PRECONDITION_FAILED,
                                          true));
            return;
        }

        bool validSubscription = false;
        for (int i = 0; i < subscriptions.size(); ++i) {
            if (("uuid:" + subscriptions.at(i)->getSid()).toUtf8()
                == headers.value("SID")) {
                session->respond(subscriptions.at(i)
                                 ->getAcceptUnsubscriptionResponse());

                delete subscriptions.at(i);
                subscriptions.removeAt(i);

                qDebug() << "BrisaEventController canceling subscription for "
                        << headers.value("SID");

                validSubscription = true;
            }
        }

        if (!validSubscription)
            session->respond(HttpResponse(request.httpVersion(),
                                          HttpResponse::PRECONDITION_FAILED,
                                          true));

    } else {
        session->respond(HttpResponse(request.httpVersion(),
                                      HttpResponse::PRECONDITION_FAILED));
    }
}

void BrisaEventController::sendEvent(const BrisaEventMessage &message, const QUrl &url)
{
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(url.host(), url.port());

    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()), socket, SLOT(deleteLater()));

    socket->write(message.getRequestMessage());

    qDebug() << "BrisaEventController sending event to "
             << message.getSid() << " at Host: "
             << url.host() << ":" << url.port();
}

BrisaEventController::~BrisaEventController()
{
    qDeleteAll(this->subscriptions);
    this->subscriptions.clear();
}

void BrisaEventController::variableChanged(BrisaStateVariable *variable)
{
    if (variable->multicastEvents()) {
        BrisaMulticastEventMessage message(variable, "upnp:/general");
        sendMulticastEvent(message);
    }
    QList<BrisaStateVariable *> variables;
    variables.append(variable);

    for (QList<BrisaEventSubscription *>::iterator i = this->subscriptions.begin(); i != this->subscriptions.end(); ++i) {
        // Remove expired subscriptions
        if ((*i)->hasExpired()) {
            qDebug() << "Removing subscription:" << (*i)->getSid();
            delete *i;
            this->subscriptions.erase(i);
            continue;
        }
        qDebug() << "Sending unicast message...";
        BrisaEventMessage message(*(*i), &variables);
        this->sendEvent(message, (*i)->getUrl());
    }

}

void BrisaEventController::sendMulticastEvent(const BrisaMulticastEventMessage &message)
{
    udpSocket.writeDatagram(message.getRequestMessage(),
                            QHostAddress("239.255.255.246"), 7900);
    qDebug() << "BrisaEventController sending multicast event";

}

QStringList BrisaEventController::getEventUrls(const QString &urls)
{
    QList<QString> urlList;
    QStringList list = urls.split(">", QString::SkipEmptyParts);
    list.replaceInStrings("<", "");
    return list;
}

int BrisaEventController::getTimeOut(const QString &timeout) {
    QString returnTime = timeout;
    returnTime.remove("Second-");
    bool ok;
    return returnTime.toInt(&ok, 10);
}

}
}
}
