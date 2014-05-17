#ifndef EVENTCONTROLLER_H
#define EVENTCONTROLLER_H

#include "eventmessage.h"
#include "upnp/device/multicasteventmessage.h"
#include "upnp/device/eventsubscription.h"
#include "upnp/brisautils.h"
#include "shared/webserver/webservice.h"
#include "shared/webserver/webserver.h"

namespace brisa {
namespace upnp {
namespace device {

class EventController: public brisa::shared::webserver::WebService
{
	Q_OBJECT

public:
	EventController(brisa::shared::webserver::Webserver *sessionManager,
			QList<BrisaStateVariable *> *stateVariableList, QObject *parent = 0);
	virtual ~EventController();

	public slots:
	void variableChanged(BrisaStateVariable *variable);

	void subscribe(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

	void unsubscribe(const brisa::shared::webserver::http::HttpRequest &request, brisa::shared::webserver::WebserverSession *session);

	protected:
	void onRequest(const brisa::shared::webserver::http::HttpRequest &, brisa::shared::webserver::WebserverSession *session);

	private:
	EventController(const EventController &); // = delete;

	void sendEvent(const EventMessage &message, const QUrl &url);

	void sendMulticastEvent(const MulticastEventMessage &message);

	QStringList getEventUrls(const QString &urls);

	int getTimeOut(const QString &timeout);

	QList<EventSubscription *> subscriptions;

	QList<BrisaStateVariable *> *variableList;

	QUdpSocket udpSocket;
};
}
}
}
#endif /* _EVENTCONTROLLER_H */
