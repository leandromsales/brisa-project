#include <QString>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

#include "controlpointgui.h"

using namespace Brisa;

controlpointgui::controlpointgui(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    createActions();
    createMenus();
    createToolBars();
    splash = new QSplashScreen(QPixmap("/usr/share/pixmaps/brisa/BrisaSplashScreenControlPoint.png"));

    controlPoint = new BrisaControlPoint();

    connect(controlPoint, SIGNAL(deviceGone(QString)), this, SLOT(removeDevice(QString)));
    connect(controlPoint, SIGNAL(deviceFound(BrisaControlPointDevice*))
            , this, SLOT(deviceFoundDump(BrisaControlPointDevice*)));
    controlPoint->start();
    controlPoint->discover();

    connect(treeWidgetCP, SIGNAL(itemClicked(QTreeWidgetItem * , int )),
            this, SLOT(lineEnabled(QTreeWidgetItem * , int)));
    connect(treeWidgetCP, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(call(QTreeWidgetItem *, int)));

    networkItem = new QTreeWidgetItem(treeWidgetCP);
    networkItem->setIcon(0,QIcon("/usr/share/pixmaps/brisa/network.png"));
    networkItem->setText(0,"UPnP Network");

    contSplashScreen = 0;
    doubleClick = false;
}

controlpointgui::~controlpointgui()
{
    controlPoint->stop();
    delete controlPoint;
}

void controlpointgui::createActions()
{
    expandAction = new QAction(tr("&Expand all Devices"), this);
    expandAction->setIcon(QIcon("/usr/share/pixmaps/brisa/plus.png"));
    expandAction->setToolTip("Expand all Devices");
    connect(expandAction, SIGNAL(triggered()), this, SLOT(expandItems()));

    collapseAction = new QAction(tr("&Collapse all Devices"), this);
    collapseAction->setIcon(QIcon("/usr/share/pixmaps/brisa/minus.png"));
    collapseAction->setToolTip("Collapse all Devices");
    connect(collapseAction, SIGNAL(triggered()), this, SLOT(collapseItems()));

    aboutCpAction = new QAction(tr("About Control Point"), this);
    aboutCpAction->setIcon(QIcon("/usr/share/pixmaps/brisa/info.png"));
    connect(aboutCpAction, SIGNAL(triggered()), this, SLOT(aboutControlPoint()));

    aboutBrisaAction = new QAction(tr("About Brisa"), this);
    aboutBrisaAction->setIcon(QIcon("/usr/share/pixmaps/brisa/brisa.png"));
    connect(aboutBrisaAction, SIGNAL(triggered()), this, SLOT(aboutBrisa()));

    aboutUpnpAction = new QAction(tr("About UPnP"), this);
    aboutUpnpAction->setIcon(QIcon("/usr/share/pixmaps/brisa/upnp.png"));
    connect(aboutUpnpAction, SIGNAL(triggered()), this, SLOT(aboutUpnp()));

    aboutQtAction = new QAction(tr("About QT"), this);
    aboutQtAction->setIcon(QIcon("/usr/share/pixmaps/brisa/qt.png"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    exitAction = new QAction(tr("&Quit"), this);
    exitAction->setIcon(QIcon("/usr/share/pixmaps/brisa/exit.png"));
    exitAction->setToolTip("Close the Application");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void controlpointgui::createMenus()
{
    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(exitAction);

    viewMenu = menubar->addMenu(tr("&View"));
    viewMenu->addAction(expandAction);
    viewMenu->addAction(collapseAction);
    viewMenu->addSeparator();

    helpMenu = menubar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutCpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutBrisaAction);
    helpMenu->addAction(aboutUpnpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutQtAction);
}

void controlpointgui::createToolBars()
{
    toolBar = new QToolBar("Device ToolBar",centralwidget);
    toolBar->addSeparator();
    toolBar->addAction(expandAction);
    toolBar->addAction(collapseAction);
    toolBar->addSeparator();
    toolBar->addAction(exitAction);
    toolBar->addSeparator();
}

void controlpointgui::expandItems()
{
    treeWidgetCP->expandItem(this->networkItem);
    for(int i=0;i < items.size(); i++ ) {
        treeWidgetCP->expandItem(items[i]);
        QTreeWidgetItem *deviceItem = items[i];
        for(int j = 0; j < deviceItem->childCount(); j++)
            treeWidgetCP->expandItem(deviceItem->child(j));
    }
}

void controlpointgui::collapseItems()
{
    treeWidgetCP->collapseItem(this->networkItem);
    for(int i=0;i < items.size(); i++) {
        treeWidgetCP->collapseItem(items[i]);
        QTreeWidgetItem *deviceItem = items[i];
        for(int j = 0; j < deviceItem->childCount(); j++)
            treeWidgetCP->collapseItem(deviceItem->child(j));
    }
}

void controlpointgui::aboutControlPoint()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Control Point");
    msgBox.setText("<h2>About Control Point</h2>"
                   "<p>"
                   "<b>Basic Control Point Gui Example</b>");
    msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/info.png"));
    msgBox.exec();
}

void controlpointgui::aboutBrisa()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Brisa");
    msgBox.setText("<center><h1>About Brisa</h1></center> "
                   "<p>"
                   "<center><b>BRisa</b> is a project focused on developing UPnP "
                   "technologies.</center>"
                   "<p>"
                   "<p>"
                   "<center>http://brisa.garage.maemo.org");
    msgBox.setIconPixmap(QPixmap("/usr/share/pixmaps/brisa/brisa_logo.png"));
    msgBox.exec();
}

void controlpointgui::aboutUpnp()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About UPnP");
    msgBox.setText("<center><h1>About UPnP</h1></center>"
                   "<p>"
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

void controlpointgui::addItem(BrisaControlPointDevice *dev)
{
    QTreeWidgetItem *deviceItem = new QTreeWidgetItem(networkItem);
    deviceItem->setIcon(0,QIcon("/usr/share/pixmaps/brisa/device.png"));
    deviceItem->setText(0, dev->getAttribute(BrisaControlPointDevice::FriendlyName) + " - "
                        +dev->getAttribute(BrisaControlPointDevice::Udn));

    items.append(deviceItem);
    QList<BrisaControlPointService*> listService;
    listService = dev->getServiceList();

    for (int i = 0; i < listService.size(); i++) {
        BrisaEventProxy *subscription = controlPoint->getSubscriptionProxy(listService[i]);

        connect(subscription,SIGNAL(eventNotification(BrisaEventProxy *,QMap<QString, QString>)),
                this, SLOT(changeEventLog(BrisaEventProxy *,QMap<QString, QString>)));
        subscription->subscribe(20);

        QList<QString> deviceAndService;

        deviceAndService.append(dev->getAttribute(BrisaControlPointDevice::FriendlyName));
        deviceAndService.append(listService[i]->getAttribute(BrisaControlPointService::ServiceType));

        eventToDevice[subscription->getId()] = deviceAndService;

        QTreeWidgetItem *serviceitem = new QTreeWidgetItem(deviceItem);
        serviceitem->setIcon(0,QIcon("/usr/share/pixmaps/brisa/service.png"));
        serviceitem->setText(0, listService[i]->getAttribute(BrisaControlPointService::ServiceType));

        QList<BrisaAction *> listAction;
        listAction = listService[i]->getActionList();

        for (int j = 0; j < listAction.size(); j++) {
            QTreeWidgetItem *actionItem = new QTreeWidgetItem(serviceitem);
            actionItem->setIcon(0, QIcon("/usr/share/pixmaps/brisa/call.png"));
            actionItem->setText(0, listAction[j]->getName());
        }
    }
}

void controlpointgui::callMethod()
{
    QString action = selected->text(0);
    QString service = selected->parent()->text(0);
    QString udn = selected->parent()->parent()->text(0).split(" - ")[1];
    BrisaControlPointDevice* dev = getDeviceByUDN(udn);
    BrisaControlPointService* serv = dev->getServiceByType(service);
    connect(serv, SIGNAL(requestFinished(QString, QString)), this, SLOT(serviceCall(QString, QString)));

    QMap<QString, QString> parameters;
    for(int i = 0; i < labels.size(); i++) {
        QString paramName = labels[i]->text().replace("       -", "");
        QString value = editors[i]->text();
        parameters[paramName] = value;
    }
    serv->call(action, parameters);
}

void controlpointgui::serviceCall(QString message, QString method)
{
    if(message.replace(" ", "").compare("")) {
        textEdit->setText("Calling method: "+method+"\n\n"+"Returned: \n"+message);
    } else {
        textEdit->setText("Calling method: "+method+"\n"+"Returned: None");
    }
}

void controlpointgui::removeDevice(QString usn)
{
    for(int i = 0; i < items.size(); i++) {
        bool change;
        if(items[i]->text(0).split(" - ")[1].compare(usn.split("::")[0]) == 0) {
            change = false;
            if((treeWidgetCP->selectedItems().size() > 0) &&
                    (!treeWidgetCP->selectedItems().contains(items[i]))) {
                for(int j = 0; j < items[i]->childCount(); j++) {
                    if(change || treeWidgetCP->selectedItems().contains(items[i]->child(j))) {
                        change = true;
                        break;
                    }
                    for(int w = 0; w < items[i]->child(j)->childCount(); w++) {
                        if(treeWidgetCP->selectedItems().contains(items[i]->child(j)->child(w))) {
                            if(closeable)
                                dialog->close();
                            change = true;
                            break;
                        }
                    }
                }
            }
            if(change || treeWidgetCP->selectedItems().contains(items[i]))
                doubleClick = false;

            QString UDN = items[i]->text(0).split(" - ")[1];
            devices.removeOne(getDeviceByUDN(UDN));
            delete items[i];
            items.removeAt(i);
        }
    }
}

void controlpointgui::deviceFoundDump(BrisaControlPointDevice *device) {
    if(device->getAttribute(BrisaControlPointDevice::Udn).compare("") == 0)
        return;
    for(int i = 0; i < devices.size(); i++) {
        if(devices[i]->getAttribute(BrisaControlPointDevice::Udn) == device->getAttribute(BrisaControlPointDevice::Udn)) {
            devices[i] = device;
            return;
        }
    }
    devices.append(device);
    addItem(device);
}

void controlpointgui::lineEnabled(QTreeWidgetItem *item, int collumn)
{
    if((item->text(collumn).compare("UPnP Network") == 0) ||
            (item->parent()->text(collumn).compare("UPnP Network") == 0)) {
        selected = NULL;
        doubleClick = false;
        return;
    }

    selected = item;
    doubleClick = true;
}

void controlpointgui::call(QTreeWidgetItem * item, int collumn)
{
    Q_UNUSED(item);
    Q_UNUSED(collumn);
    if(doubleClick) {
        if(selected->parent() != NULL && selected->parent()->parent() != NULL &&
            selected->parent()->parent()->parent()) {
            mountControlDialog();
            //dialog->exec();
            dialog->open();
        }
    }
}

void controlpointgui::changeEventLog(BrisaEventProxy *subscription,QMap<QString, QString> eventVariables)
{
    Q_UNUSED(subscription);

    QList<QString> deviceService = eventToDevice[subscription->getId()];

    QString newText = "Device: " + deviceService[0];
    newText += "       Service: " + deviceService[1];

    for(int i = 0; i < eventVariables.keys().size(); i++) {
        newText += "       Variable: " + eventVariables.keys()[i];
        newText += "       Value: " + eventVariables[eventVariables.keys()[i]];
    }
    newText += "\n" + eventText->toPlainText();
    eventText->setText(newText);
}

BrisaControlPointDevice* controlpointgui::getDeviceByUDN(QString UDN)
{
    for(int i = 0; i < devices.size(); i++) {
        if(UDN.compare(devices[i]->getAttribute(BrisaControlPointDevice::Udn)) == 0)
            return devices[i];
    }
    return NULL;
}

void controlpointgui::processSplashScreen()
{
    Qt::Alignment topLeft = Qt::AlignLeft | Qt::AlignTop;

    switch(contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("Setting up Brisa ControlPoint..."),topLeft, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("Waiting Brisa ControlPoint..."),topLeft, Qt::blue);
        break;
    }

    if(contSplashScreen > 2) {
        this->show();
        splash->finish(this);
        emit timeStop();
        delete splash;
    }
    contSplashScreen++;
}

void controlpointgui::mountControlDialog()
{
    closeable = true;
    labels.clear();
    editors.clear();

    dialog = new QDialog();
    dialog->setWindowTitle("Action Invocation");
    dialog->setWindowIcon(QIcon("/usr/share/pixmaps/brisa/call.png"));

    QLabel *brisaText = new QLabel();
    brisaText->setText("BRisa UPnP Control Point");

    QLabel *serviceText = new QLabel();
    serviceText->setPixmap(QPixmap("/usr/share/pixmaps/brisa/service.png"));
    serviceText->setText(selected->parent()->text(0));

    QLabel *actionText = new QLabel();
    actionText->setText(selected->text(0));

    QHBoxLayout *brisa = new QHBoxLayout();
    brisa->addWidget(brisaText);

    QHBoxLayout *service = new QHBoxLayout();
    service->addWidget(serviceText);

    QHBoxLayout *action = new QHBoxLayout();
    action->addWidget(actionText);

    QVBoxLayout *actionAndService = new QVBoxLayout();
    actionAndService->addLayout(brisa);
    actionAndService->addLayout(service);
    actionAndService->addLayout(action);

    QPushButton *invoke = new QPushButton();
    invoke->setFixedHeight(75);
    invoke->setIcon(QIcon("/usr/share/pixmaps/brisa/call.png"));
    invoke->setText("Invoke");
    connect(invoke, SIGNAL(clicked()), this, SLOT(callMethod()));

    QHBoxLayout *button = new QHBoxLayout();
    button->addLayout(actionAndService);
    button->addWidget(invoke);

    QVBoxLayout *vertical = new QVBoxLayout();
    vertical->addLayout(button);

    QString udn = selected->parent()->parent()->text(0).split(" - ")[1];
    QString type = selected->parent()->text(0);
    QList<BrisaAction *> actions = getDeviceByUDN(udn)->getServiceByType(type)->getActionList();

    BrisaAction *realAction = 0;
    for(int i = 0; i < actions.size(); i++) {
        BrisaAction *now = actions[i];
        if(now->getName() == selected->text(0)) {
            realAction = now;
            break;
        }
    }
    QList<BrisaArgument *> inArguments;
    for(int i = 0; i < realAction->getArgumentList().size(); i++) {
        if((realAction->getArgumentList()[i])->getAttribute(BrisaArgument::Direction).compare("in") == 0) {
            inArguments.append(realAction->getArgumentList()[i]);
        }
    }
    if(inArguments.size() > 0) {
        QLabel *a = new QLabel();
        a->setText("  ");
        QLabel *in = new QLabel();
        in->setText("'in' Arguments:");
        vertical->addWidget(a);
        vertical->addWidget(in);
        for(int i = 0; i < inArguments.size(); i++) {
            QLabel *parameter = new QLabel();
            parameter->setText("       -"+inArguments[i]->getAttribute(BrisaArgument::ArgumentName));
            labels.append(parameter);

            QLineEdit *lineEdito = new QLineEdit();
            editors.append(lineEdito);

            QHBoxLayout *temp = new QHBoxLayout();
            temp->addWidget(parameter);
            temp->addWidget(lineEdito);
            vertical->addLayout(temp);
        }
    }

    dialog->setLayout(vertical);
}
