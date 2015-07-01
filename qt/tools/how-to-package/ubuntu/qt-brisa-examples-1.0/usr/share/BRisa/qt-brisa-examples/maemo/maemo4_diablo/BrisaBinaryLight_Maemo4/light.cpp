#include "light.h"
#include "ui_light.h"
#include <QDebug>

using namespace Brisa;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    binaryLight(DEVICE_TYPE,
                DEVICE_FRIENDLY_NAME,
                DEVICE_MANUFACTURER,
                DEVICE_MANUFACTURER_URL,
                DEVICE_MODEL_DESCRIPTION,
                DEVICE_MODEL_NAME,
                DEVICE_MODEL_NUMBER,
                DEVICE_MODEL_URL,
                DEVICE_SERIAL_NUMBER,
                createUdn())
{
    //   ../../../../
    setupUi(this);
    contSplashScreen = 0;
    splash = new QSplashScreen(QPixmap("/usr/share/pixmaps/brisa/BrisaSplashScreenBinaryLight.png"));

    icon1.addPixmap(QPixmap(QString::fromUtf8("/usr/share/pixmaps/brisa/onlight1.png")),
    QIcon::Normal,QIcon::Off);

    icon2.addPixmap(QPixmap(QString::fromUtf8("/usr/share/pixmaps/brisa/offlight1.png")),
    QIcon::Normal,QIcon::Off);

    pushButton1->setIcon(icon1);
    // BRisa Device
    SwitchPower *switchPower = new SwitchPower();
    switchPower->setDescriptionFile("/etc/brisa/brisabinarylight/SwitchPower-scpd.xml");
    binaryLight.addService(switchPower);
    binaryLight.start();

    this->status = binaryLight.getServiceByType("urn:schemas-upnp-org:service:SwitchPower:1")->getVariable("Status");
    this->target = binaryLight.getServiceByType("urn:schemas-upnp-org:service:SwitchPower:1")->getVariable("Target");

    connect(status, SIGNAL(changed(BrisaStateVariable *)), this, SLOT(statechanged(BrisaStateVariable *)));
}

Widget::~Widget()
{
}

void Widget::statechanged(BrisaStateVariable *var)
{
    if(var->getAttribute(BrisaStateVariable::Value) == "1") {
        pushButton1->setIcon(icon1);
    } else {
        pushButton1->setIcon(icon2);
    }
}

void Widget::on_pushButton1_toggled()
{
    lampclicked();
}

void Widget::lampclicked()
{
    if(pushButton1->isChecked())
    {
            pushButton1->setIcon(icon2);
            status->setAttribute(BrisaStateVariable::Value, "0");
            target->setAttribute(BrisaStateVariable::Value, "0");
    }
    else
    {
            pushButton1->setIcon(icon1);
            status->setAttribute(BrisaStateVariable::Value, "1");
            target->setAttribute(BrisaStateVariable::Value, "1");
    }
}

void Widget::processSplashScreen()
{
    Qt::Alignment topLeft = Qt::AlignLeft | Qt::AlignTop;

    switch (contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("Setting up Brisa Binary Light..."),topLeft, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("Creating Brisa Binary Light..."),topLeft, Qt::blue);
        break;
    }

    if (contSplashScreen > 2) {
        this->show();
        splash->finish(this);
        emit timeStop();
        delete splash;
    }
    contSplashScreen++;
}

QString Widget::createUdn()
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

int Widget::get1RandomNumber()
{
    srand(time(NULL));
    return rand() % 10;
}

int Widget::get2RandomNumber()
{
    srand(time(NULL));
    return rand() % 90 + 10;
}

int Widget::get3RandomNumber()
{
    srand(time(NULL));
    return rand() % 900 + 100;
}

