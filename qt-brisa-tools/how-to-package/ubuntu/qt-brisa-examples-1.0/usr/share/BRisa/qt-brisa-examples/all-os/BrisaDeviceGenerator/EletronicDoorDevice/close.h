#ifndef CLOSE_H
#define CLOSE_H

#include <BrisaUpnp/BrisaAction>
#include <BrisaUpnp/BrisaService>

using namespace Brisa;

class Close: public BrisaAction {
public:
    Close(BrisaService * service);
    virtual ~Close();

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // CLOSE_H
