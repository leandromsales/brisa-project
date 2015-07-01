#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include <QMap>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QDeclarativeContext>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>

#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/GroupDataModel>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>

#include "upnp/controlpoint/ControlPoint"
#include "upnp/controlpoint/Device"

#include "DeviceList.hpp"
#include "model/ExecutableAction.hpp"

class ViewController: public QObject {
Q_OBJECT
Q_PROPERTY(bool searching READ searching NOTIFY onSearching FINAL)
Q_PROPERTY(bool isConnected READ isConnected NOTIFY onConnected FINAL)
Q_PROPERTY(bool isEmptyList READ isEmptyList NOTIFY onEmptyListChanged FINAL)
Q_PROPERTY(bb::cascades::ArrayDataModel * devicesModel READ devicesModel NOTIFY onDevicesModelChanged FINAL)
Q_PROPERTY(bb::cascades::GroupDataModel * eventLogModel READ eventLogModel NOTIFY onEventLogModelChanged FINAL)

public:
	explicit ViewController(QObject *parent = 0);
	~ViewController();
	bool searching();
	bool isEmptyList();
	bool isConnected();
	Q_INVOKABLE void onPopPage();
	Q_INVOKABLE void refreshList();
	Q_INVOKABLE void stopControlPoint();
	Q_INVOKABLE void startControlPoint();
	Q_INVOKABLE void callBBMCard();
	Q_INVOKABLE void callSettingsCard();
	Q_INVOKABLE void startSearchingTimer();
	Q_INVOKABLE void removeEvent(QVariantMap data);
	Q_INVOKABLE void clearEventLog();
	Q_INVOKABLE bb::cascades::ArrayDataModel *getGenericDataModelByID(QString ID);
	bb::cascades::ArrayDataModel *devicesModel();
	bb::cascades::GroupDataModel *eventLogModel();

signals:
	void onConnected();
	void onSearching();
	void emptyList(bool);
	void onEmptyListChanged();
	void onDevicesModelChanged();
	void onEventLogModelChanged();

public slots:
	void onReadyDownloadIcons(brisa::upnp::controlpoint::Device *device);
	void deviceFoundDump(brisa::upnp::controlpoint::Device *device);
	void removeDevice(QString udn);
	void multicastEventReceived(QString variableName, QString newValue);
	void multicastEventRawReceived(OutArgument raw);
	void unicastEventReceived(brisa::upnp::controlpoint::EventProxy *subscription,
			QMap<QString, QString> eventVariables);

private slots:
    void testConnection();
    void hideSearching();

private:
	bool m_connected;
    bool m_searchIng;
    bool m_isEmptyList;
    bool m_controlPointRunning;
    QMap<int, QList<QString> > m_eventToDevice;

    DeviceList *m_devices;

    QTimer *m_timer;
	QTimer *m_searchingTimer;
	QVariantMap *m_currentDeviceItem;
	bb::cascades::GroupDataModel *m_eventLogModel;
	bb::cascades::ArrayDataModel *m_devicesModel;

	brisa::upnp::controlpoint::ControlPoint *m_controlPoint;
	brisa::upnp::controlpoint::Device *m_currentDev;

	void createDeviceItem();
	void createActions();
	void createMenus();
	void createToolBars();
	void setUpTableWidget();
	void addItem(QVariantMap *deviceItem);
};

#endif // VIEWCONTROLLER_H
