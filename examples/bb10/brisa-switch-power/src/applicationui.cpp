/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <QDebug>
#include <QVariantMap>
#include <QSharedPointer>

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/ArrayDataModel>

#include "upnp/Action"
#include "upnp/controlpoint/Device"
#include "upnp/controlpoint/Service"
#include "upnp/controlpoint/ControlPoint"

using namespace bb::cascades;
using namespace brisa::upnp;
using namespace brisa::upnp::controlpoint;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app) {
	m_dataModel = new ArrayDataModel();
	m_controlPoint = new ControlPoint();
	m_controlPoint->start();
	m_controlPoint->discover();
	bool isOk = connect(m_controlPoint, SIGNAL(deviceGone(QString)), this,
			SLOT(removeDevice(QString)));
	Q_ASSERT(isOk);
	isOk = connect(m_controlPoint, SIGNAL(deviceFound(brisa::upnp::controlpoint::Device*)), this,
			SLOT(deviceFoundDump(brisa::upnp::controlpoint::Device*)));
	Q_ASSERT(isOk);

	// prepare the localization
	m_pTranslator = new QTranslator(this);
	m_pLocaleHandler = new LocaleHandler(this);

	bool res = QObject::connect(m_pLocaleHandler,
			SIGNAL(systemLanguageChanged()), this,
			SLOT(onSystemLanguageChanged()));
	// This is only available in Debug builds
	Q_ASSERT(res);
	// Since the variable is not used in the app, this is added to avoid a
	// compiler warning
	Q_UNUSED(res);

	// initial load
	onSystemLanguageChanged();

	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
	qml->setContextProperty("_app", this);

	// Create root object for the UI
	AbstractPane *root = qml->createRootObject<AbstractPane>();

	// Set created root object as the application scene
	app->setScene(root);
}

void ApplicationUI::onSystemLanguageChanged() {
	QCoreApplication::instance()->removeTranslator(m_pTranslator);
	// Initiate, load and install the application translation files.
	QString locale_string = QLocale().name();
	QString file_name = QString("BRisaSwitchPower_%1").arg(locale_string);
	if (m_pTranslator->load(file_name, "app/native/qm")) {
		QCoreApplication::instance()->installTranslator(m_pTranslator);
	}
}

void ApplicationUI::deviceFoundDump(Device *device) {
	if (device->getAttribute(Device::Udn).compare("") == 0)
		return;
	// TODO Verificar se podemos usar esse metodo de verificacao de duplicidade
//	if (m_devices->getDeviceByUDN(
//			device->getAttribute(Device::Udn))) {
//		(*m_devices)[i] = device;
//		return;
//	}
	if (findDeviceByUDN(device->getAttribute(Device::udn)) == -1) {
		if (device->getAttribute(Device::DeviceType)
				== "urn:schemas-upnp-org:device:BinaryLight:1"
				|| device->getAttribute(Device::DeviceType)
						== "urn:schemas-upnp-org:device:DimmableLight:1") {
			QVariantMap map;
			map.insert("friendlyName",
					device->getAttribute(Device::FriendlyName));
			map.insert("udn", device->getAttribute(Device::udn));
			m_devicesList.append(QSharedPointer<Device>(device));
			m_dataModel->append(map);
		}
	}
}

void ApplicationUI::removeDevice(QString udn) {
	qDebug() << "ApplicationUI::removeDevice:" << udn;
	int index = findDeviceByUDN(udn);
	if (index != -1) {
		m_devicesList.removeAt(index);
		m_dataModel->removeAt(index);
	}
}

ArrayDataModel *ApplicationUI::dataModel() {
	return m_dataModel;
}

void ApplicationUI::getTarget(const QString& udn) {
	int index = findDeviceByUDN(udn);
	if (index != -1) {
		Device *device = m_devicesList.at(index).data();
		m_service = device->getServiceByType(
				"urn:schemas-upnp-org:service:SwitchPower:1");
		Action *action = m_service->getAction("GetTarget");
		connect(m_service, SIGNAL(requestFinished(OutArgument, QString)),
				this, SLOT(
						serviceCall(OutArgument, QString)));
		connect(m_service, SIGNAL(requestError(QString, QString)), this, SLOT(
				requestError(QString,QString)));
		InArgument parameters;
		m_service->call(action->getName(), parameters);
	}
}

void ApplicationUI::setTarget(const QString& udn, const bool& newState) {
	int index = findDeviceByUDN(udn);
	if (index != -1) {
		Device *device = m_devicesList.at(index).data();
		m_service = device->getServiceByType(
				"urn:schemas-upnp-org:service:SwitchPower:1");
		Action *action = m_service->getAction("SetTarget");
		bool isOk = connect(m_service,
				SIGNAL(requestFinished(OutArgument, QString)), this, SLOT(
						serviceCall(OutArgument, QString)));
		Q_ASSERT(isOk);
		isOk = connect(m_service, SIGNAL(requestError(QString, QString)), this,
				SLOT(
						requestError(QString,QString)));
		Q_ASSERT(isOk);
		InArgument parameters;
		parameters.insert("NewTargetValue", QString::number(newState));
		m_service->call(action->getName(), parameters);
	}
}

void ApplicationUI::serviceCall(OutArgument arguments, QString method) {
	disconnect(m_service, SIGNAL(requestFinished(OutArgument, QString)),
			this, SLOT(
					serviceCall(OutArgument, QString)));
	disconnect(m_service, SIGNAL(requestError(QString, QString)), this, SLOT(
			requestError(QString,QString)));
	if (method == "GetTarget")
		emit resultGetTargetChanged(arguments["RetTargetValue"].toInt());
}

void ApplicationUI::requestError(QString errorMessage, QString methodName) {
	disconnect(m_service, SIGNAL(requestFinished(OutArgument, QString)),
			this, SLOT(
					serviceCall(OutArgument, QString)));
	disconnect(m_service, SIGNAL(requestError(QString, QString)), this, SLOT(
			requestError(QString,QString)));
	qDebug() << errorMessage + " when calling " + methodName;
}

QVariantMap ApplicationUI::getDeviceInformations(
		const QString& udn) {
	QVariantMap map;
	int index = findDeviceByUDN(udn);
	if (index != -1) {
		Device *device = m_devicesList.at(index).data();
//		qDebug() << "ApplicationUI::getDeviceInformations"
//				<< device->getAttribute(Device::udn)
//				<< device->getAttribute(Device::major)
//				<< device->getAttribute(Device::minor)
//				<< device->getAttribute(Device::urlBase)
//				<< device->getAttribute(Device::deviceType)
//				<< device->getAttribute(Device::friendlyName)
//				<< device->getAttribute(Device::manufacturer)
//				<< device->getAttribute(Device::manufacturerUrl)
//				<< device->getAttribute(Device::modelDescription)
//				<< device->getAttribute(Device::modelName)
//				<< device->getAttribute(Device::modelUrl)
//				<< device->getAttribute(Device::serialNumber)
//				<< device->getAttribute(Device::upc)
//				<< device->getAttribute(Device::presentationUrl)
//				<< device->getAttribute(Device::fileAddress);
		map.insert("udn", device->getAttribute(Device::udn));
		map.insert("major", device->getAttribute(Device::major));
		map.insert("minor", device->getAttribute(Device::minor));
		map.insert("urlBase", device->getAttribute(Device::urlBase));
		map.insert("deviceType", device->getAttribute(Device::deviceType));
		map.insert("friendlyName", device->getAttribute(Device::friendlyName));
		map.insert("manufacturer", device->getAttribute(Device::manufacturer));
		map.insert("manufacturerUrl", device->getAttribute(Device::manufacturerUrl));
		map.insert("modelDescription", device->getAttribute(Device::modelDescription));
		map.insert("modelName", device->getAttribute(Device::modelName));
		map.insert("modelUrl", device->getAttribute(Device::modelUrl));
		map.insert("serialNumber", device->getAttribute(Device::serialNumber));
		map.insert("upc", device->getAttribute(Device::upc));
		map.insert("presentationUrl", device->getAttribute(Device::presentationUrl));
		map.insert("fileAddress", device->getAttribute(Device::fileAddress));
	}
	return map;
}

void ApplicationUI::bDebug(const QVariant& variant) {
	qDebug() << variant;
}

int ApplicationUI::findDeviceByUDN(const QString &udn) {
	int size = m_devicesList.size();
	for (int i = 0; i < size; ++i) {
		if (udn == m_devicesList.at(i)->getAttribute(Device::udn))
			return i;
	}
	return -1;
}
