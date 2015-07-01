#include <QtGui/QApplication>
#include "phoneDirectoryControlPoint.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PhoneDirectoryControlPoint w;
    w.show();
    return a.exec();
}
