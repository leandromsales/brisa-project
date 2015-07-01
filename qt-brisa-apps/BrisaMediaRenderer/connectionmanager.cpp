#include "connectionmanager.h"

using namespace Brisa;


ConnectionManager::ConnectionManager() :
        BrisaService("urn:schemas-upnp-org:service:ConnectionManager:1",
                     "ConnectionManager",
                     "ConnectionManager/render-connmgr-scpd.xml",
                     "ConnectionManager/control",
                     "ConnectionManager/eventSub")
{
    setDescriptionFile("render-connmgr-scpd.xml");
}

BrisaOutArgument* ConnectionManager::getprotocolinfo(
        BrisaInArgument *const inArguments, BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("Source", "");
    outArgs->insert("Sink", "");
    return outArgs;
}

BrisaOutArgument* ConnectionManager::getcurrentconnectionids(
        BrisaInArgument *const inArguments, BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //TODO: everything!
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("ConnectionIDs", "");
    return outArgs;
}

BrisaOutArgument* ConnectionManager::getcurrentconnectioninfo(
        BrisaInArgument *const inArguments, BrisaAction *const action)
{
    Q_UNUSED(inArguments)
    Q_UNUSED(action)
    //There's an input argument called ConnectionID
    //TODO: everything!
    BrisaOutArgument *outArgs = new BrisaOutArgument();
    outArgs->insert("RcsID", "");
    outArgs->insert("AVTransportID", "");
    outArgs->insert("ProtocolInfo", "");
    outArgs->insert("PeerConnectionManager", "");
    outArgs->insert("PeerConnectionID", "");
    outArgs->insert("Direction", "");
    outArgs->insert("Status", "");
    return outArgs;
}
