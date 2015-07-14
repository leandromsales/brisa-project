#include "dataobject.h"

DataObject::DataObject()
{

}

DataObject::DataObject(QString udn, QString name, QString info, QUrl iconURL, QUrl appURL)
{
    this->name = name;
    this->info = info;
    this->iconURL = iconURL;
    this->appURL = appURL;
    this->udn = udn;
}

DataObject::~DataObject()
{
    this->name = "";
    this->info = "";
    this->iconURL = "";
    this->appURL = "";
}

QString DataObject::getUdn()
{
    return this->udn;
}

QString DataObject::getName()
{
    return this->name;
}

QString DataObject::getInfo()
{
    return this->info;
}

QUrl DataObject::getIconURL()
{
    return this->iconURL;
}

QUrl DataObject::getAppURL()
{
    return this->appURL;
}

void DataObject::setUdn(QString newUdn)
{
    this->udn = newUdn;
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

void DataObject::setIconURL(QUrl newIcon)
{
    this->iconURL = newIcon;
}

void DataObject::setAppURL(QUrl newApp)
{
    this->appURL = newApp;
}
