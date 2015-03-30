#ifndef SOAPENVELOPE_H
#define SOAPENVELOPE_H

#include <QObject>
#include <QHash>

namespace brisa {
namespace shared {
namespace soap {

// %1 - action name - SetAVTransportURI
// %2 - name space - urn:schemas-upnp-org:service:AVTransport:1
// %3 - parameters - <parm1></parm1><parm2></parm2>

static const QString SOAP_REQUEST_MESSAGE = "<?xml version=\"1.0\"?>"
                                            "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                            "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                            "<s:Body><u:%1 xmlns:u=\"%2\">%3</u:%1></s:Body>"
                                            "</s:Envelope>";

/*!
 * This class provides a implementation of the SOAP envelope, which defines what is in the SOAP message and how process it.
 */

class SoapEnvelope : public QObject
{
    Q_OBJECT
public:
    explicit SoapEnvelope(QObject *parent = 0);
    /*!
     * \brief Return a SOAP message
     * \param actionName is name of the UPnP action
     * \param ns is UPnP name space
     * \param parameters are UPnP parameters of the action
     */
    QString get(const QString &actionName, const QString &ns, const QHash<QString, QString> &parameters);

    /*!
     * Turn a XML message in a hash
     */
    QHash<QString, QString> *parse(const QString &);

signals:

public slots:

};

}
}
}

#endif // SOAPENVELOPE_H
