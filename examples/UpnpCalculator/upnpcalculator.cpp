#include "upnpcalculator.h"
#include "ui_calculator.h"


#define CURRENT_DIR QString("/home/bruno/Documentos/QtProjects/UpnpCalculator").append ("/")

using namespace brisa;
using namespace brisa::upnp;

UpnpCalculator::UpnpCalculator(QWidget *parent) :
    QWidget(parent), upnpCalculator(DEVICE_TYPE, DEVICE_FRIENDLY_NAME,
                                     DEVICE_MANUFACTURER, DEVICE_MANUFACTURER_URL,
                                     DEVICE_MODEL_DESCRIPTION, DEVICE_MODEL_NAME, DEVICE_MODEL_NUMBER,
                                     DEVICE_MODEL_URL, DEVICE_SERIAL_NUMBER, getCompleteUuid()) {

    setupUi(this);

    contSplashScreen = 0;

    // BRisa Device
    Functions *functions = new Functions();

#ifdef Q_OS_ANDROID
    qDebug() << "ANDROID :D";
    splash = new QSplashScreen(QPixmap("://assets/BrisaSplashScreenUpnpCalculator.png"));

    functions->setDescriptionFile("://assets/Functions.xml");

#elif defined(Q_OS_UNIX)
    qDebug() << "UNIX";
    splash = new QSplashScreen(QPixmap(CURRENT_DIR + "assets/BrisaSplashScreenUpnpCalculator.png"));

    functions->setDescriptionFile(CURRENT_DIR + "assets/Functions.xml");

#else
    qDebug() << "OUTRO S.O.";
    splash
            = new QSplashScreen(
                QPixmap(
                    "../../../pixmaps/brisa/BrisaSplashScreenUpnpCalculator.png"));

    functions->setDescriptionFile("../Functions.xml");

#endif

    upnpCalculator.addService(functions);


    upnpCalculator.start();

    this->status
            = upnpCalculator.getServiceByType(
                "urn:schemas-upnp-org:service:Functions:1")->getVariable(
                "Resultado");

    bool x = connect(status, SIGNAL(changed(brisa::upnp::StateVariable *)), this,
                     SLOT(statechanged(brisa::upnp::StateVariable *)));

    Q_ASSERT(x);

    qDebug() << "Connected: " << x;

    this->statechanged(status);
}

UpnpCalculator::~UpnpCalculator()
{
    delete ui;
}


void UpnpCalculator::statechanged(StateVariable *var) {

    result->setText(var->getValue().toString());

}

void UpnpCalculator::labelChanged()
{
    status->setAttribute(StateVariable::Value, result->text().toInt());
}

void UpnpCalculator::processSplashScreen() {
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
