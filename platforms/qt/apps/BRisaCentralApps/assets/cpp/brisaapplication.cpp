#include "brisaapplication.h"

BRisaApplication::BRisaApplication(QVariantMap app)
{
    QDir dir(app["dirPath"].toString());
    BCAJson bcaJson(app["descriptionFile"].toString());
    QVariantMap descriptionFileMap = bcaJson.toBRisaApp();
//    m_services = new QQmlObjectListModel<ServiceApp>(this,"title","title");
    m_title = descriptionFileMap["Title"].toString();
    m_description = descriptionFileMap["Description"].toString();
    m_iconPath = app["iconPath"].toString();

    if(
            descriptionFileMap["Type"].toString() != "WebApp" &&
            descriptionFileMap["Type"].toString() != "QMLApp"
            ) qFatal(QString("TYPE WRONG " + m_title + "!\nPlease, delete /apps/apps.json and try again").toLatin1());

    if(descriptionFileMap["Type"].toString() == "WebApp") {
        m_execPath = dir.absoluteFilePath("main.qml");
        this->setProperty("url",descriptionFileMap["execPath"].toString());
        this->setProperty("dir",dir.absolutePath());
        createWebviewQML();
    }
    else if(descriptionFileMap["Type"].toString() == "QMLApp")
        m_execPath = dir.absoluteFilePath(descriptionFileMap["execPath"].toString());

    m_type = (descriptionFileMap["Type"].toString() == "WebApp") ? WebApp : QMLApp;
//    QVariantList services = descriptionFileMap["Services"].toList();

//    foreach (QVariant s, services) {
//        QMap<QString,QVariant> map = s.toMap();
//        if(!map.isEmpty())
//            m_services->append(new ServiceApp(map.lastKey(),map.last().toString()));
//    }
}

QJsonObject BRisaApplication::toJsonObject()
{
    QJsonObject jsonApp;
    jsonApp.insert("Title", get_title());
    jsonApp.insert("Icon",get_iconRelPath());
    jsonApp.insert("Description",get_description());

//    QList<ServiceApp *> services = get_services()->toList();
//    QJsonArray jsonServices;

//    foreach (ServiceApp *s, services) {
//        QJsonObject jsonService;
//        jsonService.insert(s->get_title(), QJsonValue(s->get_description()));
//        jsonServices.append(jsonService);
//    }

//    jsonApp.insert("Services",QJsonValue(jsonServices));
    return jsonApp;
}

QString BRisaApplication::compePath()
{
    qDebug() << get_execPath();
    QString path = get_execPath().replace("main.qml","");
    QDir appDir(path);
    QFile compeFile(path + get_title() + ".compe");
    qDebug() << "CHECKING IF .COMPE FILE EXISTS!";
    if(compeFile.exists()) {
        qDebug() << "DELETING OLD .COMPE FILE";
        if(!appDir.remove(get_title() + ".compe")) return "ERROR DELETING OLD .COMPE";
    }

    qDebug() << "GENERATING A NEW .COMPE FILE";
    FolderCompressor folderCmp(this);
    QString srcFolder = path.replace("file:///","");
    QString destination = path.replace("file:///","") + get_title() + ".compe";
    if(!folderCmp.compressFolder(srcFolder,destination))
        return "ERROR FOLDER COMPRESSING";

    qDebug() << ".COMPE GENERATED WITH SUCESS. SENDING TO BCU";
    return path+ get_title()+".compe";
}

bool BRisaApplication::createWebviewQML()
{
    qDebug() << "CREATING THE WEBVIEW QML TO" << m_title;
    QFile webViewFile(m_execPath);
    QDir d(this->property("dir").toString());
    if(webViewFile.exists()) return true;
    if(!webViewFile.open(QIODevice::WriteOnly)) qFatal("CREATE WEBVIEW QML COULDNT BE OPENED!");
    QString libs = QString("import QtQuick 2.6\n") +
                   QString("import QtQuick.Controls 2.0\n") +
                   QString("import QtWebView 1.1\n\n");
    QString code = QString("Item {\n")+
                   QString("\tWebView {\n")+
                   QString("\t\tanchors.fill:parent\n")+
                   QString("\t\turl:Qt.resolvedUrl(\"" + this->property("url").toString()) +
                   QString("\")\n\t}\n}");
    if(webViewFile.write((libs+code).toLatin1()) == -1) qFatal("CREATE WEBVIEW QML COULDNT WRITE IN THE FILE!");
    webViewFile.close();
    return true;
}
