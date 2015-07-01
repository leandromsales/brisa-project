#ifndef OPEN_H
#define OPEN_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class Open: public BrisaAction {
public:
    Open(BrisaService * service);
    virtual ~Open();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // OPEN_H
