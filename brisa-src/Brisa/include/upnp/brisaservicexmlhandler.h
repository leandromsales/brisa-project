#ifndef ABSTRACTSERVICEXMLHANDLER_H
#define ABSTRACTSERVICEXMLHANDLER_H

#include <QIODevice>
#include <QString>
#include <QDomDocument>

#include "brisaaction.h"
#include "brisaargument.h"
#include "brisastatevariable.h"
#include "brisaabstractservice.h"

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
