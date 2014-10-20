#include "multicasteventreceiver.h"

#include "customreply.h"
#include "../brisautils.h"

namespace brisa {
namespace upnp {
namespace controlpoint {

MulticastEventReceiver::MulticastEventReceiver(QObject *parent) :
        QObject(parent)
{
    this->udpListener = new UdpListener("239.255.255.246", 7900,
                                       "MulticastEventReceiver",
                                       parent);
    connect(this->udpListener, SIGNAL(readyRead()), this, SLOT(read()));
}

MulticastEventReceiver::~MulticastEventReceiver()
{
    delete this->udpListener;
}

void MulticastEventReceiver::start()
{
    this->udpListener->start();
}

void MulticastEventReceiver::read()
{
    while (this->udpListener->hasPendingDatagrams()) {
        this->message.resize(udpListener->pendingDatagramSize());
        this->udpListener->readDatagram(this->message.data(), this->message.size());
        formatMessage();
    }
}

void MulticastEventReceiver::formatMessage()
{
    QString newMessage = QString(message);
    QString header = "";
    QString body = "";
    QString temp;
    int i = 0;
    while (i < newMessage.size()) {
        temp = "";
        do {
            temp.append(newMessage[i]);
            i++;
        } while (!temp.endsWith("\r\n"));
        if (temp == "\r\n") {
            break;
        }
        header.append(temp);
    }
    if (i == newMessage.size()) {
        qWarning() << "Bad formated multicast message!";
        return;
    }
    // QHttpRequestHeader requestHeader(header);
    CustomReply * requestHeader = new CustomReply(this);
    requestHeader->setHeader (QNetworkRequest::ContentTypeHeader, QVariant(header));
    QMap<QString, QString> map = mapFromMessage (header);

    bool castOk = 0;
    const int length = map.value("CONTENT-LENGTH").toInt(&castOk);
    if (!castOk) {
        qWarning() << "CONTENT-LENGTH is not ok on multicast message.";
        return;
    }
    for (; i < newMessage.size(); i++) {
        body.append(newMessage[i]);
    }
    if (length != body.size()) {
        qWarning() << "Value for CONTENT-LENGTH is wrong.";
        return;
    }
    this->attributes["USN"] =  map.value("USN");
    this->attributes["SVCID"] = map.value("SVCID");
    this->attributes["NT"] = map.value("NT");
    this->attributes["NTS"] = map.value("NTS");
    this->attributes["SEQ"] = map.value("SEQ");
    this->attributes["LVL"] = map.value("LVL");
    //TODO: use BOOTID.UPNP.ORG
//    this->attributes["BOOTID.UPNP.ORG"] = requestHeader.value("BOOTID.UPNP.ORG");
    parseBody(body.toUtf8());
    emit multicastReceived(attributes);
}

void MulticastEventReceiver::parseBody(QByteArray body)
{
    QDomDocument doc("body");
    body.replace(":e", "");
    doc.setContent(body);
    QString xmlns = doc.documentElement().attribute("xmlns");
    QDomNode node = doc.documentElement().firstChild().firstChild();
    QString variableName = node.toElement().tagName();
    QString newValue = node.toElement().text();
    this->attributes["xmlns"] = xmlns;
    this->attributes["variableName"] = variableName;
    this->attributes["newValue"] = newValue;
}

}
}
}
