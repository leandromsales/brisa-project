#ifndef CONTROLPOINTGUI_H
#define CONTROLPOINTGUI_H

#include <QWidget>
#include <QAction>
#include <QMenu>
#include <QIcon>
#include <QString>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QToolBar>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QSplashScreen>
#include <QStringList>
#include <QPixmap>

#include "ui_controlpointgui.h"

#include "upnp/action.h"

#define DEVICE_UUID_SEPARATOR "?content"

namespace brisa {
namespace upnp {
namespace controlpoint {

class ControlPoint;
class EventProxy;
class Device;

}
}
}

class controlpointgui : public QMainWindow,private Ui::controlpointFormCP
{
    Q_OBJECT

public:
    controlpointgui(QWidget *parent = 0);
    ~controlpointgui();

    void createDeviceItem();
    brisa::upnp::controlpoint::Device* getDeviceByUDN(QString UDN);

    QSplashScreen *splash;

signals:
    void timeStop();

public slots:
    void onReadyDownloadIcons(brisa::upnp::controlpoint::Device*);
    void lineEnabled(QTreeWidgetItem * item, int collumn);
    void deviceFoundDump(brisa::upnp::controlpoint::Device *device);
    void serviceCall(OutArgument, QString);
    void requestError(QString errorMessage, QString methodName);
    void removeDevice(QString usn);
    void callMethod();
    void expandItems();
    void collapseItems();
    void aboutControlPoint();
    void aboutUpnp();
    void aboutBrisa();
    void processSplashScreen();
    void call(QTreeWidgetItem * item, int collumn);
    void mountControlDialog();
    void changeEventLog(brisa::upnp::controlpoint::EventProxy *subscription,QMap<QString, QString> eventVariables);
    void clearEventLog();
    void multicastEventReceived(QString variableName, QString newValue);
    void multicastEventRawReceived(OutArgument raw);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void setUpTableWidget();
    void addItem(QTreeWidgetItem *deviceItem);

    QList<brisa::upnp::controlpoint::Device*> devices;
    QList<QTreeWidgetItem*> items;

    brisa::upnp::controlpoint::ControlPoint *controlPoint;
    brisa::upnp::controlpoint::Device *currentDev;
    QTreeWidgetItem *currentDeviceItem;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QDialog *dialog;
    QList<QLineEdit*> editors;
    QList<QLabel*> labels;
    QMap<QString, QList<QString> > eventToDevice;

    QAction *clearAction;
    QAction *exitAction;
    QAction *expandAction;
    QAction *collapseAction;
    QAction *aboutCpAction;
    QAction *aboutQtAction;
    QAction *aboutBrisaAction;
    QAction *aboutUpnpAction;

    QTreeWidgetItem *networkItem;
    QTreeWidgetItem *selected;

    bool doubleClick;
    bool closeable;

    int contSplashScreen;
};

#endif // CONTROLPOINTGUI_H
