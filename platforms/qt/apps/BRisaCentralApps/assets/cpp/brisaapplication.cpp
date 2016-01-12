#include "brisaapplication.h"

BRisaApplication::BRisaApplication(QVariantMap app, QDir dir)
{
    m_services = new QQmlObjectListModel<ServiceApp>(this,"title","title");
    m_title = app["Title"].toString();
    m_description = app["Description"].toString();
    m_url = "file:///" + dir.absoluteFilePath(app["Url"].toString());
    m_iconPath = "file:///" + dir.absoluteFilePath(app["Icon"].toString());
    m_mainQMLFile = "file:///" + dir.absoluteFilePath(app["MainQMLFile"].toString());

    if(app["Type"].toString() != "WebApp" && app["Type"].toString() != "QMLApp") qFatal(QString("TYPE WRONG " + m_title + "!").toLatin1());
    m_type = (app["Type"].toString() == "WebApp") ? WebApp : QMLApp;
    QVariantList services = app["Services"].toList();

    foreach (QVariant s, services) {
        QMap<QString,QVariant> map = s.toMap();
        if(!map.isEmpty())
            m_services->append(new ServiceApp(map.lastKey(),map.last().toString()));
    }
}
