#include "bcajson.h"

BCAJson::BCAJson(QString path)
{
    QFile i(path);
    i.open(QIODevice::ReadOnly);
    QJsonDocument d = QJsonDocument::fromJson(i.readAll());
    mainObject = d.object();
}
QVariantMap BCAJson::toBRisaApp() { return mainObject.toVariantMap(); }

