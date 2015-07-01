#include "reset.h"

Reset::Reset(BrisaService *service) : BrisaAction("Reset", service)
{
}

QMap<QString, QString> Reset::run(const QMap<QString, QString> &inArguments)
{
    Q_UNUSED(inArguments);
    this->getStateVariable("Game")->setAttribute(BrisaStateVariable::Value, "");
    QMap<QString, QString> outArgs;
    return outArgs;
}
