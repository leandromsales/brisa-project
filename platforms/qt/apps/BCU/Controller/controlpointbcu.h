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

#include "upnp/controlpoint/ControlPoint"
#include "dataobject.h"
#include "filedownloader.h"
#include "folderCompressor.h"
#include "essencials/qqmlobjectlistmodel.h"

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

using namespace brisa::shared::ssdp;
using namespace brisa::shared::webserver;
using namespace brisa::upnp;
using namespace brisa::upnp::controlpoint;

class ControlPointBCU: public ControlPoint {
    Q_OBJECT
    QML_OBJMODEL_PROPERTY(DataObject, apps)
public:
    ControlPointBCU(QQmlApplicationEngine *e, QObject *parent = 0, QString st = "ssdp:all", int mx = 5);
private:
    void decodeJsonList();
signals:
    void sendTestData();
    void deviceFound(Device *device);
    void decompressed();
private slots:
    void deviceRemoved(const QString udn);
    void replyFinished(QNetworkReply *reply);
    void requestError(QString errorMessage, QString methodName);
    void serviceCall(OutArgument, QString);
    void finishedGetApp(QString dirPath);
public:
    Q_INVOKABLE void run(QString appURL, QString name);
    Q_INVOKABLE bool deleteApp(QString name);
    void addAppOnDataList (QString udn, QString name, QString info, QUrl iconURL, QUrl appURL);
    void removeAppFromDataList (QString udn);
private:
    void decompressedFinished();
    QQmlApplicationEngine *m_engine;
    QString jsonMsg;
    QDir m_dir;

    Device * auxDev;
    Service * auxServ;
    DataObject *auxDO;
    QString auxAppURL, auxAppName;
};
