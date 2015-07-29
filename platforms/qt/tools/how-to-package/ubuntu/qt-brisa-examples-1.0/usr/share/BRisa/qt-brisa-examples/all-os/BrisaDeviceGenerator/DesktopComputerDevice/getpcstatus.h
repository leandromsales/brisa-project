#ifndef GETPCSTATUS_H
#define GETPCSTATUS_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class GetPcStatus: public BrisaAction {
public:
    GetPcStatus(BrisaService * service);
    virtual ~GetPcStatus();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // GETPCSTATUS_H
