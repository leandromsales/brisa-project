#ifndef GETSTATUS_H
#define GETSTATUS_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class GetDoorStatus: public BrisaAction {
public:
    GetDoorStatus(BrisaService * service);
    virtual ~GetDoorStatus();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // GETSTATUS_H
