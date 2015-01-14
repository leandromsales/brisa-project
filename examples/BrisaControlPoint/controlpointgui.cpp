#include "controlpointgui.h"

#include <QString>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>

#include "upnp/controlpoint/ControlPoint"
#include "upnp/controlpoint/EventProxy"
#include "upnp/controlpoint/Device"
#include "upnp/controlpoint/Service"

#define CURRENT_DIR QString("/home/larissa/UFAL/Labs/CompeLab_BlackBerry/Brisa/brisa-port-qt5/brisa-project/examples/BrisaControlPoint/")

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
    splash
            = new QSplashScreen(QPixmap(CURRENT_DIR + "BrisaSplashScreenControlPoint.png"));

    controlPoint = new ControlPoint();

    connect(controlPoint, SIGNAL(deviceGone(QString)), this, SLOT(removeDevice(
            QString)));
    connect(controlPoint, SIGNAL(deviceFound(brisa::upnp::controlpoint::Device*))
            , this, SLOT(deviceFoundDump(brisa::upnp::controlpoint::Device*)));
    controlPoint->start();
    controlPoint->discover();

    connect(treeWidgetCP, SIGNAL(itemClicked(QTreeWidgetItem * , int )),
            this, SLOT(lineEnabled(QTreeWidgetItem * , int)));
    connect(treeWidgetCP, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
            this, SLOT(call(QTreeWidgetItem *, int)));

    connect(controlPoint, SIGNAL(multicastReceived(QString,QString)), this,
            SLOT(multicastEventReceived(QString,QString)));
    connect(controlPoint, SIGNAL(multicastReceivedRaw(OutArgument)), this,
            SLOT(multicastEventRawReceived(OutArgument)));

    networkItem = new QTreeWidgetItem(treeWidgetCP);
    networkItem->setIcon(0, QIcon(CURRENT_DIR + "network.png"));
    networkItem->setText(0, "UPnP Network");

    contSplashScreen = 0;
    doubleClick = false;
}

controlpointgui::~controlpointgui()
{
    controlPoint->stop();
    delete controlPoint;
}

void controlpointgui::setUpTableWidget()
{
      QStringList list;
      list <<"Time"<<"Device"<<"Service" << "State Variables" << "Value";
      //QTableWidget* tableWidget = new QTableWidget(this);
      tableWidget->setRowCount(0);
      tableWidget->setColumnCount(5);
      tableWidget->setHorizontalHeaderLabels(list);
      tableWidget->setEditTriggers(NULL);

}

void controlpointgui::createActions()
{

    clearAction = new QAction(tr("Clear Event Log"), this);
    clearAction->setIcon(QIcon(CURRENT_DIR + "clear.png"));
    connect(clearAction, SIGNAL(triggered()), this, SLOT(clearEventLog()));


    expandAction = new QAction(tr("&Expand all Devices"), this);
    expandAction->setIcon(QIcon(CURRENT_DIR + "plus.png"));
    expandAction->setToolTip("Expand all Devices");
    connect(expandAction, SIGNAL(triggered()), this, SLOT(expandItems()));

    collapseAction = new QAction(tr("&Collapse all Devices"), this);
    collapseAction->setIcon(QIcon(CURRENT_DIR + "minus.png"));
    collapseAction->setToolTip("Collapse all Devices");
    connect(collapseAction, SIGNAL(triggered()), this, SLOT(collapseItems()));

    aboutCpAction = new QAction(tr("About Control Point"), this);
    aboutCpAction->setIcon(QIcon(CURRENT_DIR + "info.png"));
    connect(aboutCpAction, SIGNAL(triggered()), this, SLOT(aboutControlPoint()));

    aboutBrisaAction = new QAction(tr("About Brisa"), this);
    aboutBrisaAction->setIcon(QIcon(CURRENT_DIR + "brisa.png"));
    connect(aboutBrisaAction, SIGNAL(triggered()), this, SLOT(aboutBrisa()));

    aboutUpnpAction = new QAction(tr("About UPnP"), this);
    aboutUpnpAction->setIcon(QIcon(CURRENT_DIR + "upnp.png"));
    connect(aboutUpnpAction, SIGNAL(triggered()), this, SLOT(aboutUpnp()));

    aboutQtAction = new QAction(tr("About QT"), this);
    aboutQtAction->setIcon(QIcon(CURRENT_DIR + "qt.png"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    exitAction = new QAction(tr("&Quit"), this);
    exitAction->setIcon(QIcon(CURRENT_DIR + "exit.png"));
    exitAction->setToolTip("Close the Application");
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
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
    treeWidgetCP->expandItem(this->networkItem);
    for (int i = 0; i < items.size(); i++) {
        treeWidgetCP->expandItem(items[i]);
        QTreeWidgetItem *deviceItem = items[i];
        for (int j = 0; j < deviceItem->childCount(); j++)
            treeWidgetCP->expandItem(deviceItem->child(j));
    }
}

void controlpointgui::collapseItems()
{
    treeWidgetCP->collapseItem(this->networkItem);
    for (int i = 0; i < items.size(); i++) {
        treeWidgetCP->collapseItem(items[i]);
        QTreeWidgetItem *deviceItem = items[i];
        for (int j = 0; j < deviceItem->childCount(); j++)
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
    msgBox.setIconPixmap(QPixmap(CURRENT_DIR + "info.png"));
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
    msgBox.setIconPixmap(QPixmap(CURRENT_DIR + "brisa_logo.png"));
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
    msgBox.setIconPixmap(QPixmap(CURRENT_DIR + "upnp.png"));
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
        currentDeviceItem->setIcon(0, QIcon(CURRENT_DIR + "device.png"));
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
        serviceitem->setIcon(0, QIcon(CURRENT_DIR + "service.png"));
        serviceitem->setText(0, listService[i]->getAttribute(
                Service::ServiceType));

        QList<Action *> listAction;
        listAction = listService[i]->getActionList();

        for (int j = 0; j < listAction.size(); j++) {
            QTreeWidgetItem *actionItem = new QTreeWidgetItem(serviceitem);
            actionItem->setIcon(0, QIcon(CURRENT_DIR + "call.png"));
            actionItem->setText(0, listAction[j]->getName());
        }
    }
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

    textEdit->setText("Calling method: " + method + "\n\n" + "Returned: \n"
                      + returnMessage);
}

void controlpointgui::removeDevice(QString usn)
{
    for (int i = 0; i < items.size(); i++) {
        bool change;
        if (items[i]->text(0).split(" - ")[1].compare(usn.split("::")[0]) == 0) {
            change = false;
            if ((treeWidgetCP->selectedItems().size() > 0)
                    && (!treeWidgetCP->selectedItems().contains(items[i]))) {
                for (int j = 0; j < items[i]->childCount(); j++) {
                    if (change || treeWidgetCP->selectedItems().contains(
                            items[i]->child(j))) {
                        change = true;
                        break;
                    }
                    for (int w = 0; w < items[i]->child(j)->childCount(); w++) {
                        if (treeWidgetCP->selectedItems().contains(
                                items[i]->child(j)->child(w))) {
                            if (closeable)
                                dialog->close();
                            change = true;
                            break;
                        }
                    }
                }
            }
            if (change || treeWidgetCP->selectedItems().contains(items[i]))
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
        int row = tableWidget->rowCount() +1;
        tableWidget->setRowCount(row);
        QDateTime current = QDateTime::currentDateTime();
        QTableWidgetItem *timeItem = new QTableWidgetItem(current.toString());
        timeItem->setTextAlignment(Qt::AlignVCenter);
        QTableWidgetItem *deviceNameItem = new QTableWidgetItem(deviceService[0]);
        deviceNameItem->setTextAlignment(Qt::AlignVCenter);
        QTableWidgetItem *serviceNameItem = new QTableWidgetItem(deviceService[1]);
        serviceNameItem->setTextAlignment(Qt::AlignVCenter);
        QTableWidgetItem *variableNameItem = new QTableWidgetItem(eventVariables.keys()[i]);
        QTableWidgetItem *valueItem = new QTableWidgetItem(eventVariables[eventVariables.keys()[i]]);
        tableWidget->setItem(row-1, 0, timeItem);
        tableWidget->setItem(row-1, 1, deviceNameItem);
        tableWidget->setItem(row-1, 2, serviceNameItem);
        tableWidget->setItem(row-1, 3, variableNameItem);
        tableWidget->setItem(row-1, 4, valueItem);
        tableWidget->resizeColumnsToContents();
    }
}

void controlpointgui::clearEventLog()
{
        int row = tableWidget->rowCount();
        for (int i = row; i >= 0; i--) {
            tableWidget->removeRow(i);
        }
        tableWidget->resizeColumnsToContents();
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
    dialog->setWindowIcon(QIcon(CURRENT_DIR + "call.png"));

    QLabel *brisaText = new QLabel();
    brisaText->setText("BRisa UPnP Control Point");

    QLabel *serviceText = new QLabel();
    serviceText->setPixmap(QPixmap(CURRENT_DIR + "service.png"));
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
    invoke->setIcon(QIcon(CURRENT_DIR + "call.png"));
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

void controlpointgui::requestError(QString errorMessage, QString methodName)
{
    textEdit->setText(errorMessage + " when calling " + methodName);
}
