#ifndef RESET_H
#define RESET_H

#include <BrisaAction>
#include <BrisaService>

using namespace Brisa;

class Reset : public BrisaAction
{
public:
    Reset(BrisaService *service);

private:
    QMap<QString, QString> run(const QMap<QString, QString> &inArguments);
};

#endif // RESET_H
