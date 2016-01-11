#include "bcajson.h"

BCAJson::BCAJson(const char *path)
{
    ifstream i(path);

    //Get file size
    i.seekg (0, i.end);
    int length = i.tellg();
    i.seekg (0, i.beg);

    //Creating a buffer
    char * buffer = new char[length];
    i.read(buffer,length);

    QString str(buffer);

    //Remove the trash
    if(str.length() != length) {
        str.remove(length, str.length() - length);
    }

    //Reading the JSON
    QJsonDocument d = QJsonDocument::fromJson(QString(str).toUtf8());
    mainObject = d.object();
}
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

