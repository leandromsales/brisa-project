#ifndef BRISASERVICE_H
#define BRISASERVICE_H

#include "../brisaabstractservice.h"
#include "brisaeventcontroller.h"
#include "brisaactionxmlparser.h"

#include "upnp/shared/webserver/webserversession.h"
#include "upnp/shared/webserver/webserver.h"

namespace Brisa {

    class Webserver;

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

        void buildWebServiceTree(Webserver *sessionManager);

        void setDescriptionFile(const QString &scpdFilePath);

        QString getDescriptionFile();

    protected:
        void onRequest(const HttpRequest &request, WebserverSession *session);

    private slots:
        void call(const QString &method, BrisaInArgument param, WebserverSession *);
        void onInvalidRequest(WebserverSession *session);

    private:

        void respondAction(WebserverSession *session, const BrisaOutArgument *outArgs, const QString &actionName /* = QString() */);

        void respondError(WebserverSession *session, int errorCode, QString errorDescription = QString());

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

#endif /* BRISASERVICE_H_ */
