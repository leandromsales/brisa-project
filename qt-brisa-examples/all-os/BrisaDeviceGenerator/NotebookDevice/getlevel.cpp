#include "getlevel.h"

GetLevel::GetLevel(BrisaService * service) :
    BrisaAction("GetLevel", service) {
}

GetLevel::~GetLevel() {
}

QMap<QString, QString> GetLevel::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);

    QMap<QString, QString> outArguments;
    outArguments.insert("Level", this->getStateVariable("Level")->getAttribute(
            BrisaStateVariable::Value));
    return outArguments;
}
