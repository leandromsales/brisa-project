#ifndef TURNON_H
#define TURNON_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class TurnOn: public BrisaAction {
public:
    TurnOn(BrisaService * service);
    virtual ~TurnOn();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // TURNON_H
