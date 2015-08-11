#include "upnpcalculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UpnpCalculator w(NULL);

    QTimer *timer = new QTimer();

    QObject::connect(timer, SIGNAL(timeout()),&w,SLOT(processSplashScreen()));
    QObject::connect(&w, SIGNAL(timeStop()),timer,SLOT(stop()));
    QObject::connect(&w, SIGNAL(timeStop()),w.splash,SLOT(close()));

    w.splash->show();// Show the splash Screen
    timer->start(750);//Starts the timer with intervals of 500 miliseconds

    w.show();

    return a.exec();
}
