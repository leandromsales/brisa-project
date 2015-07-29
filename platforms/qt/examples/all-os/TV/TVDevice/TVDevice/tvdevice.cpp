#include "tvdevice.h"
#include "ui_tvdevice.h"
#include "tvoperations.h"


TVDevice::TVDevice(QWidget *parent):
     QMainWindow(parent),
     ui(new Ui::TVDeviceClass),
     tvDevice(DEVICE_TYPE, DEVICE_FRIENDLY_NAME,
                DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,
                DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_NUMBER,
                DEVICE_MODEL_URL, DEVICE_SERIAL_NUMBER, getCompleteUuid())
{
    ui->setupUi(this);

    contSplashScreen= 0;
    currentChanel = 1;
    on = false;


    /**
      * Prepara Device
      */
    TVOperations *tvOperations = new TVOperations();
#ifdef Q_OS_UNIX
    tvOperations->setDescriptionFile("TVOperations.xml");
    splash = new QSplashScreen(QPixmap("../Splash.png"));
    imgChanels.append("../off.png");
    imgChanels.append("../tv-chanel-default.png");
    imgChanels.append("../chanel5.png");
    imgChanels.append("../chanel7.png");
    imgChanels.append("../chanel11.png");
#else
    tvOperations->setDescriptionFile("../TVOperations.xml"); 
    splash = new QSplashScreen(QPixmap("../../Splash.png"));
    imgChanels.append("../../off.png");
    imgChanels.append("../../tv-chanel-default.png");
    imgChanels.append("../../chanel5.png");
    imgChanels.append("../../chanel7.png");
    imgChanels.append("../../chanel11.png");
#endif
    tvDevice.addService(tvOperations);
    tvDevice.start();


    selectedChanel = tvDevice.getServiceByType("urn:schemas-upnp-org:service:TVOperations:1")->getVariable("SelectedChanel");

    operation = tvDevice.getServiceByType("urn:schemas-upnp-org:service:TVOperations:1")->getVariable("Operation");

    qDebug() << "pass after selectedChanel";
    connect(operation,SIGNAL(changed(BrisaStateVariable*)),this,SLOT(updateTV(BrisaStateVariable*)));
    connect(selectedChanel,SIGNAL(changed(BrisaStateVariable*)),this,SLOT(changeChanel(BrisaStateVariable*)));

}

/**
  * Método que atualiza a tv
 **/
void TVDevice::updateTV(BrisaStateVariable *result)
{
    qDebug() << "pass in update TV";
    QString _operation = operation->getValue().toString();

    qDebug() << _operation;
    qDebug() << _operation;

    if(_operation == "OffOnTV")
    {
        qDebug() << "power button clicked";
        offOnTV(result);
    }else
    {
        qDebug() << "changing chanel";
        changeChanel(result);
    }

}

void TVDevice::changeChanel(BrisaStateVariable *result)
{
        qDebug(qPrintable(selectedChanel->getAttribute(BrisaStateVariable::Value)));

        QString commandControl = selectedChanel->getAttribute(BrisaStateVariable::Value);
        on = true;
        if(on){
            changeChanel(commandControl);
        }
}

/**
  * Desligar ou Ligar TV
 **/
void TVDevice::offOnTV(BrisaStateVariable *result)
{
    qDebug() << "pass in offOn TV";
    offOn();
}

/**
  * Método que altera o chanel
 **/
void TVDevice::changeChanel(QString commandControl)
{
    qDebug() << "pass in change chanel";

    if(commandControl.startsWith("next"))
    {
        currentChanel++;
    }else{
        if(currentChanel > 1)
        {
            currentChanel--;
        }
    }

    if(currentChanel < imgChanels.size() && currentChanel > 1)
    {
        icon2.addPixmap(QPixmap(imgChanels[currentChanel]),
            QIcon::Normal, QIcon::Off);

        ui->btn_tv->setIcon(icon2);

    }else{

       icon.addPixmap(QPixmap(imgChanels[1]),
            QIcon::Normal, QIcon::Off);

        ui->btn_tv->setIcon(icon);

    }    

    ui->lcdSelectedChanel->setProperty("value", QVariant(currentChanel));

}

void TVDevice::offOn(){
    qDebug() << "pass in offOn";

    if(on){

        icon3.addPixmap(QPixmap(imgChanels[0]),
            QIcon::Normal, QIcon::Off);

        ui->btn_tv->setIcon(icon3);

        on = false;
    }else{

       icon.addPixmap(QPixmap(imgChanels[1]),
            QIcon::Normal, QIcon::Off);

       ui->btn_tv->setIcon(icon);

       on = true;
       qDebug("turning on");
    }
}


/**
  * Tela Splash
 **/
void TVDevice::processSplashScreen()
{
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    switch (contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("configuring Brisa..."),topRight, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("starting..."),topRight, Qt::blue);
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

TVDevice::~TVDevice()
{
    delete ui;
}
