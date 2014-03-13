#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

#include "action.h"
#include "statevariable.h"
#include "../shared/soap/soap.h"

class QHttp;
class QNetworkReply;
class QNetworkRequest;
class QNetworkAccessManager;
class QTemporaryFile;

namespace brisa {
namespace upnp {
namespace controlpoint {

class Service : public QObject
{
    Q_OBJECT
public:
    explicit Service(QObject *parent = 0);

    explicit Service(const QString &,
                     const QString &,
                     const QString &,
                     const QString &,
                     const QString &,
                     const QString &,
                     const QString &,
                     const QUrl &,
                     const QHash<QString, Action *> &,
                     const QHash<QString, StateVariable *> &,
                     QObject *parent = 0);

    explicit Service(const Service &);
    ~Service();
    Service & operator=(const Service &);

    void setBaseUrl(const QUrl &, bool = true);

    bool setAttribute(const QString &, const QString &);
    QString getAttribute(const QString &);
    QHash<QString, QString> getAttributes() const;

    void setActions(QHash<QString, Action*>);
    QHash<QString, Action *> getActions() const;
    Action *getAction(const QString &);

    void setStateVariables(QHash<QString, StateVariable*>);
    QHash<QString, StateVariable*> getStateVariables() const;
    StateVariable *getStateVariable(const QString &) const;

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
    QHash<QString, Action*> actionList;
    QHash<QString, StateVariable*> stateVariableList;

    QNetworkAccessManager *downloader;
    SOAP *soapHttp;

signals:
    void serviceDescriptionReady(Service *);
    void errorParsingServiceDescription(Service *, quint8);
    void executionActionReply(QString, QHash<QString, ActionArgument *>, Service *);

public slots:
    void replyFinished(QNetworkReply *);
    void handleResponseReady();
    void handleRequestError(QNetworkReply::NetworkError, const QString &);

};

}
}
}
#endif // SERVICE_H
