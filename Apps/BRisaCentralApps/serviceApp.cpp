#include "serviceApp.h"

ServiceApp::ServiceApp(QString newTitle, QString newDescription)
{
    title = newTitle;
    description = newDescription;
}

ServiceApp::~ServiceApp(){ }

QString ServiceApp::getTitle()
{
    return title;
}

QString ServiceApp::getDescription()
{
    return description;
}

QStringList ServiceApp::getString()
{
    QStringList aux;

    aux.append(getTitle());
    aux.append(getDescription());

    return aux;
}

