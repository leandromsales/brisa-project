#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "src/upnp/controlpoint/cpdevice.h"
#include "src/upnp/controlpoint/eventproxy.h"
#include "src/upnp/controlpoint/multicasteventreceiver.h"
#include "src/upnp/brisaglobal.h"
#include "src/upnp/controlpoint/msearchclientcp.h"
#include "src/shared/ssdp/ssdpclient.h"
#include "src/shared/webserver/webserversession.h"

namespace brisa {

using namespace network;
using namespace shared::ssdp;
using namespace shared::webserver;

namespace upnp {
namespace controlpoint {

class ControlPointBCU: public QObject {
Q_OBJECT

public:
    ControlPointBCU(QObject *parent = 0, QString st = "ssdp:all", int mx = 5);
    ~ControlPointBCU();
    void start();
    void stop();
    bool isRunning();
    void discover();

private:
    void buildUrlBase();
    void discoverNetworkAddress();

signals:
    void deviceFound(Device *device);
    void deviceGone(QString udn);
    void multicastReceived(QString variableName, QString newValue);
    void multicastReceivedRaw(OutArgument raw);

private slots:
    void deviceFound(QString udn, QString location, QString st, QString ext,
            QString server, QString cacheControl);
    void deviceRemoved(const QString udn);
    void replyFinished(QNetworkReply *reply);
    void httpResponse(QNetworkReply *networkReply);
    void receiveMulticast(QMap<QString, QString> attributes);

public:
    EventProxy *getSubscriptionProxy(Service *service);
    QString getActiveIpAddress() {
        return ipAddress;
    }
    int getActivePort() {
        return port;
    }

private:
    QNetworkAccessManager *m_networkAccessManager;
    QNetworkAccessManager *downloader;
    QString ipAddress;
    QString urlBase;
    int port;
    bool running;
    brisa::shared::webserver::Webserver *webserver;
    brisa::shared::ssdp::SSDPClient *ssdpClient;
    MSearchClientCP *msearch;
    MulticastEventReceiver *multicastReceiver;
    int deliveryPath;
    QMap<int, EventProxy*> requests;
    QMap<QString, int> subscriptions;
    QMap<int, int> requestsMapping;
};

}
}
}
