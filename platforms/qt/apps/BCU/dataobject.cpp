#include "dataobject.h"

DataObject::DataObject()
{

}

DataObject::DataObject(QString name, QString info)
{
    this->name = name;
    this->info = info;
}

DataObject::~DataObject()
{

}

QString DataObject::getName()
{
    return this->name;
}

QString DataObject::getInfo()
{
    return this->info;
}

void DataObject::setName(QString newName)
{
    this->name = newName;
    emit nameChanged();
}

void DataObject::setInfo(QString newInfo)
{
    this->info = newInfo;
    emit infoChanged();
}
