#include "controlpointgui.h"

#include <QtGui>
#include <QApplication>
#include <QDebug>

#define CURRENT_DIR QString(PROJECT_PATH).append ("/") // /home/larissa/UFAL/Labs/CompeLab_BlackBerry/Brisa/brisa-port-qt5/brisa-project/examples/BrisaControlPoint/

int main(int argc, char *argv[])
{
    qDebug() << CURRENT_DIR;

    QApplication a(argc, argv);
    controlpointgui window;

    QTimer *timer = new QTimer();

    QObject::connect(timer, SIGNAL(timeout()),&window,SLOT(processSplashScreen()));
    QObject::connect(&window, SIGNAL(timeStop()),timer,SLOT(stop()));
    QObject::connect(&window, SIGNAL(timeStop()),window.splash,SLOT(close()));

    window.splash->show();// Show the splash Screen
    timer->start(750);//Starts the timer with intervals of 500 miliseconds

    return a.exec();

}
