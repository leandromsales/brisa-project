#ifndef SOAPENVELOPE_H
#define SOAPENVELOPE_H

#include <QObject>
#include <QHash>

// %1 - action name - SetAVTransportURI
// %2 - name space - urn:schemas-upnp-org:service:AVTransport:1
// %3 - parameters - <parm1></parm1><parm2></parm2>

static const QString SOAP_REQUEST_MESSAGE = "<?xml version=\"1.0\"?>"
                                            "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" "
                                                "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">"
                                            "<s:Body><u:%1 xmlns:u=\"%2\">%3</u:%1></s:Body>"
                                            "</s:Envelope>";

class SoapEnvelope : public QObject
{
    Q_OBJECT
public:
    explicit SoapEnvelope(QObject *parent = 0);
    QString get(const QString &, const QString &, const QHash<QString, QString> &);
    QHash<QString, QString> *parse(const QString &);
    
signals:
    
public slots:
    
};

#endif // SOAPENVELOPE_H
