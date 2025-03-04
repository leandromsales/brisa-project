#include <QApplication>
#include <QDebug>
#include "light.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget window;

    QTimer *timer = new QTimer();

    QObject::connect(timer, SIGNAL(timeout()),&window,SLOT(processSplashScreen()));
    QObject::connect(&window, SIGNAL(timeStop()),timer,SLOT(stop()));
    QObject::connect(&window, SIGNAL(timeStop()),window.splash,SLOT(close()));

    window.splash->show();// Show the splash Screen
    timer->start(750);//Starts the timer with intervals of 500 miliseconds

    window.show();
    return a.exec();
}
