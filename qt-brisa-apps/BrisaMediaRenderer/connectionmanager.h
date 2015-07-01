#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <BrisaService>

using Brisa::BrisaService;
using Brisa::BrisaAction;

/*!
 * \internal
 *
 * \class Brisa::ConnectionManager
 *
 * \brief Represents the Connection Manager of Media Renderer.
 */
class ConnectionManager : public BrisaService
{
    Q_OBJECT

public:
    /*!
     * \internal
     *
     * Create a new instance object of this class.
     */
    ConnectionManager();

public slots:
    /*!
     * \internal
     *
     * Return info about the protocol used.
     */
    BrisaOutArgument* getprotocolinfo(BrisaInArgument * const inArguments, BrisaAction * const action);
    /*!
     * \internal
     *
     * Return the current connection id.
     */
    BrisaOutArgument* getcurrentconnectionids(BrisaInArgument * const inArguments, BrisaAction * const action);
    /*!
     * \internal
     *
     * Create the current connection info.
     */
    BrisaOutArgument* getcurrentconnectioninfo(BrisaInArgument * const inArguments, BrisaAction * const action);
};

#endif // CONNECTIONMANAGER_H
