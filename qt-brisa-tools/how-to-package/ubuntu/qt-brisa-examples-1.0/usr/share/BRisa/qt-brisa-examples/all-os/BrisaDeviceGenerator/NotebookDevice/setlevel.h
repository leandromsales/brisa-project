#ifndef SETLEVEL_H_
#define SETLEVEL_H_

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class SetLevel: public BrisaAction {
public:
    SetLevel(BrisaService * service);
    virtual ~SetLevel();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif /* SETLEVEL_H_ */
