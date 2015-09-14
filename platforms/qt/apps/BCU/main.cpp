#include <QQmlContext>
#include <QQuickView>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>

#include "controlpointbcu.h"

using namespace brisa;
using namespace upnp;
using namespace controlpoint;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ControlPointBCU *controlPoint = new ControlPointBCU();
    controlPoint->start();
    controlPoint->discover();

    return app.exec();
}
