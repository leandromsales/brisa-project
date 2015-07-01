
#include <time.h>

#include <QDebug>
#include <BrisaUtils/BrisaLog>

#include "basicsdevicesgui.h"

using namespace Brisa;

BasicDeviceGui::BasicDeviceGui(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    createActions();
    createMenus();
    createToolBars();
    labelRoteador->setPixmap(QPixmap("/usr/share/pixmaps/brisa/roteador.png"));
    splash = new QSplashScreen(QPixmap("/usr/share/pixmaps/brisa/BrisaSplashScreenDevice.png"));

    pd = new QProgressDialog("Adding...", "Abort", 0,100, this);
    pd->setWindowTitle("Add a Brisa Device");
    pd->setMinimumSize(250,100);
    pd->setMaximumSize(250,100);
    connect(pd, SIGNAL(canceled()), this, SLOT(pdCancel()));

    t = new QTimer();
    t->setInterval(55);//1000 mili seconds
    connect(t, SIGNAL(timeout()), this, SLOT(pdProgress()));

    brisaLogInitialize();

    initializeLabelImageList();
    initializeLabelNameList();

    contDevices = 0;
    contSteps = 0;
    contSplashScreen = 0;

}

BasicDeviceGui::~BasicDeviceGui()
{
    stopAllDevices();
}

void BasicDeviceGui::createActions()
{
    //FILE MENU ACTION
    exitAction = new QAction(tr("Quit"), this);
    exitAction->setIcon(QIcon("/usr/share/pixmaps/brisa/exit.png"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    closeAllDevices = new QAction(tr("Close All Devices"), this);
    closeAllDevices->setIcon(QIcon("/usr/share/pixmaps/brisa/clear.png"));
    connect(closeAllDevices, SIGNAL(triggered()), this, SLOT(stopAllDevices()));

    //DEVICE ACTIONS
    deviceAction1 = new QAction(tr("Binary Light"), this);
    deviceAction1->setIcon(QIcon("/usr/share/pixmaps/brisa/lightgreen.png"));
    deviceAction1->setToolTip("Create a Binary Light Device");
    connect(deviceAction1, SIGNAL(triggered()), this, SLOT(setTagD1()));
    connect(deviceAction1, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction2 = new QAction(tr("Eletronic Door"), this);
    deviceAction2->setIcon(QIcon("/usr/share/pixmaps/brisa/door.png"));
    deviceAction2->setToolTip("Create a Eletronic Door Device");
    connect(deviceAction2, SIGNAL(triggered()), this, SLOT(setTagD2()));
    connect(deviceAction2, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction3 = new QAction(tr("Desktop Computer"), this);
    deviceAction3->setIcon(QIcon("/usr/share/pixmaps/brisa/pc2.png"));
    deviceAction3->setToolTip("Create a Desktop Computer Device");
    connect(deviceAction3, SIGNAL(triggered()), this, SLOT(setTagD3()));
    connect(deviceAction3, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction4 = new QAction(tr("SmartPhone"), this);
    deviceAction4->setIcon(QIcon("/usr/share/pixmaps/brisa/smartphone.png"));
    deviceAction4->setToolTip("Create a SmartPhone Device");
    connect(deviceAction4, SIGNAL(triggered()), this, SLOT(setTagD4()));
    connect(deviceAction4, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction5 = new QAction(tr("Notebook"), this);
    deviceAction5->setIcon(QIcon("/usr/share/pixmaps/brisa/notebook.png"));
    deviceAction5->setToolTip("Create a Notebook Device");
    connect(deviceAction5, SIGNAL(triggered()), this, SLOT(setTagD5()));
    connect(deviceAction5, SIGNAL(triggered()), this, SLOT(startProgress()));

    deviceAction6 = new QAction(tr("Game Console"), this);
    deviceAction6->setIcon(QIcon("/usr/share/pixmaps/brisa/console.png"));
    deviceAction6->setToolTip("Create a Console Game Device");
    connect(deviceAction6, SIGNAL(triggered()), this, SLOT(setTagD6()));
    connect(deviceAction6, SIGNAL(triggered()), this, SLOT(startProgress()));

    //ABOUT ACTIONS
    aboutBDGAction = new QAction(tr("About Basic Device Gui"), this);
    aboutBDGAction->setIcon(QIcon("/usr/share/pixmaps/brisa/device.png"));
    aboutBDGAction->setToolTip("Informations of Basic Device Gui");
    connect(aboutBDGAction, SIGNAL(triggered()), this, SLOT(aboutBDG()));

    aboutBrisaAction = new QAction(tr("About Brisa"), this);
    aboutBrisaAction->setIcon(QIcon("/usr/share/pixmaps/brisa/brisa.png"));
    aboutBrisaAction->setToolTip("Informations of Brisa");
    connect(aboutBrisaAction, SIGNAL(triggered()), this, SLOT(aboutBrisa()));

    aboutUpnpAction = new QAction(tr("About UPnP"), this);
    aboutUpnpAction->setIcon(QIcon("/usr/share/pixmaps/brisa/upnp.png"));
    aboutUpnpAction->setToolTip("Informations of UPnP");
    connect(aboutUpnpAction, SIGNAL(triggered()), this, SLOT(aboutUpnp()));

    aboutQtAction = new QAction(tr("About QT"), this);
    aboutQtAction->setIcon(QIcon("/usr/share/pixmaps/brisa/qt.png"));
    aboutQtAction->setToolTip("Informations of QT");
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void BasicDeviceGui::startProgress()
{
    t->start();
}

void BasicDeviceGui::createMenus()
{
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

void BasicDeviceGui::createToolBars()
{
    toolBar = new QToolBar("Add Devices ToolBar",centralwidget);
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

void BasicDeviceGui::pdProgress()
{
    pd->setValue(contSteps);
    contSteps++;
    if (contSteps > pd->maximum())
    {
        t->stop();
        pd->cancel();
        contSteps = 0;
        startDevice();
    }
}

void BasicDeviceGui::pdCancel()
{
    t->stop();
    contSteps = 0;
}

void BasicDeviceGui::startDevice()
{
    switch(this->tagD)
    {
    case 1:
        createDevice1();
        this->tagD = 0;
        break;
    case 2:
        createDevice2();
        this->tagD = 0;
        break;
    case 3:
        createDevice3();
        this->tagD = 0;
        break;
    case 4:
        createDevice4();
        this->tagD = 0;
        break;
    case 5:
        createDevice5();
        this->tagD = 0;
        break;
    case 6:
        createDevice6();
        this->tagD = 0;
        break;
    }
}

void BasicDeviceGui::createDevice1()
{
    if(contDevices == 8)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Limit of Devices");
        msgBox.setText("The number Limit of Devices is 8");
        msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/error.png"));
        msgBox.exec();
        return;
    }

    // Clicking the push button will start the progress bar animation
    contDevices++;
    BrisaDevice *device = new BrisaDevice("urn:schemas-upnp-org:device:BrisaBinaryLight:1",
                                          "Brisa Binary Light Device",
                                          "Brisa Team. Embedded Laboratory and INdT Brazil",
                                          "https://garage.maemo.org/projects/brisa",
                                          "An UPnP Binary Light Device",
                                          "Brisa Binary Light Device",
                                          "1.0",
                                          "https://garage.maemo.org/projects/brisa",
                                          "1.0",
                                          createUdn());

    BrisaService *service= new BrisaService("urn:schemas-upnp-org:service:SwitchPower:1",
                       "SwitchPower",
                       "/SwitchPower/SwitchPower-scpd.xml",
                       "/SwitchPower/control",
                       "/SwitchPower/eventSub");

    service->setDescriptionFile("/etc/brisa/brisadevicegen/SwitchPower-scpd.xml");
    device->addService(service);

    device->start();

    labelNameList[(contDevices -1 )]->setText(device->getAttribute(BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices -1 )]->setPixmap(QPixmap("/usr/share/pixmaps/brisa/lightgreen2.png"));

    this->deviceList.append(device);

}

void BasicDeviceGui::createDevice2()
{
    if(contDevices == 8)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Limit of Devices");
        msgBox.setText("The number Limit of Devices is 8");
        msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/error.png"));
        msgBox.exec();
        return;
    }
    contDevices++;


    BrisaDevice *device = new BrisaDevice("urn:schemas-upnp-org:device:BrisaEletronicDoor:1",
                                          "Brisa Eletronic Door",
                                          "Brisa Team. Embedded Laboratory and INdT Brazil",
                                          "https://garage.maemo.org/projects/brisa",
                                          "An UPnP Eletronic Door",
                                          "Brisa Eletronic Door",
                                          "1.0",
                                          "https://garage.maemo.org/projects/brisa",
                                          "1.0",
                                          createUdn());


    BrisaService *service= new BrisaService("urn:schemas-upnp-org:service:SwitchDoor:1",
                       "SwitchDoor",
                       "/SwitchDoor/SwitchDoor-scpd.xml",
                       "/SwitchDoor/control",
                       "/SwitchDoor/eventSub");

    service->setDescriptionFile("/etc/brisa/brisadevicegen/SwitchDoor-scpd.xml");
    device->addService(service);


    device->start();

    labelNameList[(contDevices -1 )]->setText(device->getAttribute(BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices -1 )]->setPixmap(QPixmap("/usr/share/pixmaps/brisa/door2.png"));

    this->deviceList.append(device);
}

void BasicDeviceGui::createDevice3()
{
    if(contDevices == 8)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Limit of Devices");
        msgBox.setText("The number Limit of Devices is 8");
        msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/error.png"));
        msgBox.exec();
        return;
    }
    contDevices++;


    BrisaDevice *device = new BrisaDevice("urn:schemas-upnp-org:device:DesktopPc:1",
                                          "Brisa Desktop Computer",
                                          "Brisa Team. Embedded Laboratory and INdT Brazil",
                                          "https://garage.maemo.org/projects/brisa",
                                          "An UPnP Dektop Computer",
                                          "Dektop Computer",
                                          "1.0",
                                          "https://garage.maemo.org/projects/brisa",
                                          "1.0",
                                          createUdn());


    BrisaService *service= new BrisaService("urn:schemas-upnp-org:service:PowerPc:1",
                       "SwitchDoor",
                       "/SwitchDoor/PowerPc-scpd.xml",
                       "/SwitchDoor/control",
                       "/SwitchDoor/eventSub");

    service->setDescriptionFile("/etc/brisa/brisadevicegen/PowerPc-scpd.xml");
    device->addService(service);


    device->start();

    labelNameList[(contDevices -1 )]->setText(device->getAttribute(BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices -1 )]->setPixmap(QPixmap("/usr/share/pixmaps/brisa/pc2.png"));

    this->deviceList.append(device);
}

void BasicDeviceGui::createDevice4()
{
    if(contDevices == 8)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Limit of Devices");
        msgBox.setText("The number Limit of Devices is 8");
        msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/error.png"));
        msgBox.exec();
        return;
    }
    contDevices++;


    BrisaDevice *device = new BrisaDevice("urn:schemas-upnp-org:device:SmartPhone:1",
                                          "Brisa SmartPhone",
                                          "Brisa Team. Embedded Laboratory and INdT Brazil",
                                          "https://garage.maemo.org/projects/brisa",
                                          "An UPnP SmartPhone",
                                          "SmartPhone",
                                          "1.0",
                                          "https://garage.maemo.org/projects/brisa",
                                          "1.0",
                                          createUdn());


    BrisaService *service= new BrisaService("urn:schemas-upnp-org:service:PowerCell:1",
                       "SmartPhone",
                       "/SmartPhone/SmartPhone-scpd.xml",
                       "/SmartPhone/control",
                       "/SmartPhone/eventSub");

    service->setDescriptionFile("/etc/brisa/brisadevicegen/SmartPhone-scpd.xml");
    device->addService(service);


    device->start();

    labelNameList[(contDevices -1 )]->setText(device->getAttribute(BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices -1 )]->setPixmap(QPixmap("/usr/share/pixmaps/brisa/smartphone.png"));

    this->deviceList.append(device);
}

void BasicDeviceGui::createDevice5()
{
    if(contDevices == 8)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Limit of Devices");
        msgBox.setText("The number Limit of Devices is 8");
        msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/error.png"));
        msgBox.exec();
        return;
    }
    contDevices++;


    BrisaDevice *device = new BrisaDevice("urn:schemas-upnp-org:device:NoteBook:1",
                                          "Brisa NoteBook",
                                          "Brisa Team. Embedded Laboratory and INdT Brazil",
                                          "https://garage.maemo.org/projects/brisa",
                                          "An UPnP NoteBook",
                                          "NoteBook",
                                          "1.0",
                                          "https://garage.maemo.org/projects/brisa",
                                          "1.0",
                                          createUdn());


    BrisaService *service= new BrisaService("urn:schemas-upnp-org:service:PowerNotebook:1",
                       "Notebook",
                       "/Notebook/Notebook-scpd.xml",
                       "/Notebook/control",
                       "/Notebook/eventSub");

    service->setDescriptionFile("/etc/brisa/brisadevicegen/Notebook-scpd.xml");
    device->addService(service);


    device->start();

    labelNameList[(contDevices -1 )]->setText(device->getAttribute(BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices -1 )]->setPixmap(QPixmap("/usr/share/pixmaps/brisa/notebook.png"));

    this->deviceList.append(device);
}

void BasicDeviceGui::createDevice6()
{
    if(contDevices == 8)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Limit of Devices");
        msgBox.setText("The number Limit of Devices is 8");
        msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/error.png"));
        msgBox.exec();
        return;
    }
    contDevices++;


    BrisaDevice *device = new BrisaDevice("urn:schemas-upnp-org:device:GameConsole:1",
                                          "Brisa Game Console",
                                          "Brisa Team. Embedded Laboratory and INdT Brazil",
                                          "https://garage.maemo.org/projects/brisa",
                                          "An UPnP Game Console",
                                          "GameConsole",
                                          "1.0",
                                          "https://garage.maemo.org/projects/brisa",
                                          "1.0",
                                          createUdn());

    BrisaService *service= new BrisaService("urn:schemas-upnp-org:service:GameConsole:1",
                       "GameConsole",
                       "/GameConsole/GameConsole-scpd.xml",
                       "/GameConsole/control",
                       "/GameConsole/eventSub");

    service->setDescriptionFile("/etc/brisa/brisadevicegen/GameConsole-scpd.xml");
    device->addService(service);

    device->start();

    labelNameList[(contDevices -1 )]->setText(device->getAttribute(BrisaDevice::FriendlyName).remove(QChar(' ')));
    labelImageList[(contDevices -1 )]->setPixmap(QPixmap("/usr/share/pixmaps/brisa/console.png"));

    this->deviceList.append(device);
}

void BasicDeviceGui::stopAllDevices()
{
    for(int i=0; i < this->deviceList.size();i++)
    {
        this->deviceList[i]->stop();
    }
    for(int j= 0; j < labelImageList.size() ; j++)
    {
        labelImageList[j]->clear();
    }
    for(int k= 0; k < labelNameList.size() ; k++)
    {
        labelNameList[k]->clear();
    }
    contDevices = 0;
}

void BasicDeviceGui::initializeLabelImageList()
{
    this->labelImageList.append(labelImage1);
    this->labelImageList.append(labelImage2);
    this->labelImageList.append(labelImage3);
    this->labelImageList.append(labelImage4);
    this->labelImageList.append(labelImage5);
    this->labelImageList.append(labelImage6);
    this->labelImageList.append(labelImage7);
    this->labelImageList.append(labelImage8);
}

void BasicDeviceGui::initializeLabelNameList()
{
    this->labelNameList.append(labelName1);
    this->labelNameList.append(labelName2);
    this->labelNameList.append(labelName3);
    this->labelNameList.append(labelName4);
    this->labelNameList.append(labelName5);
    this->labelNameList.append(labelName6);
    this->labelNameList.append(labelName7);
    this->labelNameList.append(labelName8);
}

QString BasicDeviceGui::createUdn()
{
    QString randNumber1;
    QString randNumber2;
    QString randNumber3;
    QString randNumber4;
    QString randNumber5;
    QString randNumber6;
    QString randNumber7;
    QString udn;

    randNumber1.setNum(get3RandomNumber());
    randNumber2.setNum(get1RandomNumber());
    randNumber3.setNum(get1RandomNumber());
    randNumber4.setNum(get2RandomNumber());
    randNumber5.setNum(get1RandomNumber());
    randNumber6.setNum(get1RandomNumber());
    randNumber7.setNum(get3RandomNumber());

    //udn = "uuid:" + QString(randNumber1) + QString(randNumber2) + QString(randNumber3) +"a4fa7-cf68-4cc8-844d-0af4c615cecb";
    udn.append("uuid:");
    udn.append(randNumber1);
    udn.append("a");
    udn.append(randNumber2);
    udn.append("fa");
    udn.append(randNumber3);
    udn.append("-cf");
    udn.append(randNumber4);
    udn.append("-");
    udn.append(randNumber5);
    udn.append("cc");
    udn.append(randNumber6);
    udn.append("-");
    udn.append(randNumber7);
    udn.append("d-");
    udn.append("0af4c615cecb");

    return udn;
}

int BasicDeviceGui::get1RandomNumber()
{
    srand(time(NULL));
    return rand() % 10;
}

int BasicDeviceGui::get2RandomNumber()
{
    srand(time(NULL));
    return rand() % 90 + 10;
}

int BasicDeviceGui::get3RandomNumber()
{
    srand(time(NULL));
    return rand() % 900 + 100;
}

void BasicDeviceGui::setTagD1()
{
    this->tagD = 1;
}

void BasicDeviceGui::setTagD2()
{
    this->tagD = 2;
}

void BasicDeviceGui::setTagD3()
{
    this->tagD = 3;
}

void BasicDeviceGui::setTagD4()
{
    this->tagD = 4;
}

void BasicDeviceGui::setTagD5()
{
    this->tagD = 5;
}

void BasicDeviceGui::setTagD6()
{
    this->tagD = 6;
}

void BasicDeviceGui::aboutBDG()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About BasicDeviceGUI");
    msgBox.setText("<h3>About BasicDeviceGUI</h3>"
                   "<p>"
                   "<b>BasicDeviceGui</b> is a Brisa Example with Gui "
                   "to create UPnP devices");
    msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/device.png"));
    msgBox.exec();
}

void BasicDeviceGui::aboutBrisa()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Brisa");
    msgBox.setText("<center><h1>About Brisa</h1></center>"
                   "<p>"
                   "<center><b>BRisa</b> is a project focused on developing UPnP "
                   "technologies.</center>"
                   "<p>"
                   "<p>"
                   "<center>http://brisa.garage.maemo.org");
    msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/brisa_logo.png"));
    msgBox.exec();
}

void BasicDeviceGui::aboutUpnp()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About UPnP");
    msgBox.setText("<center><h1>About UPnP</h1></center>"
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
    msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/upnp.png"));
    msgBox.exec();
}

void BasicDeviceGui::processSplashScreen()
{
    Qt::Alignment topLeft = Qt::AlignLeft | Qt::AlignTop;

    switch(contSplashScreen)
    {
        case 1:
            splash->showMessage(QObject::tr("Setting up Brisa Device Generator..."),topLeft, Qt::blue);
            break;
        case 2:
            splash->showMessage(QObject::tr("Waiting for Brisa Device Generator..."),topLeft, Qt::blue);
            break;
    }

    if(contSplashScreen > 2)
    {
        this->show();
        splash->finish(this);
        emit timeStop();
        delete splash;
    }

    contSplashScreen++;
}
