#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQmlContext>

#include "bcadevice.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    QQmlApplicationEngine engine;

    QQmlContext *ctxt = engine.rootContext();

    BRisaApplicationManager *manager = new BRisaApplicationManager();

    QStringList list;
    list.append("Play");
    list.append("Play the video");
    list.append("Pause");
    list.append("Stop the video");
    list.append("Share");
    list.append("Share the video on your social networks");

    BRisaApplication *application = new BRisaApplication("http://files.softicons.com/download/android-icons/flex-icons-by-prakhar-mishra/png/128x128/youtube.png",
                                                 "Youtube",
                                                 "A Streamer of videos",
                                                 list);

    QStringList list2;
    list2.append("Play");
    list2.append("Play the music");
    list2.append("Pause");
    list2.append("Stop the music");
    list2.append("Share");
    list2.append("Share the music on your social networks");
    list2.append("Add");
    list2.append("Add the music on your playlist");
    list2.append("Repeat");
    list2.append("Repeat the music");
    list2.append("Aleatory");
    list2.append("Play an aleatory music");

    BRisaApplication *application2 = new BRisaApplication("http://files.softicons.com/download/application-icons/spotify-icon-by-iiro-jappinen/png/128x128/Spotify.png",
                                                 "Spotify",
                                                 "A Streamer of Music",
                                                 list2);

    QStringList list3;
    list3.append("Play");
    list3.append("Play the music");
    list3.append("Pause");
    list3.append("Stop the music");
    list3.append("Share");
    list3.append("Share the music on your social networks");
    list3.append("Add");
    list3.append("Add the music on your playlist");
    list3.append("Repeat");
    list3.append("Repeat the music");
    list3.append("Aleatory");
    list3.append("Play an aleatory music");

    BRisaApplication *application3 = new BRisaApplication("http://files.softicons.com/download/application-icons/circle-icons-by-martz90/png/128x128/deviantart.png",
                                                 "DeviantArt",
                                                 "A gallery online",
                                                 list3);



    manager->addApp(application);
    manager->addApp(application2);
    manager->addApp(application3);

    BCADevice *bca = new BCADevice(manager);
    bca->printAllApps();

    ctxt->setContextProperty("manager", manager);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
