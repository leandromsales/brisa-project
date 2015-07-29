#ifndef GETLEVEL_H_
#define GETLEVEL_H_

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class GetLevel: public BrisaAction {
public:
    GetLevel(BrisaService * service);
    virtual ~GetLevel();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif /* GETLEVEL_H_ */
