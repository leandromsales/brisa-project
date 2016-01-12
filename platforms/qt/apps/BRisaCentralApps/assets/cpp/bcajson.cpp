#include "bcajson.h"

BCAJson::BCAJson(QString path)
{
    QFile i(path);
    i.open(QIODevice::ReadOnly);

    //Get file size
    int length = i.size();

    //Reading the JSON
    QJsonDocument d = QJsonDocument::fromJson(i.readAll());
    mainObject = d.object();
}
QVariantMap BCAJson::toBRisaApp() { return mainObject.toVariantMap(); }

