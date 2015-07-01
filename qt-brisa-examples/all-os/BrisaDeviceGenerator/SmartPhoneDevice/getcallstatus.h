#ifndef GETCALLSTATUS_H_
#define GETCALLSTATUS_H_

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class GetCallStatus: public BrisaAction {
public:
    GetCallStatus(BrisaService * service);
    virtual ~GetCallStatus();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif /* GETCALLSTATUS_H_ */
