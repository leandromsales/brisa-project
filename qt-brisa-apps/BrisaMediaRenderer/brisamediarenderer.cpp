#include "brisamediarenderer.h"

using namespace Brisa;

BrisaMediaRenderer::BrisaMediaRenderer(QWidget *parent) :
    QWidget(parent)
{
    this->configureWidget();
    this->cp = new ControlPoint(this);
    this->device = new BrisaDevice("urn:schemas-upnp-org:device:MediaRenderer:1",
                                   "BRisa Media Renderer",
                                   "Brisa Team. Embedded Laboratory and INdT Brazil",
                                   "https://garage.maemo.org/projects/brisa",
                                   "An Open Source UPnP Media Renderer",
                                   "BRisa Media Renderer",
                                   "1.0",
                                   "https://garage.maemo.org/projects/brisa",
                                   "1.0",
                                   getCompleteUuid());
    connect(this->cp, SIGNAL(mediaServersChanged(QMap<QString,BrisaControlPointDevice*>)),
            this, SLOT(mediaServersChanged(QMap<QString,BrisaControlPointDevice*>)));
    connect(mediaServers, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(browse(QModelIndex)));
    this->cp->start();
    this->cp->discover();
}

BrisaMediaRenderer::~BrisaMediaRenderer()
{
    delete this->device;
    delete this->cp;
    delete this->model;
    delete this->mediaServers;
    delete this->play;
    delete this->pause;
    delete this->stop;
    delete this->mediaControl;
    delete this->mrLayout;
    delete this->msLayout;
    delete this->mainLayout;
}

void BrisaMediaRenderer::configureWidget()
{
    this->msLayout = new QHBoxLayout();
    this->model = new QStringListModel();
    this->mediaServers = new QListView(this);
    this->mediaServers->setModel(model);
    this->msLayout->addWidget(mediaServers);
    this->mrLayout = new QVBoxLayout();
    this->mediaControl = new QHBoxLayout();
    this->play = new QPushButton("Play", this);
    this->stop = new QPushButton("Stop", this);
    this->pause = new QPushButton("Pause", this);
    this->mediaControl->addWidget(play);
    this->mediaControl->addWidget(pause);
    this->mediaControl->addWidget(stop);
    this->mrLayout->addLayout(this->mediaControl);
    this->mainLayout = new QHBoxLayout(this);
    this->mainLayout->addLayout(this->msLayout);
    this->mainLayout->addLayout(this->mrLayout);
}

void BrisaMediaRenderer::mediaServersChanged(QMap<QString, BrisaControlPointDevice *> devices)
{
//    QStringList deviceList;
//    foreach(BrisaControlPointDevice *ms, devices.values()) {
//        deviceList.append(ms->getAttribute(BrisaControlPointDevice::FriendlyName));
//    }
    this->model->setStringList(devices.keys());
}

void BrisaMediaRenderer::browse(QModelIndex index)
{
}
