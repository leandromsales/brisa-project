#include "brisaapplication.h"

BRisaApplication::BRisaApplication(QString newIconPath, QString newTitle, QString newDescription, QList<ServiceApp *> newServiceApps)
{
    description = newDescription;
    services = newServiceApps;
    iconPath = newIconPath;
    title = newTitle;
}

BRisaApplication::BRisaApplication(QString newIconPath, QString newTitle, QString pathDir, QList<QString> *jsonFile)
{
    title = newTitle;
    iconPath = newIconPath;

    url = pathDir + "/" + title + ".compe";
    mainQMLFile = pathDir + "/main.qml";

    description = jsonFile->at(0);
    jsonFile->removeAt(0);

    for(int i = 0; i < jsonFile->size(); i += 2) {
        services.append(new ServiceApp(jsonFile->at(i), jsonFile->at(i + 1)));
    }
}

BRisaApplication::~BRisaApplication(){}

QStringList BRisaApplication::getString() const
{
    QStringList aux;

    foreach (ServiceApp *s, services) {
        aux.append(s->getString());
    }

    return aux;
}

QList<ServiceApp *> BRisaApplication::getServices() {    return services;  }

QString BRisaApplication::getDescription() const{    return description;    }

QString BRisaApplication::getIconPath() const{    return iconPath;  }

QString BRisaApplication::getTitle() const{    return title;    }

QString BRisaApplication::getUrl() const{   return url;     }

QString BRisaApplication::getMainQMLFile() const{    return mainQMLFile;   }
