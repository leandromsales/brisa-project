#ifndef BCAJSON_H
#define BCAJSON_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QList>
#include <QFile>


using namespace std;

class BCAJson
{
public:
    BCAJson(QString path);
    ~BCAJson();

    QList<QString> *toBRisaApp();

private:

    QJsonObject mainObject;

};

#endif // BCAJSON_H
