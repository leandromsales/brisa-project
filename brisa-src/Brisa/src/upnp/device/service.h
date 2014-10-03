#ifndef SERVICE_H
#define SERVICE_H

#include "../abstractservice.h"
#include "eventcontroller.h"
#include "actionxmlparser.h"
#include "../../shared/webserver/webserversession.h"
#include "../../shared/webserver/webserver.h"

namespace brisa {
//class shared::webserver::Webserver;
namespace upnp {
    //class Webserver;
    class Action;
namespace device {


class Service: public AbstractService
{
	Q_OBJECT
public:

	Service(QObject *parent = 0);

	Service(const QString &serviceType,
			const QString &serviceId = QString(),
			const QString &scpdUrl = QString(),
			const QString &controlUrl = QString(),
			const QString &eventSubUrl = QString(),
			const QString &host = QString(),
			QObject *parent = 0);

	Service(Service &service);

	virtual ~Service();

	StateVariable *getVariable(const QString &variableName);

	void buildWebServiceTree(brisa::shared::webserver::Webserver *sessionManager);

	void setDescriptionFile(const QString &scpdFilePath);

	QString getDescriptionFile();

protected:
	void onRequest(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

	private slots:
	void call(const QString &method, InArgument param, brisa::shared::webserver::WebserverSession *);
	void onInvalidRequest(brisa::shared::webserver::WebserverSession *session);

	private:

	void respondAction(brisa::shared::webserver::WebserverSession *session, const OutArgument *outArgs, const QString &actionName /* = QString() */);

	void respondError(brisa::shared::webserver::WebserverSession *session, int errorCode, QString errorDescription = QString());

	void parseDescriptionFile();

	void connectVariablesEventSignals();

	void setDefaultValues();

	brisa::upnp::Action * actionRelatedToMethod(QString methodSignature);

	void bindActionsToServiceMethods();

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
