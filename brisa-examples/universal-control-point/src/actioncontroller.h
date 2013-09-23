/*
 * actioncontroller.h
 *
 *  Created on: 17/09/2013
 *      Author: daniel
 */

#ifndef ACTIONCONTROLLER_H_
#define ACTIONCONTROLLER_H_

#include <QObject>
#include <QDebug>
#include <QList>
#include <QString>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <bb/cascades/ArrayDataModel>

#include <bb/cascades/Label>
#include <bb/cascades/Container>
#include <bb/cascades/TextField>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>

#include "upnp/brisaaction.h"
#include "upnp/controlpoint/brisacontrolpointservice.h"

#include "devicelist.h"

using namespace Brisa;
using namespace bb::cascades;

class ActionController : public QObject {
	Q_OBJECT
public:
	ActionController();
	virtual ~ActionController();
	Q_INVOKABLE void setDataModel(QVariantMap service);
	Q_INVOKABLE ArrayDataModel* getDataModel();
	Q_INVOKABLE QString getServiceType();
	Q_INVOKABLE void setCurrentAction(QString action);
	Q_INVOKABLE QVariantMap getArgumentsMap();
private:
	QString serviceType;
	DeviceList *deviceList;
	BrisaAction *currentAction;
	ArrayDataModel *actionsDataModel;
};

#endif /* ACTIONCONTROLLER_H_ */
