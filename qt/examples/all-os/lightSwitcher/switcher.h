#include <QtCore>

#include <BrisaUpnp/BrisaControlPoint>

using namespace Brisa;

class LightSwitcher : public QObject {
    Q_OBJECT

    public:
        LightSwitcher(QObject *parent=0);
        ~LightSwitcher();
        void start();

    private slots:
        void deviceFound(BrisaControlPointDevice *device);
        void serviceCall(QString t, QString method);
        void lightStatusNotification(BrisaEventProxy*, QMap<QString, QString> eventingVariables);

    private:
        BrisaControlPointDevice *dev;
        void dumpDeviceInfo(BrisaControlPointDevice *device);

        BrisaControlPoint *controlPoint;
};

