#include <QtGui/QApplication>
#include "aircontrolpoint.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AirControlPoint w;
    w.show();
    return a.exec();
}
