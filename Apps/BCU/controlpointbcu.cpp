#include "controlpointbcu.h"

brisa::upnp::controlpoint::ControlPointBCU::ControlPointBCU(QObject *parent, QString st, int mx) :
    brisa::upnp::controlpoint::ControlPoint(parent, st, mx) {
    // BCU's constructor is equal to control point's constructor
}

brisa::upnp::controlpoint::ControlPointBCU::~ControlPointBCU()
{
    // BCU has no specific destructor, control point's destructor is enough
}

void brisa::upnp::controlpoint::ControlPointBCU::replyFinished(QNetworkReply *reply)
{
    QTemporaryFile *rootXml = new QTemporaryFile();
    if (!rootXml->open()) {
        qWarning() << "BCU: Failed to open file for writing "
                      "root XML.";
    } else {
        rootXml->write(reply->readAll());
        rootXml->seek(0);
        QUrl *urlBase = new QUrl(reply->url());

        Device *device = new Device(rootXml, urlBase);

        QList<Service*> serviceList = device->getServiceList();

        bool has3gets = false;

        // foreach service, search by actions getListOfApps, getAppInfo, getApp
        foreach (Service *s, serviceList) {
            Action * list = s->getAction(QString("getListOfApps"));
            Action * info = s->getAction(QString("getAppInfo"));
            Action * app = s->getAction(QString("getApp"));

            // if 3 actions exists, break foreach
            if (list != 0 && info != 0 && app != 0) {
                has3gets = true;
                break;
            }
        }

        // if has3gets (it means, if 3 actions exists), go on
        // otherwise, this device is not compatible with bcu, so ignore it
        if (has3gets) {
            foreach(Service *s, serviceList) {
                s->setAttribute(Service::Host, urlBase->host());
                s->setAttribute(Service::Port,
                                QString().setNum(urlBase->port()));
            }

            rootXml->remove();
            delete rootXml;
            delete urlBase;
            reply->deleteLater();

            emit deviceFound(device);
        } else {
            qWarning() << "BCU: Incompatible device";
        }
    }
}
