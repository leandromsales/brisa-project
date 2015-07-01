#include <ctime>
#include <QDebug>
#include <BrisaUtils/BrisaLog>
#include "basicsdevicesgui.h"
#include "EletronicDoorDevice/eletronicdoordevice.h"
#include "BinaryLightDevice/binarylightdevice.h"
#include "ConsoleGameDevice/consolegamedevice.h"
#include "DesktopComputerDevice/desktopcomputerdevice.h"
#include "NotebookDevice/notebookdevice.h"
#include "SmartPhoneDevice/smartphonedevice.h"

using namespace Brisa;

BasicDeviceGui::BasicDeviceGui(QWidget *parent) :
    QMainWindow(parent) {
    setupUi(this);
    createActions();
    createMenus();
    createToolBars();
    labelRoteador->setPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/roteador.png"));
    splash
            = new QSplashScreen(
                    QPixmap(
                            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/BrisaSplashScreenDevice.png"));

    pd = new QProgressDialog("Adding...", "Abort", 0, 100, this);
    pd->setWindowTitle("Add a Brisa Device");
    pd->setMinimumSize(250, 100);
    pd->setMaximumSize(250, 100);
    connect(pd, SIGNAL(canceled()), this, SLOT(pdCancel()));

    t = new QTimer();
    t->setInterval(50);
    connect(t, SIGNAL(timeout()), this, SLOT(pdProgress()));

    brisaLogInitialize();

    initializeLabelImageList();
    initializeLabelNameList();

    contDevices = 0;
    contSteps = 0;
    contSplashScreen = 0;
}

BasicDeviceGui::~BasicDeviceGui() {
    stopAllDevices();
}

void BasicDeviceGui::createActions() {
    //FILE MENU ACTION
    exitAction = new QAction(tr("Quit"), this);
    exitAction->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/exit.png"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    closeAllDevices = new QAction(tr("Close All Devices"), this);
    closeAllDevices->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/clear.png"));
    connect(closeAllDevices, SIGNAL(triggered()), this, SLOT(stopAllDevices()));

    //DEVICE ACTIONS
    deviceAction1 = new QAction(tr("Binary Light"), this);
    deviceAction1->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/lightgreen.png"));
    deviceAction1->setToolTip("Create a Binary Light Device");
    connect(deviceAction1, SIGNAL(triggered()), this, SLOT(setTagD1()));
    connect(deviceAction1, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction2 = new QAction(tr("Eletronic Door"), this);
    deviceAction2->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/door.png"));
    deviceAction2->setToolTip("Create a Eletronic Door Device");
    connect(deviceAction2, SIGNAL(triggered()), this, SLOT(setTagD2()));
    connect(deviceAction2, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction3 = new QAction(tr("Desktop Computer"), this);
    deviceAction3->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/pc2.png"));
    deviceAction3->setToolTip("Create a Desktop Computer Device");
    connect(deviceAction3, SIGNAL(triggered()), this, SLOT(setTagD3()));
    connect(deviceAction3, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction4 = new QAction(tr("SmartPhone"), this);
    deviceAction4->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/smartphone.png"));
    deviceAction4->setToolTip("Create a SmartPhone Device");
    connect(deviceAction4, SIGNAL(triggered()), this, SLOT(setTagD4()));
    connect(deviceAction4, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction5 = new QAction(tr("Notebook"), this);
    deviceAction5->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/notebook.png"));
    deviceAction5->setToolTip("Create a Notebook Device");
    connect(deviceAction5, SIGNAL(triggered()), this, SLOT(setTagD5()));
    connect(deviceAction5, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction6 = new QAction(tr("Game Console"), this);
    deviceAction6->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/console.png"));
    deviceAction6->setToolTip("Create a Console Game Device");
    connect(deviceAction6, SIGNAL(triggered()), this, SLOT(setTagD6()));
    connect(deviceAction6, SIGNAL(triggered()), this, SLOT(startProgress()));

    //ABOUT ACTIONS
    aboutBDGAction = new QAction(tr("About Basic Device Gui"), this);
    aboutBDGAction->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/device.png"));
    aboutBDGAction->setToolTip("Informations of Basic Device Gui");
    connect(aboutBDGAction, SIGNAL(triggered()), this, SLOT(aboutBDG()));

    aboutBrisaAction = new QAction(tr("About Brisa"), this);
    aboutBrisaAction->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/brisa.png"));
    aboutBrisaAction->setToolTip("Informations of Brisa");
    connect(aboutBrisaAction, SIGNAL(triggered()), this, SLOT(aboutBrisa()));

    aboutUpnpAction = new QAction(tr("About UPnP"), this);
    aboutUpnpAction->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/upnp.png"));
    aboutUpnpAction->setToolTip("Informations of UPnP");
    connect(aboutUpnpAction, SIGNAL(triggered()), this, SLOT(aboutUpnp()));

    aboutQtAction = new QAction(tr("About QT"), this);
    aboutQtAction->setIcon(QIcon(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/qt.png"));
    aboutQtAction->setToolTip("Informations of QT");
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void BasicDeviceGui::startProgress() {
    if (contDevices == 8) {
        this->limitDevicesError();
        return;
    }

    setEnabled(false);
    pd->setEnabled(true);
    t->start();
}

void BasicDeviceGui::createMenus() {
    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(closeAllDevices);
    fileMenu->addAction(exitAction);

    addMenu = menubar->addMenu(tr("&Add Device"));
    addMenu->addAction(deviceAction1);
    addMenu->addAction(deviceAction2);
    addMenu->addAction(deviceAction3);
    addMenu->addAction(deviceAction4);
    addMenu->addAction(deviceAction5);
    addMenu->addAction(deviceAction6);

    helpMenu = menubar->addMenu(tr("&Help"));

    helpMenu->addAction(aboutBDGAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutBrisaAction);
    helpMenu->addAction(aboutUpnpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAction);
}

void BasicDeviceGui::createToolBars() {
    toolBar = new QToolBar("Add Devices ToolBar", centralwidget);
    toolBar->addSeparator();
    toolBar->addAction(deviceAction1);
    toolBar->addAction(deviceAction2);
    toolBar->addAction(deviceAction3);
    toolBar->addAction(deviceAction4);
    toolBar->addAction(deviceAction5);
    toolBar->addAction(deviceAction6);
    toolBar->addSeparator();
    toolBar->addAction(closeAllDevices);
    toolBar->addSeparator();
    toolBar->addAction(exitAction);
}

void BasicDeviceGui::pdProgress() {
    pd->setValue(contSteps);
    contSteps++;
    if (contSteps > pd->maximum()) {
        t->stop();
        pd->cancel();
        contSteps = 0;
        startDevice();
        setEnabled(true);
    }
}

void BasicDeviceGui::pdCancel() {
    t->stop();
    contSteps = 0;
    setEnabled(true);
}

void BasicDeviceGui::startDevice() {
    switch (this->tagD) {
    case 1:
        createBinaryLight();
        this->tagD = 0;
        break;
    case 2:
        createEletronicDoor();
        this->tagD = 0;
        break;
    case 3:
        createDesktopComputer();
        this->tagD = 0;
        break;
    case 4:
        createSmartPhone();
        this->tagD = 0;
        break;
    case 5:
        createNotebook();
        this->tagD = 0;
        break;
    case 6:
        createConsoleGame();
        this->tagD = 0;
        break;
    }
}

void BasicDeviceGui::limitDevicesError() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Limit of Devices");
    msgBox.setText("The number Limit of Devices is 8.");
    msgBox.setIconPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/error.png"));
    msgBox.exec();
}

void BasicDeviceGui::createBinaryLight() {
    contDevices++;
    BinaryLightDevice * device = new BinaryLightDevice();
    device->start();

    labelNameList[(contDevices - 1)]->setText(device->getAttribute(
            BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices - 1)]->setPixmap(
            QPixmap(
                    "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/lightgreen2.png"));
    this->deviceList.append(device);
}

void BasicDeviceGui::createEletronicDoor() {
    contDevices++;
    EletronicDoorDevice * device = new EletronicDoorDevice();
    device->start();

    labelNameList[(contDevices - 1)]->setText(device->getAttribute(
            BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices - 1)]->setPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/door2.png"));
    this->deviceList.append(device);
}

void BasicDeviceGui::createDesktopComputer() {
    contDevices++;
    DesktopComputerDevice *device = new DesktopComputerDevice();
    device->start();

    labelNameList[(contDevices - 1)]->setText(device->getAttribute(
            BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices - 1)]->setPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/pc2.png"));
    this->deviceList.append(device);
}

void BasicDeviceGui::createSmartPhone() {
    contDevices++;
    SmartPhoneDevice *device = new SmartPhoneDevice();
    device->start();

    labelNameList[(contDevices - 1)]->setText(device->getAttribute(
            BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices - 1)]->setPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/smartphone.png"));
    this->deviceList.append(device);
}

void BasicDeviceGui::createNotebook() {
    contDevices++;
    NotebookDevice * device = new NotebookDevice();
    device->start();

    labelNameList[(contDevices - 1)]->setText(device->getAttribute(
            BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices - 1)]->setPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/notebook.png"));
    this->deviceList.append(device);
}

void BasicDeviceGui::createConsoleGame() {
    contDevices++;
    ConsoleGameDevice *device = new ConsoleGameDevice();
    device->start();

    labelNameList[(contDevices - 1)]->setText(device->getAttribute(
            BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices - 1)]->setPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/console.png"));
    this->deviceList.append(device);
}

void BasicDeviceGui::stopAllDevices() {
    for (int i = 0; i < this->deviceList.size(); i++) {
        this->deviceList[i]->stop();
    }
    for (int j = 0; j < labelImageList.size(); j++) {
        labelImageList[j]->clear();
    }
    for (int k = 0; k < labelNameList.size(); k++) {
        labelNameList[k]->clear();
    }
    contDevices = 0;
}

void BasicDeviceGui::initializeLabelImageList() {
    this->labelImageList.append(labelImage1);
    this->labelImageList.append(labelImage2);
    this->labelImageList.append(labelImage3);
    this->labelImageList.append(labelImage4);
    this->labelImageList.append(labelImage5);
    this->labelImageList.append(labelImage6);
    this->labelImageList.append(labelImage7);
    this->labelImageList.append(labelImage8);
}

void BasicDeviceGui::initializeLabelNameList() {
    this->labelNameList.append(labelName1);
    this->labelNameList.append(labelName2);
    this->labelNameList.append(labelName3);
    this->labelNameList.append(labelName4);
    this->labelNameList.append(labelName5);
    this->labelNameList.append(labelName6);
    this->labelNameList.append(labelName7);
    this->labelNameList.append(labelName8);
}

void BasicDeviceGui::setTagD1() {
    this->tagD = 1;
}

void BasicDeviceGui::setTagD2() {
    this->tagD = 2;
}

void BasicDeviceGui::setTagD3() {
    this->tagD = 3;
}

void BasicDeviceGui::setTagD4() {
    this->tagD = 4;
}

void BasicDeviceGui::setTagD5() {
    this->tagD = 5;
}

void BasicDeviceGui::setTagD6() {
    this->tagD = 6;
}

void BasicDeviceGui::aboutBDG() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("About BasicDeviceGUI");
    msgBox.setText("<h3>About BasicDeviceGUI</h3>"
        "<p>"
        "<b>BasicDeviceGui</b> is a Brisa Example with Gui "
        "to create UPnP devices");
    msgBox.setIconPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/device.png"));
    msgBox.exec();
}

void BasicDeviceGui::aboutBrisa() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Brisa");
    msgBox.setText("<center><h1>About Brisa</h1></center>"
        "<p>"
        "<center><b>BRisa</b> is a project focused on developing UPnP "
        "technologies.</center>"
        "<p>"
        "<p>"
        "<center>http://brisa.garage.maemo.org");
    msgBox.setIconPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/brisa_logo.png"));
    msgBox.exec();
}

void BasicDeviceGui::aboutUpnp() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("About UPnP");
    msgBox.setText(
            "<center><h1>About UPnP</h1></center>"
                "<b>Universal Plug and Play (UPnP)</b> is a set of networking"
                "protocols promulgated by the UPnP Forum. The goals of UPnP are to allow devices"
                "to connect seamlessly and to simplify the implementation of networks in the "
                "home (data sharing,communications, and entertainment) and in corporate "
                "environments for simplified installation of computer components. UPnP achieves "
                "this by defining and publishing UPnP device control protocols (DCP) built upon "
                "open, Internet-based communication standards."
                "<p>"
                "<p>"
                "<center>www.upnp.org");
    msgBox.setIconPixmap(QPixmap(
            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/upnp.png"));
    msgBox.exec();
}

void BasicDeviceGui::processSplashScreen() {
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    switch (contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("Setting up Brisa Basic Device..."),
                topRight, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("Waiting for Brisa Basic Device..."),
                topRight, Qt::blue);
        break;
    }

    if (contSplashScreen > 2) {
        this->show();
        splash->finish(this);
        emit
        timeStop();
        delete splash;
    }

    contSplashScreen++;
}
