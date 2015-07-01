#ifndef LIGHTSWITCHER_H
#define LIGHTSWITCHER_H

#include "ui_lightSwitcher.h"
#include <QtGui/QMainWindow>
#include <QtDebug>
#include <BrisaUpnp/BrisaControlPoint>
#include <QSplashScreen>

using namespace Brisa;

namespace Ui
{
    class MainWindow;
}

class LightSwitcher : public QMainWindow , public  Ui::MainWindow
{
    Q_OBJECT

public:
    LightSwitcher(QMainWindow *parent = 0);
    ~LightSwitcher();
    BrisaControlPointDevice* getDeviceByUdn(QString udn);
    QSplashScreen *splash;

signals:
    void timeStop();

public slots:
    void getTargetResponse(QString response, QString method);
    void getStatusResponse(QString response, QString method);
    void getTargetAction();
    void getStatusAction();
    void setTargetAction();
    void deviceFoundDump(BrisaControlPointDevice *device);
    void removeDevice(QString udn);
    void processSplashScreen();

private:
    BrisaControlPoint *controlPoint;
    QList<BrisaControlPointDevice*> devicesList;
    int contSplashScreen;
};

#endif // LIGHTSWITCHER_H
