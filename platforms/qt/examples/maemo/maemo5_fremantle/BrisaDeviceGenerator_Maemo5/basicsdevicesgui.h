#ifndef BASICSDEVICESGUI_H
#define BASICSDEVICESGUI_H

#include <QtGui/QMainWindow>
#include <BrisaUpnp/BrisaDevice>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QProgressDialog>
#include <QProgressBar>
#include <QTimeLine>
#include <QToolBar>
#include <QTimer>
#include <QSplashScreen>
#include "ui_basicsdevicesgui.h"

using namespace Brisa;

class BasicDeviceGui : public QMainWindow, private Ui::BasicDeviceGuiClass
{
    Q_OBJECT

public:
    BasicDeviceGui(QWidget *parent = 0);
    ~BasicDeviceGui();
    QSplashScreen *splash;

signals:
    void timeStop();

public slots:
    void stopAllDevices();
    void startProgress();
    void pdProgress();
    void pdCancel();
    void aboutBDG();
    void aboutBrisa();
    void aboutUpnp();
    void setTagD1();
    void setTagD2();
    void setTagD3();
    void setTagD4();
    void setTagD5();
    void setTagD6();
    void processSplashScreen();

private:
    QList<BrisaDevice*> deviceList;
    QList<QLabel*> labelImageList;
    QList<QLabel*> labelNameList;

    //METHODS TO INITIALIZE THE MAINWINDOW
    void createActions();
    void createMenus();
    void createToolBars();
    void startDevice();
    void initializeLabelImageList();
    void initializeLabelNameList();

    //METHODS TO CREATE THE DEVICES
    void createDevice1();
    void createDevice2();
    void createDevice3();
    void createDevice4();
    void createDevice5();
    void createDevice6();

    //METHODS FOR UDN CREATION
    QString createUdn();
    int get1RandomNumber();
    int get2RandomNumber();
    int get3RandomNumber();

    QToolBar *toolBar;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QMenu *addMenu;

    QAction *addAction;;
    QAction *closeAllDevices;
    QAction *exitAction;

    QAction *deviceAction1;
    QAction *deviceAction2;
    QAction *deviceAction3;
    QAction *deviceAction4;
    QAction *deviceAction5;
    QAction *deviceAction6;

    QAction *aboutUpnpAction;
    QAction *aboutBrisaAction;
    QAction *aboutBDGAction;
    QAction *aboutQtAction;

    int contDevices;
    int contSteps;
    QTimer *t;
    QProgressDialog *pd;
    int tagD;
    int contSplashScreen;

};

#endif // BASICSDEVICESGUI_H

