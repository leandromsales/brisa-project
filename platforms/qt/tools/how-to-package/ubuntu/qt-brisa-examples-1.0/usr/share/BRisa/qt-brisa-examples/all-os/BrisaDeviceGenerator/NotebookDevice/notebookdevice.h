#ifndef NOTEBOOKDEVICE_H_
#define NOTEBOOKDEVICE_H_

#include <BrisaUpnp/BrisaDevice>

using namespace Brisa;

class NotebookDevice: public BrisaDevice {
Q_OBJECT

public:
    NotebookDevice();

public slots:
    void reducePower();

private:
    QTimer * timer;
};

#endif /* NOTEBOOKDEVICE_H_ */
