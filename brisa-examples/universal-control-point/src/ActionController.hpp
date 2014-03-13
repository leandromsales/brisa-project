/*
 * ActionController.hpp
 *
 *  Created on: 17/09/2013
 *      Author: daniel
 */

#ifndef ACTIONCONTROLLER_H_
#define ACTIONCONTROLLER_H_

#include <QList>
#include <QDebug>
#include <QTimer>
#include <QString>
#include <QObject>
#include <QVariant>
#include <QByteArray>
#include <QVariantMap>
#include <QVariantList>

#include <bb/system/Clipboard>
#include <bb/system/SystemToast>

#include "upnp/brisaaction.h"
#include "upnp/controlpoint/brisacontrolpointdevice.h"
#include "upnp/controlpoint/brisacontrolpointservice.h"

#include "model/ExecutableAction.hpp"

class ActionController : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString getResult READ getResult NOTIFY onResultChanged)
	Q_PROPERTY(bool haveResult READ haveResult NOTIFY onHaveResult)
public:
	ActionController(QObject *parent = 0);
	virtual ~ActionController();
	Q_INVOKABLE QVariantMap getArguments();
	Q_INVOKABLE void callMethod(QVariantMap argumentsList);
	Q_INVOKABLE void copyText(QByteArray text);
	Q_INVOKABLE QString getResult();
	Q_INVOKABLE void clearResult();
	Q_INVOKABLE bool haveResult();

signals:
	void onHaveResult();
	void onResultChanged();

public slots:
    void serviceCall(BrisaOutArgument, QString);
    void requestError(QString errorMessage, QString methodName);
	void checkIfHaveResult();

private:
	QString m_result;
	QTimer *m_requestTimer;
	ExecutableAction *m_executableAction;
	brisa::upnp::controlpoint::BrisaControlPointService *m_currentService;
};

#endif /* ACTIONCONTROLLER_H_ */
