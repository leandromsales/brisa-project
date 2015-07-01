#ifndef ABSTRACTSERVICEXMLHANDLER_H
#define ABSTRACTSERVICEXMLHANDLER_H

#include <QIODevice>
#include <QString>
#include <QDomDocument>

#include "action.h"
#include "argument.h"
#include "statevariable.h"
#include "abstractservice.h"

namespace brisa {
namespace upnp {

class ServiceXMLHandler : public QObject {
    Q_OBJECT
public:
    ServiceXMLHandler(QObject *parent = 0);
    virtual ~ServiceXMLHandler();

    void parseService(AbstractService *service, QIODevice *scpd);
};

}
}

#endif /* _ABSTRACTSERVICEXMLHANDLER_H */
