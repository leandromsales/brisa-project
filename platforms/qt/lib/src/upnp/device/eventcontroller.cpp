#include <QDebug>

#include "eventcontroller.h"
#include "../../shared/webserver/webserversession.h"
#include "../../shared/webserver/webserver.h"

namespace brisa {
using namespace shared::webserver;
using namespace shared::webserver::http;
namespace upnp {
namespace device {

EventController::EventController(
        Webserver *sessionManager,
        QList<StateVariable *> *stateVariableList,
        QObject *parent) :
        WebService(parent),
        variableList(stateVariableList)
{
    udpSocket.bind(QHostAddress("239.255.255.246"), 7900);
}

void EventController::onRequest(const HttpRequest &r,
                                     WebserverSession *session)
{
    if (r.method() == "SUBSCRIBE") {
        subscribe(r, session);
    } else if (r.method() == "UNSUBSCRIBE") {
        unsubscribe(r, session);
    }
}

inline void EventController::subscribe(const HttpRequest &request,
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
        foreach (EventSubscription *current, subscriptions)
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

        EventSubscription *newSubscriber = new EventSubscription(
                getUuid(), getEventUrls(headers.value("CALLBACK")),
                getTimeOut(headers.value("TIMEOUT")));

        subscriptions.append(newSubscriber);
        session->respond(newSubscriber->getAcceptSubscriptionResponse());

        EventMessage *message = new EventMessage(*newSubscriber,
                                                           this->variableList);
        sendEvent(*message, newSubscriber->getUrl());
        delete message;

        return;
    } else {
        session->respond(HttpResponse(request.httpVersion(),
                                      HttpResponse::PRECONDITION_FAILED));
    }
}

inline void EventController::unsubscribe(const HttpRequest &request,
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

void EventController::sendEvent(const EventMessage &message, const QUrl &url)
{
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(url.host(), url.port());

    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), socket,
            SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()), socket, SLOT(deleteLater()));

    socket->write(message.getRequestMessage());

    qDebug() << "BrisaEventController sending event to "
             << message.getSid() << " at Host: "
             << url.host() << ":" << url.port();
}

EventController::~EventController()
{
    qDeleteAll(this->subscriptions);
    this->subscriptions.clear();
}

void EventController::variableChanged(StateVariable *variable)
{
    if (variable->multicastEvents()) {
        MulticastEventMessage message(variable, "upnp:/general");
        sendMulticastEvent(message);
    }
    QList<StateVariable *> variables;
    variables.append(variable);

    for (QList<EventSubscription *>::iterator i = this->subscriptions.begin();
         i != this->subscriptions.end(); ++i) {
        // Remove expired subscriptions
        if ((*i)->hasExpired()) {
            qDebug() << "Removing subscription:" << (*i)->getSid();
            delete *i;
            this->subscriptions.erase(i);
            continue;
        }
        qDebug() << "Sending unicast message...";
        EventMessage message(*(*i), &variables);
        this->sendEvent(message, (*i)->getUrl());
    }

}

void EventController::sendMulticastEvent(const MulticastEventMessage &message)
{
    udpSocket.writeDatagram(message.getRequestMessage(),
                            QHostAddress("239.255.255.246"), 7900);
    qDebug() << "BrisaEventController sending multicast event";

}

QStringList EventController::getEventUrls(const QString &urls)
{
    QList<QString> urlList;
    QStringList list = urls.split(">", QString::SkipEmptyParts);
    list.replaceInStrings("<", "");
    return list;
}

int EventController::getTimeOut(const QString &timeout) {
    QString returnTime = timeout;
    returnTime.remove("Second-");
    bool ok;
    return returnTime.toInt(&ok, 10);
}

}
}
}
