#ifndef ON_H
#define ON_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class On: public BrisaAction {
public:
    On(BrisaService * service);
    virtual ~On();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // ON_H
