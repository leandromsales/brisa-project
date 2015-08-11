#include "controlpoint.h"

#include <QMap>
#include <QDir>

#include <iostream>
#include <fstream>
#include <typeinfo>

#define WHITE_SPACES ("[\\r\\n\\t\\v\\s]*")

extern ofstream *out;
Logger *logger_ptr;

#define logger (*logger_ptr)

bool openCalled = false;

/**
 * Constructor.
 */
ControlPoint::ControlPoint(QDir *root, bool onOpen, QObject *parent):
    QObject(parent), controlPoint(new BrisaControlPoint)
{    
    this->rootDir = root;
    this->invokeOnOpen = onOpen;

    logger_ptr = new Logger(*out);

    if(!QDir(this->rootDir->absolutePath() + QDir::separator() + EVENTS_PATH).exists()) {
        rootDir->mkdir(EVENTS_PATH);
    }

    connect(this->controlPoint, SIGNAL(deviceFound(BrisaControlPointDevice*)),
            this, SLOT(onNewDevice(BrisaControlPointDevice*)));
    connect(this->controlPoint, SIGNAL(deviceGone(QString)),
            this, SLOT(onRemovedDevice(QString)));

    connect(this->controlPoint, SIGNAL(multicastReceived(QString,QString)),
            this, SLOT(multicastEventReceived(QString,QString)));
    connect(this->controlPoint, SIGNAL(multicastReceivedRaw(BrisaOutArgument)),
            this, SLOT(multicasteventReceivedRaw(BrisaOutArgument)));
}

ControlPoint::~ControlPoint()
{
    delete controlPoint;
}

//==============================================================================
/**
 * Deprecated
 * Converts QString in C ANSI string
 */
const char *ControlPoint::c_str(QString & str) const
{
    return str.toStdString().c_str();
}

/**
 * Get time on QString
 */
QString ControlPoint::getTime()
{
    time_t inicio = time(0); //Pegar a hora atual
    QString sTime(ctime(&inicio));
    return sTime.remove("\n");
}

/**
 * Starts the control point
 */
void ControlPoint::start()
{
    logger << " ===== Starting Control Point at " << getTime() << endl << endl;
    logger << "Root: " << rootDir->absolutePath().toStdString().c_str() << endl;
    logger << "Invoke services on Open: " << this->invokeOnOpen << endl;

    controlPoint->start();
    controlPoint->discover();
}

/**
 * Restarts the control point
*/
void ControlPoint::restart()
{
    logger << "\n\n===== Restarting Control Point at " << getTime() << endl << endl;
    controlPoint->stop();
    controlPoint->start();
    controlPoint->discover();
}

//==============================================================================
/**
  * public slot
  * method call when one new device is found on network
 **/
void ControlPoint::onNewDevice(BrisaControlPointDevice *c)
{
    if (c->getAttribute(BrisaControlPointDevice::Udn).compare("") == 0)
        return;

    for(int i = 0; i < devices.size(); i++) {
        if(devices[i]->getAttribute(BrisaControlPointDevice::Udn) ==
                c->getAttribute(BrisaControlPointDevice::Udn)) {
            devices[i] = c;
            return;
        }
    }

    QString deviceName = c->getAttribute(BrisaControlPointDevice::FriendlyName);
    logger << "======= onNewDevice =====" << endl;
    logger << "Device found: " << deviceName << endl;
    logger << "UDN: " << c->getAttribute(BrisaControlPointDevice::Udn) << endl;

    this->devices.append(c);

    //Creates the filesystem directories' structure
    addDeviceFolders(c);
}

/**
 * private
 * Creates the structure of directories for each device
 */
void ControlPoint::addDeviceFolders(BrisaControlPointDevice *d)
{
    QDir tempDir = *rootDir;

    QMap<QString, QString> attributes;
    attributes["DeviceType"] = d->getAttribute(BrisaControlPointDevice::DeviceType);
    attributes["FriendlyName"] = d->getAttribute(BrisaControlPointDevice::FriendlyName);
    attributes["udn"] = d->getAttribute(BrisaControlPointDevice::udn);
    attributes["urlBase"] = d->getAttribute(BrisaControlPointDevice::urlBase);
    attributes["manufacturer"] = d->getAttribute(BrisaControlPointDevice::manufacturer);
    attributes["manufacturerUrl"] = d->getAttribute(BrisaControlPointDevice::manufacturerUrl);
    attributes["modelDescription"] = d->getAttribute(BrisaControlPointDevice::modelDescription);
    attributes["modelName"] = d->getAttribute(BrisaControlPointDevice::modelName);
    attributes["modelNumber"] = d->getAttribute(BrisaControlPointDevice::modelNumber);
    attributes["modelUrl"] = d->getAttribute(BrisaControlPointDevice::modelUrl);
    attributes["serialNumber"] = d->getAttribute(BrisaControlPointDevice::serialNumber);
    attributes["presentationUrl"] = d->getAttribute(BrisaControlPointDevice::presentationUrl);

    QString deviceName = attributes["FriendlyName"];

    //mkdir devices folder
    tempDir.mkdir(DEVICES_PATH);
    tempDir.cd(DEVICES_PATH);

    QString descFileStr = tempDir.absolutePath() + QDir::separator() + deviceName + DESCRIPTION_EXT;
    QFile descFile(descFileStr);
    descFile.open(QIODevice::WriteOnly);
    QTextStream outToDescFile(&descFile);

    foreach(QString k, attributes.keys()) {
        outToDescFile << k << ": " << attributes[k] << "\r\n";
    }
    outToDescFile << "\r\n\t";

    //mkdir this device folder
    tempDir.mkdir(deviceName);
    tempDir.cd(deviceName);

    //For each service in service list
    foreach(BrisaControlPointService* service, d->getServiceList()) {

        QString serviceName = service->getAttribute(
                    BrisaControlPointService::ServiceType);

        //create a folder
        tempDir.mkdir(serviceName);
        logger << "Service: " << serviceName << endl;
        outToDescFile << "Service: " << serviceName  << "\r\n\t\t";
        tempDir.cd(serviceName);

        createEventsFiles(service, &tempDir, outToDescFile);

        //for each action in action list...
        foreach(BrisaAction *action, service->getActionList()) {
            //create a file
            QString filePath = tempDir.absolutePath() +
                    QDir::separator() + action->getName() + ACTION_EXT;

            fileActionMap.insert(action->getName(), filePath);

            QFile actionFile(filePath);
            actionFile.open(QIODevice::WriteOnly);
            QTextStream outToActionFile(&actionFile);

            outToDescFile << "Action: " << action->getName() << "\r\n\t\t";

            int inArgCount = 0;
            //for each argument in argument list...
            foreach(BrisaArgument* arg, action->getArgumentList()) {
                if(arg->getAttribute(BrisaArgument::Direction) == "in")
                {
                    ++inArgCount;
                    logger << "\t" << arg->getAttribute(BrisaArgument::ArgumentName).toStdString() << endl;
                    outToActionFile << arg->getAttribute(BrisaArgument::ArgumentName) << "=\n";
                    outToDescFile << "\t\tIn Argument: " << arg->getAttribute(BrisaArgument::ArgumentName)
                                  << "\r\n\t\t";
                }
            } //end arglist      

            //IF action hasnt in args
            if(inArgCount == 0) {
                outToActionFile << NO_ARGS_REQUIRED << "\r\n";
                outToDescFile << "\t\t" << NO_ARGS_REQUIRED << "\r\n\t\t";
            }

            actionFile.close();

        } // end action list

        tempDir.cdUp();

    } //end service list
    outToDescFile << "\r\n";
    descFile.close();

    //foreach(BrisaControlPointDevice *emb, d->getEmbeddedDeviceList()) {
    //    addDeviceFolders(emb);
    //}
}

/**
 * public
 * Creates the events files
 */
void ControlPoint::createEventsFiles(BrisaControlPointService *service, QDir *cDir, QTextStream &outToDesc)
{
    BrisaEventProxy *subscription = controlPoint->getSubscriptionProxy(service);

    QString eventSub = subscription->getEventSub();
    qDebug() << "EventSub: " << eventSub;

    //Do not allow to create files like "/SwitchPower/eventSub"
    if(eventSub.contains(QDir::separator())) {
        QStringList list = eventSub.split(QDir::separator());
        if(!list.isEmpty())
            eventSub = list.back();
    }

    if(eventSub.contains("event=")) {
        eventSub = eventSub.split(":").last();
    }

    QString filePath = cDir->absolutePath() + QDir::separator() +
            eventSub + EVENT_EXT;

    outToDesc << "Event: " << eventSub << "\r\n\t\t";

    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.close();
}


/**
  * public slot
  * Slot call when one device was removed.
  * this slot remove device from device list if the device leave
 **/
void ControlPoint::onRemovedDevice(QString udn)
{
    //Only the first part of UDN
    QStringList UDN = udn.split("::");
    BrisaControlPointDevice *device = getDeviceByUDN(UDN.at(0));

    //If UDN was found
    if(device!=NULL) {
        QString deviceName = device->getAttribute(
                    BrisaControlPointDevice::FriendlyName);

        logger << "Removing device: " << deviceName << endl;
        devices.removeOne(device);
        this->removeDeviceFolders(deviceName);
        logger << "Item removed!" << endl;

    } else {
        logger << "Device not found!" << endl;
    }
    restart();
}

/**
 * private
 * Remove the structure of directories for each device
 */
void ControlPoint::removeDeviceFolders(QString deviceName)
{
    QDir temp = *rootDir;
    temp.cd(DEVICES_PATH);
    temp.remove(deviceName + DESCRIPTION_EXT);
    temp.cd(deviceName);
    deleteDir(temp);
}

/**
 * Deleta diretorios recursivamente
*/
void ControlPoint::deleteDir(QDir &dir) {

    //Apaga os arquivos
    QStringList fileList = dir.entryList(QDir::Files);
    foreach(QString file, fileList) {
        dir.remove(file);
    }

    //Apaga os subdiretorios,recursivamente
    QStringList subdirList = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    foreach(QString subdir, subdirList) {
        dir.cd(subdir);
        deleteDir(dir);
    }

    QString dirPath = dir.path();
    dir.cdUp();
    dir.rmdir(dirPath);
}

//==============================================================================
/**
 * private
 * get service of device
 **/
QStringList ControlPoint::getServices(BrisaControlPointDevice *c)
{
    QStringList Services;

    foreach(BrisaControlPointService *s, c->getServiceList().toVector())
        Services.append(s->getAttribute(BrisaControlPointService::ServiceType));

    return Services;
}

/**
 * private
 * get actions of one service
**/
QStringList ControlPoint::getActions(BrisaControlPointService *s)
{
    QStringList actions;
    foreach(BrisaAction *a, s->getActionList().toVector())
        actions.append(a->getName());
    return actions;
}

/**
  * private
  * get device by Udn
 **/
BrisaControlPointDevice* ControlPoint::getDeviceByUDN(QString UDN)
{
    for(int i = 0; i < devices.size(); i++) {
        if(UDN.compare(devices[i]->getAttribute(BrisaControlPointDevice::Udn)) == 0)
            return devices[i];
    }
    return NULL;
}

/**
  * private
  * get device by name
 **/
BrisaControlPointDevice* ControlPoint::getDeviceByName(QString name)
{
    for(int i = 0; i < devices.size(); i++) {
        if(name.compare(devices[i]->getAttribute(BrisaControlPointDevice::FriendlyName)) == 0)
            return devices[i];
    }
    return NULL;
}

/**
*
*/
bool ControlPoint::isValidDeviceFile(const QString &filepath)
{
    if(filepath.startsWith(EVENTS_PATH) || filepath.endsWith(RETURN_EXT) ||
            filepath.endsWith(EVENTLOG_EXT) ||
            filepath.endsWith(DESCRIPTION_EXT))
    {
        return false;
    }
    else if(filepath.split("/").last().startsWith("."))
    {
        return false;
    }

    return true;
}

//==============================================================================
/**
 * public Q_SLOTS
 * Receive notification when a file was written
 **/
void ControlPoint::fileWrite(const QString &filepath)
{
    //Files that should be ignored
    if(!isValidDeviceFile(filepath)) {
        //logger << filepath << "Isn't Device File" << endl;
        return;
    }

    // Filename:       /Devices/Device/Service/<Action or Event>
    //            [0]  /  [1]  / [2]  /  [3]  /       [4]

    QStringList filelist = filepath.split("/");
    QString fileName = filelist.takeLast();
    QString serviceName = filelist.takeLast();
    QString deviceName = filelist.takeLast();

    qDebug() << "Device: " << deviceName;
    qDebug() << "Service: " << serviceName;
    qDebug() << "Action: " << fileName;

    BrisaControlPointDevice* device = getDeviceByName(deviceName);
    if(device == NULL) {
        logger << "Device not found!" << endl;
        qDebug() << "Devices:";

        foreach(BrisaControlPointDevice *d, devices) {
            qDebug() << d->getAttribute(BrisaControlPointDevice::FriendlyName);
        }

        return;
    }

    BrisaControlPointService* service = device->getServiceByType(serviceName);
    if(service==NULL) {
        logger << "Service not found!" << endl;
        return;
    }

    //Action file
    if(fileName.endsWith(ACTION_EXT)) {

        //Removes extension: '.action'
        QString actionName = fileName;
        actionName.remove(ACTION_EXT);

        qDebug() << "Trying calling action: " << actionName;

        connect(service, SIGNAL(requestFinished(BrisaOutArgument, QString)),
                this, SLOT(serviceCall(BrisaOutArgument, QString)));
        //Connect to catch errors after the call of an action
        connect(service, SIGNAL(requestError(QString, QString)),
                this, SLOT( requestError(QString,QString)));

        QFile file(rootDir->absolutePath() + filepath);
        file.open(QIODevice::ReadOnly);
        QTextStream inFromFile(&file);
        QString line;

        BrisaInArgument arguments;

        //////////////////////////////////

        while(!inFromFile.atEnd()) {
            line = inFromFile.readLine();

             //Eh tarefa do usuario se certificar dos espaços em branco.
            // line.remove(QRegExp(WHITE_SPACES)); //Alguns dispositivos exigem espaços em branco

            logger << line << endl;
            if(!line.isEmpty()) {
                //Se soh invoca quando salva, ele nao adiciona args, mas invoca mesmo assim.
                if(line.startsWith(NO_ARGS_REQUIRED) || line.startsWith("[RETURN")) {
                    if(!invokeOnOpen) {
                        ; //Nao faz nada
                    } else {
                        service->call(actionName, arguments); //invoca sem argumentos
                        file.close();
                        return;
                    }
                } else if(!openCalled){
                    //echo "param1=3; param2=4; param3=5" > Action1.action
                    QStringList multiline = line.split(";");

                    foreach(QString ln, multiline) {
                        QStringList lineSplited = ln.split("=");
                        if(lineSplited.size() == 2){
                            //argumento=valor, se valor for vazio, fica argumento=""
                            arguments[lineSplited[0]] = lineSplited[1];
                        }
                    }
                } else { //Se o metodo foi chamado por fileOpen() e a action exige args, nao executa.
                    return;
                }
            }
        }
        file.close();

        //Finally calls the service
        qDebug() << "Calling " << actionName << " with " << arguments;
        service->call(actionName, arguments);

        //Event file
    } else if(fileName.endsWith(EVENT_EXT)) {

        QFile file(rootDir->absolutePath() + filepath);
        file.open(QIODevice::ReadOnly);
        QTextStream inFromFile(&file);
        QString line;

        while(!inFromFile.atEnd()) {
            inFromFile >> line;
            logger << line << endl;

            //The order should be that because "subscribe" is a substring of "unsubscribe"
            if(line.contains(UNSUBSCRIBE_LINE)) {
                logger << "Unsubscribing on " << serviceName << endl;
                unsubscribe(service);
                return;
            } else if(line.contains(SUBSCRIBE_LINE)) {
                logger << "Subscribing on " << serviceName << endl;
                subscribe(service);
                return;
            }
        }
        file.close();
    }

}

/**
 * public Q_SLOTS
 * Receive notification when a file was opened
 **/
void ControlPoint::fileOpen(const QString &filepath)
{
    //If mode --onOpen unabled
    if(!invokeOnOpen) {
        return;
    } else {
        openCalled = true;
        fileWrite(filepath); //Um unico método trata tudo.
    }
    openCalled = false;
}

//==============================================================================
/**
 * public Q_SLOTS
 * Receive notification when the system is unmounted
 **/
void ControlPoint::fsUmounted()
{
    logger << "\n\n===== Stopping the Control Point at " << getTime() << endl;
    controlPoint->stop();

    logger << "Removing devices... " << endl;
    foreach(BrisaControlPointDevice *dev, devices)
    {
        QString udn = dev->getAttribute(BrisaControlPointDevice::Udn);

        //Only the first part of UDN
        QStringList UDN = udn.split("::");
        BrisaControlPointDevice *device = getDeviceByUDN(UDN.at(0));

        //If UDN was found
        if(device!=NULL) {
            QString deviceName = device->getAttribute(
                        BrisaControlPointDevice::FriendlyName);
            logger << "Removing device: " << deviceName << endl;
            removeDeviceFolders(deviceName);
        } else {
            logger << "Device not found!" << endl;
        }
    }
    logger << "BRisa Control Point was stopped" << endl;
    logger << "Exiting..." << endl;
    qApp->exit(0);
}

/**
 * private slots
 * When the service with args is called
 * Write the return result in a log file
 **/
void ControlPoint::serviceCall(BrisaOutArgument arguments, QString methodName)
{    
    QString filename(fileActionMap[methodName]);
    filename.remove(ACTION_EXT);
    filename.append(RETURN_EXT);

    qDebug() << "Service call";

    if(this->invokeOnOpen) {
        QString textFile = FileManager::readFile(fileActionMap[methodName]);
        qDebug() << textFile;
        if(textFile.startsWith(NO_ARGS_REQUIRED) || textFile.startsWith("[RETURN")) {
            filename = fileActionMap[methodName];
        }
    }

    QString returnMessage = "[RETURN (" + getTime() + ")]: ";

    QMapIterator<QString, QString> it(arguments);
    while (it.hasNext()) {
        it.next();
        returnMessage.append(it.key() + "=" + it.value() + "\r\n");
    }

    returnMessage.append("\r\n");
    logger << "Calling method: " << methodName << endl;
    logger << "Returned: " << returnMessage << endl;

    FileManager::writeFile(filename, returnMessage);
}

/**
 * private slots
 * Displays an error message if a service request fails
 **/
void ControlPoint::requestError(QString errorMessage, QString methodName)
{
    QString filename(fileActionMap[methodName]);
    filename.remove(ACTION_EXT);
    filename.append(RETURN_EXT);

    qDebug() << "Service Error";

    if(this->invokeOnOpen) {
        qDebug() << "Invoke on open";
        QString textFile = FileManager::readFile(fileActionMap[methodName]);
        qDebug() << textFile;
        if(textFile.startsWith(NO_ARGS_REQUIRED) || textFile.startsWith("[RETURN")) {
            filename = fileActionMap[methodName];
        }
    }

    qDebug() << "Filename: " << filename;
    QString returnMessage = "[RETURN (" + getTime() + ")]: ";
    returnMessage.append(errorMessage + " when calling " + methodName + "\r\n");
    logger << returnMessage;

    FileManager::writeFile(filename, returnMessage);
}

//==============================================================================
/**
 * public slot
 * Called when receives an multicast event
 */
void ControlPoint::multicastEventReceived(QString variable, QString newValue)
{
    logger << "Multicast event received!" << endl;
    logger << "Variable " + variable + " changed value to " + newValue << endl;
}

/**
 * public slot
 * Called when receives an multicast event
**/
void ControlPoint::multicasteventReceivedRaw(BrisaOutArgument raw)
{

    QFile file(rootDir->absolutePath()+ QDir::separator() + EVENTS_PATH +
               QDir::separator() + MULTICAST_FILE_NAME + EVENTLOG_EXT);
    file.open(QIODevice::ReadWrite);
    QTextStream fileStream(&file);

    QString line;
    do {
        line = fileStream.readLine();
    } while(!line.isNull());

    logger << "----------------------------\n" << "Multicast event raw:" << endl;
    fileStream << "----------------------------\n" << "Multicast event raw:\n";

    foreach (QString key, raw.keys()) {
        logger << key + ": " + raw.value(key);
        fileStream << key + ": " + raw.value(key) << "\n";
    }
    file.close();
}

//==============================================================================
/**
* Public
* Subscribes to receive unicast events
*/
void ControlPoint::subscribe(BrisaControlPointService *service, int timeout)
{
    BrisaEventProxy *subscription = controlPoint->getSubscriptionProxy(service);

    connect(subscription, SIGNAL(eventNotification(BrisaEventProxy *, QMap<QString, QString>)),
            this, SLOT(unicastEventReceived(BrisaEventProxy *,QMap<QString, QString>)));

    subscription->subscribe(timeout);
}

/**
* Public
* Unsubscribes unicast events
*/
void ControlPoint::unsubscribe(BrisaControlPointService *service)
{
    BrisaEventProxy *unsubscription = controlPoint->getSubscriptionProxy(service);
    unsubscription->unsubscribe();
}

//======================
/**
 * public slot
 * Called when receives an unicast event
 */
void ControlPoint::unicastEventReceived(BrisaEventProxy *subscription,
                                        QMap<QString, QString> variables)
{
    Q_UNUSED(subscription);

    QFile file(rootDir->absolutePath()+ QDir::separator() + EVENTS_PATH +
               QDir::separator() + EVENTS_FILE_NAME + EVENTLOG_EXT);
    file.open(QIODevice::ReadWrite);
    QTextStream fileStream(&file);

    QString line;
    do {
        line = fileStream.readLine();
    } while(!line.isNull());

    logger << "----------------------------\n" << "Unicast event message:" << endl;
    fileStream << "----------------------------\n" << "Unicast event message:\n";

    foreach (QString key, variables.keys()) {
        logger << key + ": " + variables.value(key);
        fileStream << key + ": " + variables.value(key) << "\n";
    }
    file.close();
}


//=============================================================================

