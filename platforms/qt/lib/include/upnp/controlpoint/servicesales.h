#ifndef SERVICESALES_H
#define SERVICESALES_H

#include <QObject>

#include "actionsales.h"
#include "statevariablesales.h"
#include "../shared/soap/soap.h"

class QHttp;
class QNetworkReply;
class QNetworkRequest;
class QNetworkAccessManager;
class QTemporaryFile;

namespace brisa {
namespace upnp {
namespace controlpoint {

class ServiceSales : public QObject
{
    Q_OBJECT
public:
    explicit ServiceSales(QObject *parent = 0);

    explicit ServiceSales(const QString &,
                     const QString &,
                     const QString &,
                     const QString &,
                     const QString &,
                     const QString &,
                     const QString &,
                     const QUrl &,
                     const QHash<QString, ActionSales *> &,
                     const QHash<QString, StateVariableSales *> &,
                     QObject *parent = 0);

    explicit ServiceSales(const ServiceSales &);
    ~ServiceSales();
    ServiceSales & operator=(const ServiceSales &);

    void setBaseUrl(const QUrl &, bool = true);

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &);
    QHash<QString, QString> getAttributes() const;

    void setActions(QHash<QString, ActionSales*>);
    QHash<QString, ActionSales *> getActions() const;
    ActionSales *getAction(const QString &);

    void setStateVariables(QHash<QString, StateVariableSales*>);
    QHash<QString, StateVariableSales*> getStateVariables() const;
    StateVariableSales *getStateVariable(const QString &) const;

    quint8 parseXMLDescription(QTemporaryFile *description);
    QString executeAction(const QString &, const QHash<QString, QString> &);

    typedef enum {
        CREATE_TMP_XML_ERROR = 100,
        WRONG_MAJOR_MINOR_VERSION_ERROR,
        MALFORMED_XML_DESCRIPTION_ERROR,
        INCOMPLETED_SERVICE_DESCRIPTION_ERROR
    } errorType;

private:
    QUrl baseUrl;
    QHash<QString, QString> attributes;
    QHash<QString, ActionSales*> actionList;
    QHash<QString, StateVariableSales*> stateVariableList;

    QNetworkAccessManager *downloader;
    brisa::shared::soap::SOAP *soapHttp;

signals:
    void serviceDescriptionReady(ServiceSales *);
    void errorParsingServiceDescription(ServiceSales *, quint8);
    void executionActionReply(QString, QHash<QString, ActionArgument *>, ServiceSales *);

public slots:
    void replyFinished(QNetworkReply *);
    void handleResponseReady();
    void handleRequestError(QNetworkReply::NetworkError, const QString &);

};

}
}
}
#endif // SERVICESALES_H
