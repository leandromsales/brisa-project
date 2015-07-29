#include "getsystemtime.h"

GetSystemTime::GetSystemTime(BrisaService * service) :
    BrisaAction("GetSystemDateTime", service) {
}

GetSystemTime::~GetSystemTime() {
}

QMap<QString, QString> GetSystemTime::run(
        const QMap<QString, QString> &inArguments) {
    Q_UNUSED(inArguments);

    QMap<QString, QString> outArguments;
    outArguments.insert("DateTime", QDateTime::currentDateTime().toString(
            Qt::ISODate));
    return outArguments;
}
