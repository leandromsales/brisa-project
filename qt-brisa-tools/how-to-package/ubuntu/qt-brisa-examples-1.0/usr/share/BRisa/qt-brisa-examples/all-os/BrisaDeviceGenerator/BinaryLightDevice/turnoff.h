#ifndef TURNOFF_H
#define TURNOFF_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class TurnOff: public BrisaAction {
public:
    TurnOff(BrisaService * service);
    virtual ~TurnOff();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // TURNOFF_H
