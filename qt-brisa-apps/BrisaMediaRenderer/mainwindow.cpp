#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace Brisa;
using namespace Phonon;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    source = new MediaObject(this);
    videoWidget = new VideoWidget(this);
    audio = new AudioOutput(VideoCategory, this);

    createPath(source, videoWidget);
    createPath(source, audio);
    audio->setVolume(100);
    avTransport = new AVTransport(source);
    renderControl = new RenderControl(audio);
    connectionManager = new ConnectionManager();

    this->setGeometry(QRect(5,5,645,380));
    videoWidget->setGeometry(QRect(10,20,624,352));

    this->cp = new ControlPoint(this);
    device = new BrisaDevice("urn:schemas-upnp-org:device:MediaRenderer:1",
                             "BRisa Media Renderer",
                             "Brisa Team. Embedded Laboratory and INdT Brazil",
                             "https://garage.maemo.org/projects/brisa",
                             "An Open Source UPnP Media Renderer",
                             "BRisa Media Renderer",
                             "1.0",
                             "https://garage.maemo.org/projects/brisa",
                             "1.0",
                             getCompleteUuid());

    device->addService(avTransport);
    device->addService(renderControl);
    device->addService(connectionManager);

    device->start();
    this->cp->start();
    this->cp->discover();
}

MainWindow::~MainWindow()
{
    delete device;
    delete ui;
}
