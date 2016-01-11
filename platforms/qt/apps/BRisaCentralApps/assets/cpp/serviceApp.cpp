#include "serviceApp.h"

ServiceApp::ServiceApp(QString newTitle, QString newDescription) :
    m_title(newTitle), m_description(newDescription) { }

QStringList ServiceApp::getString()
{
    QStringList aux;
    aux.append(get_title());
    aux.append(get_description());
    return aux;
}

