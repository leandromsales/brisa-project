#ifndef GETSYSTEMTIME_H
#define GETSYSTEMTIME_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class GetSystemTime: public BrisaAction {
public:
    GetSystemTime(BrisaService * service);
    virtual ~GetSystemTime();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // GETSYSTEMTIME_H
