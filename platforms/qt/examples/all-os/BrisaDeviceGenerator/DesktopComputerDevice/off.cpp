#include "off.h"

Off::Off(BrisaService * service) :
    BrisaAction("Off", service) {
}

Off::~Off() {
}

QMap<QString, QString> Off::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);
    this->getStateVariable("Status")->setAttribute(BrisaStateVariable::Value, 0);
    QMap<QString, QString> outArguments;
    return outArguments;
}
