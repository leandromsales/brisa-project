#ifndef CALL_H_
#define CALL_H_

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class Call: public BrisaAction {
public:
    Call(BrisaService * service);
    virtual ~Call();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
    bool isCalling();
    bool validNumber(QString number);
};

#endif /* CALL_H_ */
