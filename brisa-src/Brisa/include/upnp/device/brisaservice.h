#ifndef BRISASERVICE_H
#define BRISASERVICE_H

#include "upnp/brisaabstractservice.h"
#include "upnp/device/brisaeventcontroller.h"
#include "upnp/device/brisaactionxmlparser.h"

#include "shared/webserver/webserversession.h"
#include "shared/webserver/webserver.h"

namespace brisa {
//class shared::webserver::Webserver;
namespace upnp {
//class Webserver;
namespace device {


class BrisaService: public BrisaAbstractService
{
	Q_OBJECT
public:

	BrisaService(QObject *parent = 0);

	BrisaService(const QString &serviceType,
			const QString &serviceId = QString(),
			const QString &scpdUrl = QString(),
			const QString &controlUrl = QString(),
			const QString &eventSubUrl = QString(),
			const QString &host = QString(),
			QObject *parent = 0);

	BrisaService(BrisaService &service);

	virtual ~BrisaService();

	BrisaStateVariable *getVariable(const QString &variableName);

	void buildWebServiceTree(brisa::shared::webserver::Webserver *sessionManager);

	void setDescriptionFile(const QString &scpdFilePath);

	QString getDescriptionFile();

protected:
	void onRequest(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

	private slots:
	void call(const QString &method, BrisaInArgument param, brisa::shared::webserver::WebserverSession *);
	void onInvalidRequest(brisa::shared::webserver::WebserverSession *session);

	private:

	void respondAction(brisa::shared::webserver::WebserverSession *session, const BrisaOutArgument *outArgs, const QString &actionName /* = QString() */);

	void respondError(brisa::shared::webserver::WebserverSession *session, int errorCode, QString errorDescription = QString());

	void parseDescriptionFile();

	void connectVariablesEventSignals();

	void setDefaultValues();

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

#endif /* BRISASERVICE_H_ */
