#include "brisamulticasteventmessage.h"

namespace brisa {
namespace upnp {
namespace device {

BrisaMulticastEventMessage::BrisaMulticastEventMessage(
        BrisaStateVariable *variable, QString LVL, QObject *parent) :
        BrisaAbstractEventMessage(parent),
        variable(variable),
        SEQ(variable->getNextMulticastSeq()),
        LVL(LVL)
{
}

//TODO Use a real value for BOOTID.UPNP.ORG
QByteArray BrisaMulticastEventMessage::getMessageHeader() const
{
    QString header = NOTIFY_EVENT_MESSAGE.arg(
                        "239.255.255.246:7900",
                        this->variable->getMulticastUdn() + "::" + this->variable->getMulticastUsn(),
                        this->variable->getMulticastSvcid(),
                        QString::number(this->SEQ),
                        this->LVL,
                        "",
                        QString::number(this->getMessageBody().size()));
    return header.toLatin1();
}

QByteArray BrisaMulticastEventMessage::getMessageBody() const {
    QByteArray body;

    body.append("<?xml version=\"1.0\"?>\r\n");
    body.append(
            "<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\">\r\n");

    QString variableName = this->variable->getAttribute(BrisaStateVariable::Name);
    QString variableValue = this->variable->getAttribute(BrisaStateVariable::Value);

    body.append("  <e:property>\r\n");
    body.append("    <" + variableName + ">" + variableValue + "</"
            + variableName + ">\r\n");
    body.append("  </e:property>\r\n");
    body.append("</e:propertyset>\r\n");

    return body;
}

QByteArray BrisaMulticastEventMessage::getRequestMessage() const {
    return (this->getMessageHeader() + this->getMessageBody());
}

}
}
}
