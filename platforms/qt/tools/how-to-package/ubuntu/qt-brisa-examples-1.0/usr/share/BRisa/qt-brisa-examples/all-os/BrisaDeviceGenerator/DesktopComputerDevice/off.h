#ifndef OFF_H
#define OFF_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class Off: public BrisaAction {
public:
    Off(BrisaService * service);
    virtual ~Off();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // OFF_H
