#include "brisaapplication.h"

BRisaApplication::BRisaApplication(QVariantMap app)
{
    QDir dir(app["dirPath"].toString());
    BCAJson bcaJson(app["descriptionFile"].toString());
    QVariantMap descriptionFileMap = bcaJson.toBRisaApp();
    m_services = new QQmlObjectListModel<ServiceApp>(this,"title","title");
    m_title = descriptionFileMap["Title"].toString();
    m_description = descriptionFileMap["Description"].toString();
    m_iconPath = "file:///" + app["iconPath"].toString();

    if(
            descriptionFileMap["Type"].toString() != "WebApp" &&
            descriptionFileMap["Type"].toString() != "QMLApp"
            ) qFatal(QString("TYPE WRONG " + m_title + "!").toLatin1());

    if(descriptionFileMap["Type"].toString() == "WebApp")
        m_execPath = descriptionFileMap["execPath"].toString();
    if(descriptionFileMap["Type"].toString() == "QMLApp")
        m_execPath = "file:///" + dir.absoluteFilePath(descriptionFileMap["execPath"].toString());

    m_type = (descriptionFileMap["Type"].toString() == "WebApp") ? WebApp : QMLApp;
    QVariantList services = descriptionFileMap["Services"].toList();

    foreach (QVariant s, services) {
        QMap<QString,QVariant> map = s.toMap();
        if(!map.isEmpty())
            m_services->append(new ServiceApp(map.lastKey(),map.last().toString()));
    }
}
