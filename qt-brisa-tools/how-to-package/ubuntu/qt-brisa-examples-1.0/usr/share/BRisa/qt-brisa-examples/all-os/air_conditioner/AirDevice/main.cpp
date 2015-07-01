#include <QtGui/QApplication>
#include "airdevice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AirDevice w;
    w.show();
    return a.exec();
}
