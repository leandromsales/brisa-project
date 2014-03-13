/*
 * ActionController.cpp
 *
 *  Created on: 17/09/2013
 *      Author: daniel
 */

#include "ActionController.hpp"

using namespace brisa::upnp;
using namespace brisa::upnp::controlpoint;
using namespace bb::system;

ActionController::ActionController(QObject *parent) :
		QObject(parent) {
	m_currentService = 0;
	m_executableAction = ExecutableAction::getInstance(this);
	m_requestTimer = new QTimer(this);
	connect(m_requestTimer, SIGNAL(timeout()), this, SLOT(checkIfHaveResult()));
}

ActionController::~ActionController() {
	disconnect(m_requestTimer, SIGNAL(timeout()), this,
			SLOT(checkIfHaveResult()));
}

QVariantMap ActionController::getArguments() {
	QVariantMap arguments;
	BrisaAction *currentAction = m_executableAction->getAction();
	if (currentAction) {
		QVariantList inArgs;
		QVariantList outArgs;
		QList<BrisaArgument *> inArguments;
		QList<BrisaArgument *> outArguments;

		for (int i = 0; i < currentAction->getArgumentList().size(); i++) {
			if ((currentAction->getArgumentList()[i])->getAttribute(
					BrisaArgument::Direction).compare("in") == 0) {
				inArguments.append(currentAction->getArgumentList()[i]);
			} else {
				outArguments.append(currentAction->getArgumentList()[i]);
			}
		}

		if (inArguments.size() > 0) {
			for (int i = 0; i < inArguments.size(); ++i) {
				QVariantMap parameter;
				parameter["name"] = inArguments[i]->getAttribute(
						BrisaArgument::ArgumentName);
				parameter["related_state"] = inArguments[i]->getAttribute(
						BrisaArgument::RelatedStateVariable);
				inArgs.append(parameter);
			}
		}
		if (outArguments.size() > 0) {
			for (int i = 0; i < outArguments.size(); ++i) {
				QVariantMap parameter;
				parameter["name"] = outArguments[i]->getAttribute(
						BrisaArgument::ArgumentName);
				parameter["related_state"] = outArguments[i]->getAttribute(
						BrisaArgument::RelatedStateVariable);
				outArgs.append(parameter);
			}
		}

		arguments["in"] = inArgs;
		arguments["out"] = outArgs;
	}
	return arguments;
}

void ActionController::callMethod(QVariantMap arguments) {
	m_currentService = m_executableAction->getService();
	connect(m_currentService,
			SIGNAL(requestFinished(BrisaOutArgument, QString)), this, SLOT(
					serviceCall(BrisaOutArgument, QString)));
	connect(m_currentService, SIGNAL(requestError(QString, QString)), this,
			SLOT(
					requestError(QString,QString)));

	BrisaInArgument parameters;
	foreach (QString argument, arguments.keys())
	{
		QString paramName = argument;
		paramName.replace(":", "");
		QString value = arguments[argument].toString();
		parameters[paramName] = value;
	}
	m_requestTimer->start(5000);
	m_currentService->call(m_executableAction->getAction()->getName(), parameters);
}

bool ActionController::haveResult() {
	return false;
}

void ActionController::serviceCall(BrisaOutArgument arguments, QString method) {
	disconnect(m_currentService,
			SIGNAL(requestFinished(BrisaOutArgument, QString)), this, SLOT(
					serviceCall(BrisaOutArgument, QString)));
	disconnect(m_currentService, SIGNAL(requestError(QString, QString)), this,
			SLOT(
					requestError(QString,QString)));
	QString returnMessage = "";
	QMapIterator<QString, QString> it(arguments);
	while (it.hasNext()) {
		it.next();
		returnMessage.append(it.key() + " = " + it.value() + "\n");
	}
	qDebug()
			<< "Calling method: " + method + "\n\n" + "Returned: \n"
					+ returnMessage;
	m_result.clear();
	m_result.append(
			"Calling method: " + method + "\n\n" + "Returned: \n"
					+ returnMessage);
	emit this->onResultChanged();
	m_requestTimer->stop();
	checkIfHaveResult();
}

void ActionController::requestError(QString errorMessage, QString methodName) {
	disconnect(m_currentService,
			SIGNAL(requestFinished(BrisaOutArgument, QString)), this, SLOT(
					serviceCall(BrisaOutArgument, QString)));
	disconnect(m_currentService, SIGNAL(requestError(QString, QString)), this,
			SLOT(
					requestError(QString,QString)));
	qDebug() << errorMessage + " when calling " + methodName;
	m_result.clear();
	emit this->onResultChanged();
	m_requestTimer->stop();
	checkIfHaveResult();
}

void ActionController::copyText(QByteArray text) {
	Clipboard clipboard;
	clipboard.clear();
	clipboard.insert("text/plain", text);
	SystemToast *toast = new SystemToast(this);
	// feel free to change the message below to whatever you need.
	toast->setBody("The result was copied to clipboard.");
	toast->show();
}

QString ActionController::getResult() {
	return m_result;
}

void ActionController::clearResult() {
	m_result.clear();
	emit this->onResultChanged();
}

void ActionController::checkIfHaveResult() {
	emit onHaveResult();
}
