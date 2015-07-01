#include <QtGui/QApplication>

#include "brisamediarenderer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Brisa Media Renderer");

    BrisaMediaRenderer mr;
    mr.show();

    return a.exec();
}
