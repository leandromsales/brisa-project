#ifndef BCAJSON_H
#define BCAJSON_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QDebug>
#include <QList>
#include <fstream>

using namespace std;
class BCAJson
{
public:
    BCAJson(const char *path);
    QList<QString> *toBRisaApp();
private:
    QJsonObject mainObject;
};

#endif // BCAJSON_H
