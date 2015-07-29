#ifndef CONTROLPOINTGUI_H
#define CONTROLPOINTGUI_H

#include <QtGui/QWidget>
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
#include <BrisaUpnp/BrisaControlPoint>
#include "ui_controlpointgui.h"


using namespace Brisa;

class controlpointgui : public QMainWindow,private Ui::controlpointFormCP
{
    Q_OBJECT

public:
    controlpointgui(QWidget *parent = 0);
    ~controlpointgui();
    void addItem(BrisaControlPointDevice *device);
    BrisaControlPointDevice* getDeviceByUDN(QString UDN);
    QSplashScreen *splash;

signals:
    void timeStop();

public slots:
    void lineEnabled(QTreeWidgetItem * item, int collumn);
    void deviceFoundDump(BrisaControlPointDevice *device);
    void serviceCall(QString, QString);
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
    void changeEventLog(BrisaEventProxy *subscription,QMap<QString, QString> eventVariables);

private:
    void createActions();
    void createMenus();
    void createToolBars();

    QList<BrisaControlPointDevice*> devices;
    QList<QTreeWidgetItem*> items;

    BrisaControlPoint *controlPoint;

    QToolBar *toolBar;
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QDialog *dialog;
    QList<QLineEdit*> editors;
    QList<QLabel*> labels;
    QMap<int, QList<QString> > eventToDevice;

    QAction *clearAction;
    QAction *exitAction;
    QAction *expandAction;
    QAction *collapseAction;
    QAction *aboutCpAction;
    QAction *aboutQtAction;
    QAction *aboutBrisaAction;
    QAction *aboutUpnpAction;

    QTreeWidgetItem *networkItem;

    QTreeWidgetItem* selected;

    bool doubleClick;
    bool closeable;

    int contSplashScreen;
};

#endif // CONTROLPOINTGUI_H
