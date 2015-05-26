#ifndef SERVICESALES_H
#define SERVICESALES_H

#include <QObject>

#include "actionsales.h"
#include "statevariablesales.h"
#include "../../shared/soap/soap.h"

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

    /*!
     * Set base URL of service.
     */
    void setBaseUrl(const QUrl &, bool = true);

    /*!
     * Add an attribute and your value in QHash \a attributes.
     */
    bool setAttribute(const QString &, const QString &);
    /*!
     * Return value of attribute with name equal of param.
     */
    QString getAttribute(const QString &);
    /*!
     * Return a QHash contains all attributes.
     */
    QHash<QString, QString> getAttributes() const;

    /*!
     * Set QHash \a actionList.
     */
    void setActions(QHash<QString, ActionSales*>);
    /*!
     * Return a QHash contains all actions.
     */
    QHash<QString, ActionSales *> getActions() const;
    /*!
     * Return action with name of param.
     */
    ActionSales *getAction(const QString &);

    /*!
     * Set QHash \a stateVariableList.
     */
    void setStateVariables(QHash<QString, StateVariableSales*>);
    /*!
     * Return a QHash contains all state variables.
     */
    QHash<QString, StateVariableSales*> getStateVariables() const;
    /*!
     * Return state variable with name of param.
     */
    StateVariableSales *getStateVariable(const QString &) const;

    /*!
     * Parse a description of service to XML.
     */
    quint8 parseXMLDescription(QTemporaryFile *description);
    /*!
     * Try execute action with name equal to first param and use second param
     * as your arguments.
     */
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
