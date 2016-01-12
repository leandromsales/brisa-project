#include "brisaapplication.h"

BRisaApplication::BRisaApplication(QString newIconPath, QString newTitle, QString newDescription, QList<ServiceApp *> newServiceApps)
{
    m_description = newDescription;
    m_services = newServiceApps;
    m_iconPath = newIconPath;
    m_title = newTitle;
}

BRisaApplication::BRisaApplication(QString newIconPath, QString newTitle, QString pathDir, QList<QString> *jsonFile)
{
    m_title = newTitle;
    m_iconPath = newIconPath;

    m_url = pathDir + "/" + m_title + ".compe";
    m_mainQMLFile = pathDir + "/main.qml";

    m_description = jsonFile->at(0);
    jsonFile->removeAt(0);

    for(int i = 0; i < jsonFile->size(); i += 2) {
        m_services.append(new ServiceApp(jsonFile->at(i), jsonFile->at(i + 1)));
    }
}

QStringList BRisaApplication::getString() const
{
    QStringList aux;

    foreach (ServiceApp *s, m_services) {
        aux.append(s->getString());
    }

    return aux;
}
