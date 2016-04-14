#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QtCore>
#include <QtDebug>
#include <QNetworkAccessManager>

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickItem>

#include <QElapsedTimer>

#include "src/upnp/controlpoint/cpdevice.h"
#include "src/upnp/controlpoint/eventproxy.h"
#include "src/upnp/controlpoint/multicasteventreceiver.h"
#include "src/upnp/brisaglobal.h"
#include "src/upnp/controlpoint/msearchclientcp.h"
#include "src/shared/ssdp/ssdpclient.h"
#include "src/shared/webserver/webserversession.h"

#include "dataobject.h"
#include "filedownloader.h"
#include "FolderCompressor.h"

/*
 * Esta classe é bem parecida com a classe ControlPoint do BRisa. Inicialmente,
 * acreditava que poderia estendê-la e apenas sobreescrever os slots. Após algumas
 * tentativas e testes, percebi que isso não é possível. Assim, grande parte da
 * classe ControlPointBCU é igual a classe ControlPoint. As modificações ocorrem
 * no slot replyFinished, onde foram adicionadas condições que:
 *   1. verificam se o dispositivo possui o device type correto
 *   2. se a resposta a [1] for verdadeira, as estruturas condicionais verificam
 *      se o dispositivo possui os 3 gets obrigatórios
 * Para o caso de uma dessas condições falharem, uma mensagem adequada é mostrada.
 */

namespace brisa {

using namespace network;
using namespace shared::ssdp;
using namespace shared::webserver;

namespace upnp {
namespace controlpoint {

class ControlPointBCU: public QObject {
    Q_OBJECT

public:
    /*!
     *  Constructor
     *  \param parent parent
     *  \param st service type
     *  \param mx interval
     */
    ControlPointBCU(QObject *parent = 0, QString st = "ssdp:all", int mx = 5);

    /*!
     *  Destructor
     */
    ~ControlPointBCU();

    /*!
     *  Starts the control point, the ssdpClient and the msearch
     *  \sa stop(), isRunning()
     */
    void start();

    /*!
     *  Stops the control point, the ssdpClient and the msearch
     *  \sa start(), isRunning()
     */
    void stop();

    /*!
     *  Return if the control point is running
     *  \return true if the control point is running or false otherwise
     *  \sa start(), stop()
     */
    bool isRunning();

    /*!
     *  Starts the control point msearch discover.
     */
    void discover();

private:
    /*!
     *  Private function to create the UrlBase of the control point (http:// + ip + : + port)'.
     */
    void buildUrlBase();

    /*!
     *  Private function to discover the network address currently used by the machine where control
     *  point is running and select an free port to use with BrisaWebServer.
     */
    void discoverNetworkAddress();

    /*!
     *  This function decode JSON received by getListOfApps of BCA and create a icon
     *  on grid foreach app on JSON.
     */
    void decodeJsonList();

signals:
    /*!
     *    \fn void BrisaControlPoint::deviceFound(BrisaControlPointDevice *device)
     *    This signal is emitted when a new device is find in network and all it's attributes are created
     *    by the xml reading.
     *    \sa deviceGone(QString udn)
     *    \param device the device that has been found
     */
    void deviceFound(Device *device);

    /*!
     *    \fn void BrisaControlPoint::deviceGone(QString udn)
     *    This signal is emitted when a device leaves the network, that means that the the ssdp client
     *    received a "ssdp:byebye" message from the device and, to handle this, the control point emit
     *    a deviceGone event with the device's \a udn as parameter.
     *    \sa deviceFound(BrisaControlPointDevice *device)
     */
    void deviceGone(QString udn);

    /*!
     * Signal emitted when a multicast message is received.
     *
     * \param variableName name of the state variable.
     * \param newValue new value of the state variable.
     */
    void multicastReceived(QString variableName, QString newValue);

    /*!
     * Signal emitted when a multicast message is received.
     *
     * Similar to multicastReceived, but sends a OutArgument containing
     * all the attributes of the multicast event massage, including
     * "variableName" and "newValue".
     *
     * \param raw attributes of the multicast event message.
     */
    void multicastReceivedRaw(OutArgument raw);
    void decompressed();

private slots:
    /*!
     *  Slot called when receive a newDevice event, this slot start the device's
     *  xml download.
     *  \param udn \a empty
     *  \param location \a empty
     *  \param ext \a empty
     *  \param server \a empty
     *  \param cacheControl \a empty
     */
    void deviceFound(QString udn, QString location, QString st, QString ext,
                     QString server, QString cacheControl);

    /*!
     *  Slot called when ssdp client emits a removed device event, this slot
     *  emit the deviceGone signal which has as parameter the device's udn.
     *  \param udn empty
     */
    void deviceRemoved(const QString udn);

    /*!
     * Write the content of the downloaded xml in a new xml temporary file to set
     * the device's attributes emits the deviceFound signal when finished. Also
     * verify some conditions of device, like device type (must be
     * org.compelab.AppServer:1) and service list.
     *  \param reply \a empty
     */
    void replyFinished(QNetworkReply *reply);

    /*!
     *  Slot to get the response of the http request, made by BrisaEventProxy
     *  class and set the SID of the subscription object.
     *  \param i \a empty
     *  \param error \a empty
     */
    void httpResponse(QNetworkReply *networkReply);

    /*!
     * Receives the attributes of a multicast event.
     *
     * \param attributes attributes of the multicast event.
     */
    void receiveMulticast(QMap<QString, QString> attributes);

    void serviceCall(OutArgument, QString);
    void requestError(QString errorMessage, QString methodName);
    void downloaded ();
    void finishedGetApp();
    void decompressedFinished();

public:
    EventProxy *getSubscriptionProxy(Service *service);

    QString getActiveIpAddress() {
        return ipAddress;
    }

    int getActivePort() {
        return port;
    }

    Q_INVOKABLE void run(QString appURL, QString name);

    Q_INVOKABLE bool deleteApp(QString name);

    QList<QObject*> getDataList () {
        return this->dataList;
    }

    void addAppOnDataList (QString udn, QString name, QString info,
                           QUrl iconURL, QUrl appURL);

    void removeAppFromDataList (QString udn);

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
    QList<QObject*> dataList;
    QQmlApplicationEngine engine;
    QString jsonMsg;

    Device * auxDev;
    Service * auxServ;
    DataObject *auxDO;
    QString auxAppURL, auxAppName;

    QElapsedTimer timer;
};

}
}
}
