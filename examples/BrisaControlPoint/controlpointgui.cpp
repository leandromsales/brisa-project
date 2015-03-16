#include "controlpointgui.h"

#include <QString>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

#include "upnp/controlpoint/controlpoint.h"
#include "upnp/controlpoint/eventproxy.h"
#include "upnp/controlpoint/devicesales.h"
#include "upnp/controlpoint/servicesales.h"

using namespace brisa::upnp;
using namespace brisa::upnp::controlpoint;

controlpointgui::controlpointgui(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    createActions();
    createMenus();
    createToolBars();
    setUpTableWidget();

#ifdef Q_OS_ANDROID

    #define OS 1

#else

    #define OS 0

#endif

    //Pegando o tamanho da Tela
    screenSize = getScreenSize();

    //Definindo a imagem da Splash Screen do programa
    QPixmap pixmap("://rsc/BrisaSplashScreenControlPoint.png");
    //Instanciando a Splash Screen
    splash = new QSplashScreen();
    //Definindo a imagem da Splash Screen
    if(OS)
    {
        splash->setPixmap(pixmap.scaled(screenSize.width(), screenSize.height()*(0.7)));
    }
    else
    {
        splash->setPixmap(pixmap.scaled(this->width(), this->height()*(0.7)));
    }

    //Criando um Control Point
    controlPoint = new ControlPoint();

    connect(controlPoint, SIGNAL(deviceGone(QString)), this, SLOT(removeDevice(
                                                                      QString)));
    connect(controlPoint, SIGNAL(deviceFound(brisa::upnp::controlpoint::Device*))
            , this, SLOT(deviceFoundDump(brisa::upnp::controlpoint::Device*)));

    //Iniciando o Control Point e pesquisando Devices
    controlPoint->start();
    controlPoint->discover();

    connect(devicesTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem * , int )),
            this, SLOT(lineEnabled(QTreeWidgetItem * , int)));
    connect(devicesTreeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(call(QTreeWidgetItem *, int)));

    connect(controlPoint, SIGNAL(multicastReceived(QString,QString)), this,
            SLOT(multicastEventReceived(QString,QString)));
    connect(controlPoint, SIGNAL(multicastReceivedRaw(OutArgument)), this,
            SLOT(multicastEventRawReceived(OutArgument)));

    //Definindo o primeiro item da Lista de Devices
    networkItem = new QTreeWidgetItem(devicesTreeWidget);
    networkItem->setIcon(0, QIcon("://rsc/network.png"));
    networkItem->setText(0, "UPnP Network");

    //Setando Variáveis
    contSplashScreen = 0;
    doubleClick = false;
}

controlpointgui::~controlpointgui()
{
    delete devicesTreeWidget;
    delete controlPoint;
}

void controlpointgui::setUpTableWidget()
{
    QStringList list;
    list <<"Time"<<"Device"<<"Service" << "State Variables" << "Value";

    logBoxTable->setRowCount(0);
    logBoxTable->setColumnCount(5);
    logBoxTable->setHorizontalHeaderLabels(list);
    logBoxTable->setEditTriggers(NULL);

}

void controlpointgui::createActions()
{
    //OS define o sistema operacional executado no momento
    if(OS)
    {
        //Criando ações
        clearAction = new QAction(tr("Clear"),this);
        expandAction = new QAction(tr("Expand"),this);
        collapseAction = new QAction(tr("Collapse"),this);
        exitAction = new QAction(tr("Quit"),this);
    }
    else
    {
        //Criando ações
        clearAction = new QAction(tr("Clear Event Log"), this);
        expandAction = new QAction(tr("&Expand all Devices"), this);
        collapseAction = new QAction(tr("&Collapse all Devices"), this);
        exitAction = new QAction(tr("&Quit"), this);
    }

    aboutCpAction = new QAction(tr("About Control Point"), this);
    aboutBrisaAction = new QAction(tr("About Brisa"), this);
    aboutUpnpAction = new QAction(tr("About UPnP"), this);
    aboutQtAction = new QAction(tr("About QT"), this);

    clearAction->setIcon(QIcon("://rsc/clear.png"));
    connect(clearAction, SIGNAL(triggered()), this, SLOT(clearEventLog()));


    expandAction->setIcon(QIcon("://rsc/plus.png"));
    expandAction->setToolTip("Expand all Devices");
    connect(expandAction, SIGNAL(triggered()), this, SLOT(expandItems()));

    collapseAction->setIcon(QIcon("://rsc/minus.png"));
    collapseAction->setToolTip("Collapse all Devices");
    connect(collapseAction, SIGNAL(triggered()), this, SLOT(collapseItems()));

    aboutCpAction->setIcon(QIcon("://rsc/info.png"));
    connect(aboutCpAction, SIGNAL(triggered()), this, SLOT(aboutControlPoint()));

    aboutBrisaAction->setIcon(QIcon("://rsc/brisa.png"));
    connect(aboutBrisaAction, SIGNAL(triggered()), this, SLOT(aboutBrisa()));

    aboutUpnpAction->setIcon(QIcon("://rsc/upnp.png"));
    connect(aboutUpnpAction, SIGNAL(triggered()), this, SLOT(aboutUpnp()));

    aboutQtAction->setIcon(QIcon("://rsc/qt.png"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    exitAction->setIcon(QIcon("://rsc/exit.png"));
    exitAction->setToolTip("Close the Application");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(closeProgram()));
}

void controlpointgui::createMenus()
{
    fileMenu = menubar->addMenu(tr("&File"));
    fileMenu->addAction(clearAction);
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
    toolBar->addAction(expandAction);
    toolBar->addAction(collapseAction);
    toolBar->addSeparator();
    toolBar->addAction(clearAction);
    toolBar->addSeparator();
    toolBar->addAction(exitAction);
    toolBar->addSeparator();

    addToolBar(Qt::TopToolBarArea, toolBar);

}

void controlpointgui::expandItems()
{
    devicesTreeWidget->expandItem(this->networkItem);
    for (int i = 0; i < items.size(); i++) {
        devicesTreeWidget->expandItem(items[i]);
        QTreeWidgetItem *deviceItem = items[i];
        for (int j = 0; j < deviceItem->childCount(); j++)
            devicesTreeWidget->expandItem(deviceItem->child(j));
    }
}

void controlpointgui::collapseItems()
{
    devicesTreeWidget->collapseItem(this->networkItem);
    for (int i = 0; i < items.size(); i++) {
        devicesTreeWidget->collapseItem(items[i]);
        QTreeWidgetItem *deviceItem = items[i];
        for (int j = 0; j < deviceItem->childCount(); j++)
            devicesTreeWidget->collapseItem(deviceItem->child(j));
    }
}

void controlpointgui::aboutControlPoint()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About Control Point");
    msgBox.setText("<h2>About Control Point</h2>"
                   "<p>"
                   "<b>Basic Control Point Gui Example</b>");
    msgBox.setIconPixmap(QPixmap("://rsc/info.png"));
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
    msgBox.setIconPixmap(QPixmap("://rsc/brisa_logo.png"));
    msgBox.exec();
}

void controlpointgui::aboutUpnp()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About UPnP");
    msgBox.setText(
                "<center><h1>About UPnP</h1></center>"
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
    msgBox.setIconPixmap(QPixmap("://rsc/upnp.png"));
    msgBox.exec();
}

void controlpointgui::createDeviceItem()
{
    currentDeviceItem = new QTreeWidgetItem(networkItem);

    currentDeviceItem->setText(0, currentDev->getAttribute(
                                   Device::FriendlyName) + " - " + currentDev->getAttribute(
                                   Device::Udn));

    if(currentDev->getIconList().size() > 0){
        connect(currentDev, SIGNAL(onReadyDownloadIcons(brisa::upnp::controlpoint::Device*)),
                this, SLOT(onReadyDownloadIcons(brisa::upnp::controlpoint::Device*)));
        currentDev->downloadIcons();

    } else {
        currentDeviceItem->setIcon(0, QIcon("://rsc/device.png"));
        addItem(currentDeviceItem);
    }
}

void controlpointgui::onReadyDownloadIcons(Device *dev)
{
    currentDeviceItem->setIcon(0, dev->getIconList().at(0)->getIcon());
    addItem(currentDeviceItem);
}

void controlpointgui::addItem(QTreeWidgetItem *deviceItem)
{
    items.append(deviceItem);
    QList<Service*> listService;
    listService = currentDev->getServiceList();

    for (int i = 0; i < listService.size(); i++) {
        EventProxy *subscription = controlPoint->getSubscriptionProxy(
                    listService[i]);

        connect(subscription,SIGNAL(eventNotification(brisa::upnp::controlpoint::EventProxy *,QMap<QString, QString>)),
                this, SLOT(changeEventLog(brisa::upnp::controlpoint::EventProxy *,QMap<QString, QString>)));
        subscription->subscribe(20);

        QList<QString> deviceAndService;

        deviceAndService.append(currentDev->getAttribute(
                                    Device::FriendlyName));
        deviceAndService.append(listService[i]->getAttribute(
                                    Service::ServiceType));

        eventToDevice[subscription->getId()] = deviceAndService;

        QTreeWidgetItem *serviceitem = new QTreeWidgetItem(deviceItem);
        serviceitem->setIcon(0, QIcon("://rsc/service.png"));
        serviceitem->setText(0, listService[i]->getAttribute(
                                 Service::ServiceType));

        QList<Action *> listAction;
        listAction = listService[i]->getActionList();

        for (int j = 0; j < listAction.size(); j++) {
            QTreeWidgetItem *actionItem = new QTreeWidgetItem(serviceitem);
            actionItem->setIcon(0, QIcon("://rsc/call.png"));
            actionItem->setText(0, listAction[j]->getName());
        }
    }
}

QSize controlpointgui::getScreenSize() const
{
    return QApplication::desktop()->size();
}


void controlpointgui::callMethod()
{
    QString action = selected->text(0);
    QString service = selected->parent()->text(0);
    QString udn = selected->parent()->parent()->text(0).split(" - ")[1];
    Device* dev = getDeviceByUDN(udn);
    Service* serv = dev->getServiceByType(service);
    connect(serv, SIGNAL(requestFinished(OutArgument, QString)), this, SLOT(
                serviceCall(OutArgument, QString)));
    connect(serv, SIGNAL(requestError(QString, QString)), this, SLOT(
                requestError(QString,QString)));

    InArgument parameters;
    for (int i = 0; i < labels.size(); i++) {
        QString paramName = labels[i]->text().replace("       -", "");
        QString value = editors[i]->text();
        parameters[paramName] = value;
    }
    serv->call(action, parameters);
}

void controlpointgui::serviceCall(OutArgument arguments, QString method)
{
    QString returnMessage = "";
    QMapIterator<QString, QString> it(arguments);
    while (it.hasNext()) {
        it.next();
        returnMessage.append(it.key() + " = " + it.value() + "\n");
    }

    actionsTextEdit->setText("Calling method: " + method + "\n\n" + "Returned: \n"
                      + returnMessage);
}

void controlpointgui::removeDevice(QString usn)
{
    for (int i = 0; i < items.size(); i++) {
        bool change;
        if (items[i]->text(0).split(" - ")[1].compare(usn.split("::")[0]) == 0) {
            change = false;
            if ((devicesTreeWidget->selectedItems().size() > 0)
                    && (!devicesTreeWidget->selectedItems().contains(items[i]))) {
                for (int j = 0; j < items[i]->childCount(); j++) {
                    if (change || devicesTreeWidget->selectedItems().contains(
                                items[i]->child(j))) {
                        change = true;
                        break;
                    }
                    for (int w = 0; w < items[i]->child(j)->childCount(); w++) {
                        if (devicesTreeWidget->selectedItems().contains(
                                    items[i]->child(j)->child(w))) {
                            if (closeable)
                                dialog->close();
                            change = true;
                            break;
                        }
                    }
                }
            }
            if (change || devicesTreeWidget->selectedItems().contains(items[i]))
                doubleClick = false;

            QString UDN = items[i]->text(0).split(" - ")[1];
            devices.removeOne(getDeviceByUDN(UDN));
            delete items[i];
            items.removeAt(i);
        }
    }
}

void controlpointgui::deviceFoundDump(Device *device)
{
    if (device->getAttribute(Device::Udn).compare("") == 0)
        return;
    for (int i = 0; i < devices.size(); i++) {
        if (devices[i]->getAttribute(Device::Udn)
                == device->getAttribute(Device::Udn)) {
            devices[i] = device;
            return;
        }
    }
    devices.append(device);
    this->currentDev = device;
    createDeviceItem();
}

void controlpointgui::lineEnabled(QTreeWidgetItem *item, int collumn)
{
    if ((item->text(collumn).compare("UPnP Network") == 0)
            || (item->parent()->text(collumn).compare("UPnP Network") == 0)) {
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
    if (doubleClick) {
        if (selected->parent() != NULL && selected->parent()->parent() != NULL
                && selected->parent()->parent()->parent()) {
            mountControlDialog();
            dialog->exec();
        }
    }
}

void controlpointgui::changeEventLog(EventProxy *subscription, QMap<
                                     QString, QString> eventVariables)
{

    Q_UNUSED(subscription);
    QList<QString> deviceService = eventToDevice[subscription->getId()];
    qDebug() << "Unicast event message received:";
    for (int i = 0; i < eventVariables.keys().size(); i++) {
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>> State variable " << eventVariables.keys()[i] <<
                    "changed value to " << eventVariables[eventVariables.keys()[i]];
        int row = logBoxTable->rowCount() +1;
        logBoxTable->setRowCount(row);
        QDateTime current = QDateTime::currentDateTime();
        QTableWidgetItem *timeItem = new QTableWidgetItem(current.toString());
        timeItem->setTextAlignment(Qt::AlignVCenter);
        QTableWidgetItem *deviceNameItem = new QTableWidgetItem(deviceService[0]);
        deviceNameItem->setTextAlignment(Qt::AlignVCenter);
        QTableWidgetItem *serviceNameItem = new QTableWidgetItem(deviceService[1]);
        serviceNameItem->setTextAlignment(Qt::AlignVCenter);
        QTableWidgetItem *variableNameItem = new QTableWidgetItem(eventVariables.keys()[i]);
        QTableWidgetItem *valueItem = new QTableWidgetItem(eventVariables[eventVariables.keys()[i]]);
        logBoxTable->setItem(row-1, 0, timeItem);
        logBoxTable->setItem(row-1, 1, deviceNameItem);
        logBoxTable->setItem(row-1, 2, serviceNameItem);
        logBoxTable->setItem(row-1, 3, variableNameItem);
        logBoxTable->setItem(row-1, 4, valueItem);
        logBoxTable->resizeColumnsToContents();
    }
}

void controlpointgui::clearEventLog()
{
    int row = logBoxTable->rowCount();
    for (int i = row; i >= 0; i--) {
        logBoxTable->removeRow(i);
    }
    logBoxTable->resizeColumnsToContents();
}


Device *controlpointgui::getDeviceByUDN(QString UDN)
{
    for (int i = 0; i < devices.size(); i++) {
        if (UDN.compare(devices[i]->getAttribute(Device::Udn))
                == 0)
            return devices[i];
    }
    return NULL;
}

void controlpointgui::processSplashScreen() {
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;

    switch (contSplashScreen) {
    case 1:
        splash->showMessage(QObject::tr("Setting up Brisa ControlPoint..."),
                            topRight, Qt::blue);
        break;
    case 2:
        splash->showMessage(QObject::tr("Waiting Brisa ControlPoint..."),
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

void controlpointgui::mountControlDialog()
{
    closeable = true;
    labels.clear();
    editors.clear();

    dialog = new QDialog();
    dialog->setWindowTitle("Action Invocation");
    dialog->setWindowIcon(QIcon("://rsc/call.png"));

    QLabel *brisaText = new QLabel();
    brisaText->setText("BRisa UPnP Control Point");

    QLabel *serviceText = new QLabel();
    serviceText->setPixmap(QPixmap("://rsc/service.png"));
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
    invoke->setIcon(QIcon("://rsc/call.png"));
    invoke->setText("Invoke");
    connect(invoke, SIGNAL(clicked()), this, SLOT(callMethod()));

    QHBoxLayout *button = new QHBoxLayout();
    button->addLayout(actionAndService);
    button->addWidget(invoke);

    QVBoxLayout *vertical = new QVBoxLayout();
    vertical->addLayout(button);

    QString udn = selected->parent()->parent()->text(0).split(" - ")[1];
    QString type = selected->parent()->text(0);
    QList<Action *> actions =
            getDeviceByUDN(udn)->getServiceByType(type)->getActionList();

    Action *realAction = 0;
    for (int i = 0; i < actions.size(); i++) {
        Action *now = actions[i];
        if (now->getName() == selected->text(0)) {
            realAction = now;
            break;
        }
    }
    QList<Argument *> inArguments;
    for (int i = 0; i < realAction->getArgumentList().size(); i++) {
        if ((realAction->getArgumentList()[i])->getAttribute(
                    Argument::Direction).compare("in") == 0) {
            inArguments.append(realAction->getArgumentList()[i]);
        }
    }
    if (inArguments.size() > 0) {
        QLabel *a = new QLabel();
        a->setText("  ");
        QLabel *in = new QLabel();
        in->setText("'in' Arguments:");
        vertical->addWidget(a);
        vertical->addWidget(in);
        for (int i = 0; i < inArguments.size(); i++) {
            QLabel *parameter = new QLabel();
            parameter->setText("       -" + inArguments[i]->getAttribute(
                                   Argument::ArgumentName));
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

void controlpointgui::multicastEventReceived(QString variableName,
                                             QString newValue)
{
    qDebug() << "Multicast event message received:";
    qDebug() << "State variable: " << variableName << " changed value to " <<
                newValue;
}

void controlpointgui::multicastEventRawReceived(OutArgument raw)
{
    qDebug() << "Multicast event message raw received:";
    qDebug() << "Raw: " << raw;
}

void controlpointgui::closeProgram()
{
    this->close();
}

void controlpointgui::requestError(QString errorMessage, QString methodName)
{
    actionsTextEdit->setText(errorMessage + " when calling " + methodName);
}
