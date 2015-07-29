#ifndef GETSTATUS_H
#define GETSTATUS_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class GetStatus: public BrisaAction {
public:
    GetStatus(BrisaService * service);
    virtual ~GetStatus();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // GETSTATUS_H
