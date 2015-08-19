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

BCAJson::~BCAJson(){}

QList<QString> *BCAJson::toBRisaApp()
{
    QList<QString> *list = new QList<QString>();

    list->append(mainObject["Description"].toString());

    QJsonObject services = mainObject["Services"].toArray()[0].toObject();
    QStringList servicesKeyList = services.keys();

    foreach (QString str, servicesKeyList) {
        list->append(str);
        list->append(services[str].toString());
    }

    return list;
}

