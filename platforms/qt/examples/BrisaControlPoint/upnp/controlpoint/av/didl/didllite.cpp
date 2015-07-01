#include <QtXml>
#include <QDomDocument>
#include <QDomElement>

#include "didllite.h"

namespace brisa {
namespace upnp {
namespace controlpoint {
namespace av {
namespace didl {

DIDLLite::DIDLLite(QObject *parent) :
    QObject(parent) {

}

Container *DIDLLite::parse(const QString &content) {
    QDomDocument document("didl");
    document.setContent(content);
    //qDebug() << document.toString();

    QDomElement element = document.documentElement();
    if (element.tagName() != "DIDL-Lite") {
        qDebug() << "DIDL-Lite: wrong format.";
        return new Container();
    }

    QDomNodeList domNodeList = element.elementsByTagName("container");
    QDomNodeList domNodeList1;
    QDomNode node;
    QString nodeName, nodeValue;

    Container *root = new Container();

    for (int i = 0; i < domNodeList.length(); i++) {
        node = domNodeList.item(i);
        nodeName = node.nodeName().toLower();
        Container *container = new Container();

        for (int k = 0; k < node.attributes().length(); k++) {
            nodeName = node.attributes().item(k).nodeName();
            nodeValue = node.attributes().item(k).nodeValue();
            //qDebug() << nodeName << nodeValue;
            container->addProperty(nodeName, nodeValue);
        }

        domNodeList1 = node.childNodes();
        for (int j = 0; j < domNodeList1.length(); j++) {
            QStringList sl = domNodeList1.item(j).nodeName().split(":");
            nodeName = (sl.length() == 2) ? sl[1] : sl[0];
            nodeValue = domNodeList1.item(j).childNodes().item(0).nodeValue();
            container->setAttribute(nodeName, nodeValue);
        }
        //qDebug() << container->getProperty("id");
        root->addContainer(container);
    }

    domNodeList = element.elementsByTagName("item");
    for (int i = 0; i < domNodeList.length(); i++) {
        node = domNodeList.item(i);
        Item *item = new Item();

        for (int k = 0; k < node.attributes().length(); k++) {
            nodeName = node.attributes().item(k).nodeName();
            nodeValue = node.attributes().item(k).nodeValue();
            //qDebug() << nodeName << nodeValue;
            item->addProperty(nodeName, nodeValue);
        }

        domNodeList1 = node.childNodes();
        for (int j = 0; j < domNodeList1.length(); j++) {
            node = domNodeList1.item(j);
            nodeName = node.nodeName();
            if (nodeName == "res") {
                Res *res = new Res();
                //qDebug() << node.childNodes().item(0).nodeValue();
                res->setValue(node.childNodes().item(0).nodeValue());
                for (int k = 0; k < node.attributes().length(); k++) {
                    nodeName = node.attributes().item(k).nodeName();
                    nodeValue = node.attributes().item(k).nodeValue();
                    //qDebug() << nodeName << nodeValue;
                    res->addProperty(nodeName, nodeValue);
                }
                item->setRes(res);
            } else {
                QStringList sl = domNodeList1.item(j).nodeName().split(":");
                nodeName = (sl.length() == 2) ? sl[1] : sl[0];
                nodeValue = domNodeList1.item(j).childNodes().item(0).nodeValue();
                //qDebug() << nodeName << nodeValue;
                item->setAttribute(nodeName, nodeValue);
            }
        }
        //qDebug() << container->getProperty("id");

        root->addItem(item);
    }

    return root;
}

}  // namespace didl
}  // namespace av
}
}
}
