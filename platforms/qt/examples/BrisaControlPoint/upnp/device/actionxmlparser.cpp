#include "actionxmlparser.h"

#include <QtDebug>

namespace brisa {
namespace upnp {
namespace device {

ActionXmlParser::ActionXmlParser() {
    reader = new QDomDocument();
}

ActionXmlParser::~ActionXmlParser() {
    delete reader;
}

void ActionXmlParser::setXmlContent(const QByteArray &content) {
    reader->setContent(content);
}

bool ActionXmlParser::parseSOAP() {

    QDomElement root = reader->documentElement();

    if (!root.tagName().contains("Envelope", Qt::CaseInsensitive)){
        return false;
    }

    QDomElement child = root.firstChildElement();

    while (!child.isNull()) {
        if (child.tagName().contains("Body", Qt::CaseInsensitive)){
            parseElement(child);
        }
        child = child.nextSiblingElement();
    }

    if (method != "" && serviceType != "")
        return true;

    return false;
}

void ActionXmlParser::parseElement(QDomElement &element) {

    QDomElement child = element.firstChildElement();

    while (!child.isNull()) {

        method = child.tagName().section(':', -1);

        serviceType = child.attributes().item(0).nodeValue();
        domArgList = child.childNodes();

        for (int i = 0; i < domArgList.length(); i++) {
            if (!domArgList.item(i).isElement())
                return;

            QDomElement current = domArgList.item(i).toElement();
            args.insert(current.tagName(), current.text());
        }

        child = child.nextSiblingElement();
    }
}

}
}
}
