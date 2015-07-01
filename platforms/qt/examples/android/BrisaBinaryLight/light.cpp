#include "light.h"
#include "ui_light.h"
#include <QDebug>

using namespace Brisa;

Widget::Widget(QWidget *parent) :
    QWidget(parent),binaryLight(DEVICE_TYPE, DEVICE_FRIENDLY_NAME,DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_NUMBER, DEVICE_MODEL_URL, DEVICE_SERIAL_NUMBER, getCompleteUuid()) {


    //   ../../../../
    qDebug() << "Brisa BL: Setting Up UI";
    setupUi(this);
/*    contSplashScreen = 0;
    splash
            = new QSplashScreen(
                    QPixmap(
                            "../BrisaDebExamples_Linux/usr/share/pixmaps/brisa/BrisaSplashScreenBinaryLight.png"));
*/
    icon1.addPixmap(QPixmap(QString::fromUtf8(
            "/data/data/eu.licentia.necessitas.industrius.example.brisa/lib/libonlight1.so")),
            QIcon::Normal, QIcon::Off);
    qDebug() << "Brisa BL: setting icon1 uri";
    icon2.addPixmap(
            QPixmap(
                    QString::fromUtf8(
                            "/data/data/eu.licentia.necessitas.industrius.example.brisa/lib/libofflight1.so")),
            QIcon::Normal, QIcon::Off);
    qDebug() << "Brisa BL: setting icon1 uri";
    // BRisa Device
    SwitchPower *switchPower = new SwitchPower();
    switchPower->setDescriptionFile("/data/data/eu.licentia.necessitas.industrius.example.brisa/lib/libSwitchPower-scpd-xml.so");
    qDebug() << "Brisa BL: SwitchPower Created";
    qDebug() << "Brisa BL: SwitchPower File: " << switchPower->getDescriptionFile();
    binaryLight.addService(switchPower);
    qDebug() << "Brisa BL: SwitchPower service added to BL device";
    binaryLight.start();
    qDebug() << "Brisa BL: BL device started";

    this->status = binaryLight.getServiceByType(
                    "urn:schemas-upnp-org:service:SwitchPower:1")->getVariable(
                    "Status");
    qDebug() << "Brisa BL: service pointer " << binaryLight.getServiceByType(
                        "urn:schemas-upnp-org:service:SwitchPower:1")->getVariable(
                        "Status");
    this->target
            = binaryLight.getServiceByType(
                    "urn:schemas-upnp-org:service:SwitchPower:1")->getVariable(
                    "Target");


        qDebug() << "Brisa BL: Brisa Device Status has returning null after constructor";

        qDebug() << binaryLight.getAttribute(binaryLight.FriendlyName) <<" Brisa Device has returning null after constructor";



  connect(status, SIGNAL(changed(BrisaStateVariable *)), this,    SLOT(statechanged(BrisaStateVariable *)));

  this->statechanged(status);
}

Widget::~Widget() {
}

void Widget::statechanged(BrisaStateVariable *var) {
    if (var->getValue().toBool()) {
        pushButton1->setIcon(icon1);
        label->setText("Switched On");
    } else {
        pushButton1->setIcon(icon2);
        label->setText("Switched Off");
    }
}

void Widget::on_pushButton1_toggled() {
    lampclicked();
}

void Widget::lampclicked() {
    if (pushButton1->isChecked()) {
        pushButton1->setIcon(icon2);
        status->setAttribute(BrisaStateVariable::Value, "0");
        target->setAttribute(BrisaStateVariable::Value, "0");
    } else {
        pushButton1->setIcon(icon1);
        status->setAttribute(BrisaStateVariable::Value, "1");
        target->setAttribute(BrisaStateVariable::Value, "1");
    }
}

void Widget::processSplashScreen() {
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    switch (contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("Setting up Brisa Configuration..."),
                topRight, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("Creating Brisa Configuration..."),
                topRight, Qt::blue);
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
