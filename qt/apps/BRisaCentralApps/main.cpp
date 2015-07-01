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

    QList<ServiceApp *>  list;
    list.append(new ServiceApp("Play","Play the video"));
    list.append(new ServiceApp("Pause","Stop the video"));
    list.append(new ServiceApp("Share","Share the video on your social networks"));

    BRisaApplication *application = new BRisaApplication("http://files.softicons.com/download/android-icons/flex-icons-by-prakhar-mishra/png/128x128/youtube.png",
                                                 "Youtube",
                                                 "A Streamer of videos",
                                                 list);

    QList<ServiceApp *> list2;
    list2.append(new ServiceApp("Play", "Play the music"));
    list2.append(new ServiceApp("Pause", "Pause the music"));
    list2.append(new ServiceApp("Share", "Share the music on your social networks"));
    list2.append(new ServiceApp("Add", "Add the music on your playlist"));
    list2.append(new ServiceApp("Repeat", "Repeat the music"));
    list2.append(new ServiceApp("Aleatory", "Play an aleatory music"));

    BRisaApplication *application2 = new BRisaApplication("http://files.softicons.com/download/application-icons/spotify-icon-by-iiro-jappinen/png/128x128/Spotify.png",
                                                 "Spotify",
                                                 "A Streamer of Music",
                                                 list2);

    QList<ServiceApp *>  list3;
    list3.append(new ServiceApp("Play","Play the music"));
    list3.append(new ServiceApp("Pause","Stop the music"));
    list3.append(new ServiceApp("Share","Share the music on your social networks"));
    list3.append(new ServiceApp("Add","Add the music on your playlist"));
    list3.append(new ServiceApp("Repeat","Repeat the music"));
    list3.append(new ServiceApp("Aleatory","Play an aleatory music"));

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
