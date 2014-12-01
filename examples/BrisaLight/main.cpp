//#include <QApplication>
//#include "light.h"

//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    Widget window;

//    QTimer *timer = new QTimer();

//    QObject::connect(timer, SIGNAL(timeout()),&window,SLOT(processSplashScreen()));
//    QObject::connect(&window, SIGNAL(timeStop()),timer,SLOT(stop()));
//    QObject::connect(&window, SIGNAL(timeStop()),window.splash,SLOT(close()));

//    window.splash->show();// Show the splash Screen
//    timer->start(750);//Starts the timer with intervals of 500 miliseconds



//    window.show();
//    return a.exec();
//}

#include <QApplication>
#include "shared/webserver/webserver.h"
#include "network/brisanetwork.h"
#include <QDebug>

using namespace brisa;
using namespace network;
using namespace brisa::shared::webserver;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // QString ipAddress = "172.20.8.208";
    QString ipAddress = "192.168.1.113";
    quint16 port = 58136;
    qDebug() << "Address " << ipAddress + ":" + QString::number(port);

    Webserver *ws = new Webserver(QHostAddress(ipAddress), port);
    ws->start ();

    qDebug() << ws->isListening();
    return a.exec();
}


