#ifndef GETLASTNUMBERCALLED_H_
#define GETLASTNUMBERCALLED_H_

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class GetLastNumberCalled: public BrisaAction {
public:
    GetLastNumberCalled(BrisaService * service);
    virtual ~GetLastNumberCalled();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif /* GETLASTNUMBERCALLED_H */
