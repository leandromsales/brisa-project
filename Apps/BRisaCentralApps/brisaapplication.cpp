#include "brisaapplication.h"

BRisaApplication::BRisaApplication(QString newIconPath, QString newTitle, QString newDescription, QStringList newServices)
{
    description = newDescription;
    services = newServices;
    iconPath = newIconPath;
    title = newTitle;
}

BRisaApplication::~BRisaApplication(){}

QStringList BRisaApplication::getServices() const{    return services; }

QString BRisaApplication::getDescription() const{    return description;    }

QString BRisaApplication::getIconPath() const{    return iconPath;  }

QString BRisaApplication::getTitle() const{    return title;    }
