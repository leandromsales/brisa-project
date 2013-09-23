#ifndef BRISAABSTRACTSERVICEXMLHANDLER_H
#define BRISAABSTRACTSERVICEXMLHANDLER_H

#include <QIODevice>
#include <QString>
#include <QDomDocument>

#include "brisaaction.h"
#include "brisaargument.h"
#include "brisastatevariable.h"
#include "brisaabstractservice.h"

namespace Brisa {

class BrisaServiceXMLHandler : public QObject {
    Q_OBJECT
public:
    BrisaServiceXMLHandler(QObject *parent = 0);
    virtual ~BrisaServiceXMLHandler();

    void parseService(BrisaAbstractService *service, QIODevice *scpd);
};

}

#endif /* _BRISAABSTRACTSERVICEXMLHANDLER_H */
