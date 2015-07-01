#include "setlevel.h"

SetLevel::SetLevel(BrisaService * service) :
    BrisaAction("SetLevel", service) {
}

SetLevel::~SetLevel() {
}

QMap<QString, QString> SetLevel::run(const QMap<QString, QString> &inArguments) {
    this->getStateVariable("Level")->setAttribute(BrisaStateVariable::Value,
            inArguments["NewLevel"]);
    QMap<QString, QString> outArguments;
    return outArguments;
}
