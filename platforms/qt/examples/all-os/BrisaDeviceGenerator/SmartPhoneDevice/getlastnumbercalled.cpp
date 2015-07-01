#include "getlastnumbercalled.h"

GetLastNumberCalled::GetLastNumberCalled(BrisaService * service) :
    BrisaAction("GetLastNumberCalled", service) {
}

GetLastNumberCalled::~GetLastNumberCalled() {
}

QMap<QString, QString> GetLastNumberCalled::run(
        const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);

    QMap<QString, QString> outArguments;
    outArguments.insert("Number",
            this->getStateVariable("NumberCalling")->getAttribute(
                    BrisaStateVariable::Value));
    return outArguments;
}
