#include <QtDebug>
#include <BrisaUtils/BrisaLog>

#include "switcher.h"

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    brisaLogInitialize();

    LightSwitcher controlPoint;
    controlPoint.start();

    return app.exec();
}
