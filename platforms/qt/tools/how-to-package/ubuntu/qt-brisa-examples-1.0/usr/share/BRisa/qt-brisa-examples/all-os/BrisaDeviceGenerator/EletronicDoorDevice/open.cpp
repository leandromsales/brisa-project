#include "open.h"

Open::Open(BrisaService * service) :
    BrisaAction("Open", service) {
}

Open::~Open() {
}

QMap<QString, QString> Open::run(const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);
    bool change = this->getStateVariable("Opened")->getValue().toBool()
            == false;
    this->getStateVariable("Opened")->setAttribute(BrisaStateVariable::Value, 1);

    QMap<QString, QString> outArguments;
    outArguments.insert("Changed", change ? "1" : "0");
    return outArguments;
}

