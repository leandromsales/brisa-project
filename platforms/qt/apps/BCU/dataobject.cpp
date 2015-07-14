#include "dataobject.h"

DataObject::DataObject()
{

}

DataObject::DataObject(QString name, QString info, QUrl iconURL, QUrl appURL)
{
    this->name = name;
    this->info = info;
    this->iconURL = iconURL;
    this->appURL = appURL;
}

DataObject::~DataObject()
{
    this->name = "";
    this->info = "";
    this->iconURL = "";
    this->appURL = "";
}

QString DataObject::getName()
{
    return this->name;
}

QString DataObject::getInfo()
{
    return this->info;
}

QString DataObject::getIconURL()
{
    return this->iconURL;
}

QString DataObject::getAppURL()
{
    return this->appURL;
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

void DataObject::setIconURL(QString newIcon)
{
    this->iconURL = newIcon;
}

void DataObject::setAppURL(QString newApp)
{
    this->appURL = newApp;
}
