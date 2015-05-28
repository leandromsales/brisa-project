#ifndef SERVICE_H
#define SERVICE_H

#include "../abstractservice.h"
#include "eventcontroller.h"
#include "actionxmlparser.h"
#include "../../shared/webserver/webserversession.h"
#include "../../shared/webserver/webserver.h"

namespace brisa {
namespace upnp {
class Action;
namespace device {


class Service: public AbstractService
{
    Q_OBJECT
public:
    /*!
     * Constructor.
     */
    Service(QObject *parent = 0);
    /*!
     * Constructor.
     */
    Service(const QString &serviceType,
            const QString &serviceId = QString(),
            const QString &scpdUrl = QString(),
            const QString &controlUrl = QString(),
            const QString &eventSubUrl = QString(),
            const QString &host = QString(),
            QObject *parent = 0);
    /*!
     * Constructor.
     */
    Service(Service &service);
    /*!
     * Destructor.
     */
    virtual ~Service();
    /*!
     * Return variable with name equal to \param variableName.
     */
    StateVariable *getVariable(const QString &variableName);
    /*!
     * Create web service tree based on an abstract service.
     */
    void buildWebServiceTree(brisa::shared::webserver::Webserver *sessionManager);
    /*!
     * Set value of variable \a scpdFilePath with value of param \param scpdFilePath.
     */
    void setDescriptionFile(const QString &scpdFilePath);
    /*!
     * Return value of \a scpdFilePath.
     */
    QString getDescriptionFile();

protected:
    /*!
     * Handle with new requests. Also write a response if request is valid.
     */
    void onRequest(const brisa::shared::webserver::http::HttpRequest &request,
                   brisa::shared::webserver::WebserverSession *session);

private slots:
    /*!
     * Call \param method with \param param in \param session.
     */
    void call(const QString &method, InArgument param,
              brisa::shared::webserver::WebserverSession *session);
    /*!
     * Call \a respondError to write an approppriate error message
     */
    void onInvalidRequest(brisa::shared::webserver::WebserverSession *session);

private:
    /*!
     * Write a response to action handled by \a call method.
     */
    void respondAction(brisa::shared::webserver::WebserverSession *session,
                       const OutArgument *outArgs,
                       const QString &actionName /* = QString() */);
    /*!
     * Write an error message.
     */
    void respondError(brisa::shared::webserver::WebserverSession *session,
                      int errorCode, QString errorDescription = QString());
    /*!
     * Parse this service to XML. This method is called by \a buildWebServiceTree.
     */
    void parseDescriptionFile();
    /*!
     * Connect all variables which values changes must be send events.
     */
    void connectVariablesEventSignals();
    /*!
     * Set default value to variables.
     */
    void setDefaultValues();
    /*!
     * Call appropriate method associated with an action.
     */
    brisa::upnp::Action * actionRelatedToMethod(QString methodSignature);
    /*!
     * Bind actions (pre actions, pos actions and actions to handle with failures)
     * to services.
     */
    void bindActionsToServiceMethods();

private:
    QMap<QString, WebService *> childWebServices;
    QString scpdFilePath;
    QMetaMethod preActionMethod;
    QMetaMethod postActionMethod;
    QMetaMethod handleActionFailureMethod;
};

}
}
}

#endif /* SERVICE_H_ */
