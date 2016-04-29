#include "dataobject.h"

DataObject::DataObject()
{}

DataObject::DataObject(QString udn, QString name, QString info, QUrl iconURL, QUrl appURL):
    m_name(name), m_info(info), m_iconURL(iconURL), m_appURL(appURL), m_udn(udn)
{}

DataObject::~DataObject()
{
    this->m_name = "";
    this->m_info = "";
    this->m_iconURL = "";
    this->m_appURL = "";
    this->m_udn = m_udn;
}
