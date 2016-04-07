/*
 * Esse arquivo é responsável pela adição de funções ao Device.
 * Essas funções são visualizadas pelo Control Point e podem ser manuseadas pelo mesmo.
 * As funções são definidas em um arquivo xml, que no exemplo é o functions.xml.
 * As funções são definidas como Actions e as variáveis como State Variables no xml.
 * As variáveis são definidas através de Arguments no Control Point.
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SERVICE_TYPE "urn:schemas-upnp-org:service:Functions:1"
#define SERVICE_ID "Functions"
#define SERVICE_XML_PATH "/Functions/functions.xml"
#define SERVICE_CONTROL "/Functions/control"
#define SERVICE_EVENT_SUB "/Functions/eventSub"

 #include "upnp/Action"
 #include "upnp/device/Service"
#include "brisaapplicationmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

class Functions : public brisa::upnp::device::Service
{
    Q_OBJECT
public:
    Functions(BRisaApplicationManager *manager);
private slots:
    OutArgument *getListOfApps();
//    OutArgument *getAppInfo(InArgument * const inArguments);
//    OutArgument *getApp(InArgument * const inArguments);
private:
    BRisaApplicationManager *appManager;
};


#endif // FUNCTIONS_H
