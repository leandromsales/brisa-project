#include "simpledevice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SimpleDevice *simpleDevice = new SimpleDevice();
    simpleDevice->show();

    return a.exec();
}
