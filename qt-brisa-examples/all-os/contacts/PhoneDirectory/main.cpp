#include <QtGui/QApplication>
#include "phoneDirectoryDevice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PhoneDirectoryDevice w;
    w.show();
    return a.exec();
}
