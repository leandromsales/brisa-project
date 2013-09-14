#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QVariantMap>
#include <qdeclarativecontext.h>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/GroupDataModel>

#include "upnp/controlpoint/brisacontrolpoint.h"
#include "upnp/controlpoint/brisacontrolpointdevice.h"

using namespace bb::cascades;
using namespace Brisa;


class ViewController: public QObject {
Q_OBJECT

public:
	explicit ViewController(QmlDocument * context, QObject *parent = 0);
	~ViewController();
	void goesToQML();
	GroupDataModel *getControlPointModel();
	QmlDocument * uiCtx;

signals:

public slots:
	void onReadyDownloadIcons(BrisaControlPointDevice* device);
//	void lineEnabled(QTreeWidgetItem * item, int collumn);
	void deviceFoundDump(BrisaControlPointDevice *device);
	void serviceCall(BrisaOutArgument, QString);
	void requestError(QString errorMessage, QString methodName);
	void removeDevice(QString usn);
	void callMethod();
	void expandItems();
	void collapseItems();
	void aboutControlPoint();
	void aboutUpnp();
	void aboutBrisa();
	void processSplashScreen();
	void call();//QTreeWidgetItem * item, int collumn);
	void mountControlDialog();
	void changeEventLog(BrisaEventProxy *subscription,
			QMap<QString, QString> eventVariables);
	void clearEventLog();
	void multicastEventReceived(QString variableName, QString newValue);
	void multicastEventRawReceived(BrisaOutArgument raw);

private:
	BrisaControlPointDevice* getDeviceByUDN(QString UDN);
	void createDeviceItem();
	void createActions();
	void createMenus();
	void createToolBars();
	void setUpTableWidget();
//	void addItem(QTreeWidgetItem *deviceItem);

	GroupDataModel *controlPointDataModel;

	QList<BrisaControlPointDevice*> devices;
//	QList<QTreeWidgetItem*> items; //Mudar isso para uma interface mobile

	BrisaControlPoint *controlPoint;
	BrisaControlPointDevice *currentDev;
	QList<BrisaControlPointDevice *> currentDeviceItem; //Mudar isso para uma interface mobile

//	QDialog *dialog; //Mudar isso para uma interface mobile
	QList<QLineEdit*> editors;
	QList<QLabel*> labels;
	QMap<int, QList<QString> > eventToDevice;

};

#endif // VIEWCONTROLLER_H
