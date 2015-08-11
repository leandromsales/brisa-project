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
#include <QtDebug>

class Functions : public brisa::upnp::device::Service
{
    Q_OBJECT

public:

    Functions();
    ~Functions();

private slots:

    /*
     * Essa função retorna o valor da variável Status.
     * O valor padrão da variável Status é " - ".
     */
    OutArgument* getStatus();

    /*
     * Essa função define o valor da variável Status através da variável Target.
     * O valor padrão da variável Target é " - "
     */
    OutArgument* setStatus(InArgument *const inArguments);

};


#endif // FUNCTIONS_H
