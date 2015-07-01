#ifndef HANGUP_H_
#define HANGUP_H_

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class HangUp: public BrisaAction {
public:
    HangUp(BrisaService * service);
    virtual ~HangUp();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif /* HANGUP_H_ */
