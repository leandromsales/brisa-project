#include <QDebug>
#include <QMessageBox>
#include <typeinfo>
#include "main.h"
#include "lightSwitcher.h"

MyQApplication::MyQApplication(int argc, char** argv): QApplication(argc, argv) {
	this->countQtEventLoop = 0;
}

bool MyQApplication::notify(QObject * receiver, QEvent * event) {
    QString receiverName;
    receiverName = (receiver->objectName() == "") ? "<unknown object name>" : receiver->objectName();
    qDebug("Sending event %s to object %s (%s)", 
            typeid(*event).name(), qPrintable(receiverName),
            typeid(*receiver).name());
    try {
        return QApplication::notify(receiver, event);
    } catch (std::exception &e) {
        qFatal("Error %s sending event %s to object %s (%s)", 
            e.what(), typeid(*event).name(), qPrintable(receiver->objectName()),
            typeid(*receiver).name());
    } catch (...) {
        qFatal("Error <unknown> sending event %s to object %s (%s)", 
            typeid(*event).name(), qPrintable(receiver->objectName()),
            typeid(*receiver).name());
    }        

    // qFatal aborts, so this isn't really necessary
    // but you might continue if you use a different logging lib
    return false;

}

int main(int argc, char *argv[])
{
    MyQApplication a(argc, argv);
    LightSwitcher window;

    QTimer *timer = new QTimer();

    QObject::connect(timer, SIGNAL(timeout()),&window,SLOT(processSplashScreen()));
    QObject::connect(&window, SIGNAL(timeStop()),timer,SLOT(stop()));
    QObject::connect(&window, SIGNAL(timeStop()),window.splash,SLOT(close()));

    window.splash->show();// Show the splash Screen
    timer->start(750);//Starts the timer with intervals of 500 miliseconds

    //w.show();
    return a.exec();
}
