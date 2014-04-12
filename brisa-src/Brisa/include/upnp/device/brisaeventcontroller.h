#ifndef BRISAEVENTCONTROLLER_H
#define BRISAEVENTCONTROLLER_H

#include "brisaeventmessage.h"
#include "brisamulticasteventmessage.h"
#include "brisaeventsubscription.h"
#include "upnp/brisautils.h"
#include "shared/webserver/webservice.h"
#include "shared/webserver/webserver.h"

namespace brisa {
namespace upnp {
namespace device {

class BrisaEventController: public brisa::shared::webserver::WebService
{
	Q_OBJECT

public:
	BrisaEventController(brisa::shared::webserver::Webserver *sessionManager,
			QList<BrisaStateVariable *> *stateVariableList, QObject *parent = 0);
	virtual ~BrisaEventController();

	public slots:
	void variableChanged(BrisaStateVariable *variable);

	void subscribe(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

	void unsubscribe(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

	protected:
	void onRequest(const brisa::shared::webserver::http::HttpRequest &, brisa::shared::webserver::WebserverSession *session);

	private:
	BrisaEventController(const BrisaEventController &); // = delete;

	void sendEvent(const BrisaEventMessage &message, const QUrl &url);

	void sendMulticastEvent(const BrisaMulticastEventMessage &message);

	QStringList getEventUrls(const QString &urls);

	int getTimeOut(const QString &timeout);

	QList<BrisaEventSubscription *> subscriptions;

	QList<BrisaStateVariable *> *variableList;

	QUdpSocket udpSocket;
};
}
}
}
#endif /* _BRISAEVENTCONTROLLER_H */
