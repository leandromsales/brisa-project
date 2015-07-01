#include "brisaapplication.h"

BRisaApplication::BRisaApplication(QString newIconPath, QString newTitle, QString newDescription, QList<ServiceApp *> newServiceApps)
{
    description = newDescription;
    services = newServiceApps;
    iconPath = newIconPath;
    title = newTitle;
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
