#include "applicationui.hpp"

using namespace bb::cascades;
using namespace brisa::upnp;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
		QObject(app) {

	ApplicationInfo::registerQmlTypes();

	m_activeLamp = false;
	emit onActiveLamp();

	m_settings = new QSettings("COMPE", "BRisa Light", this);

	if (m_settings->contains("lamp") && m_settings->contains("flash")
			&& m_settings->contains("lampAndFlash")
			&& m_settings->contains("screen")
			&& m_settings->contains("initImage")) {
		m_lamp = m_settings->value("lamp").toBool();
		m_flash = m_settings->value("flash").toBool();
		m_lampAndFlash = m_settings->value("lampAndFlash").toBool();
		m_screen = m_settings->value("screen").toBool();
	} else {
		m_lamp = false;
		m_flash = false;
		m_lampAndFlash = true;
		m_screen = false;
		updateSettings();
	}
	m_connected = false;
	emit onConnected();

	// prepare the localization
	m_pTranslator = new QTranslator(this);
	m_pLocaleHandler = new LocaleHandler(this);
	if (!QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()),
			this, SLOT(onSystemLanguageChanged()))) {
		// This is an abnormal situation! Something went wrong!
		// Add own code to recover here
		qWarning() << "Recovering from a failed connect()";
	}
	// initial load
	onSystemLanguageChanged();

	// Create scene document from main.qml asset, the parent is set
	// to ensure the document gets destroyed properly at shut down.
	m_qml = QmlDocument::create("asset:///main.qml").parent(this);

	// Create root object for the UI
	m_root = m_qml->createRootObject<AbstractPane>();

	m_qml->setContextProperty("_app", this);

	// Create a SceneCover and set the application cover
	ActiveFrameQML *activeFrame = new ActiveFrameQML();
	Application::instance()->setCover(activeFrame);

	// Set created root object as the application scene
	app->setScene(m_root);

	m_binaryLight = new BinaryLight(this);

	m_timer = new QTimer(this);
	connect(m_timer, SIGNAL(timeout()), this, SLOT(testConnection()));
	emit testConnection();
}

ApplicationUI::~ApplicationUI() {
	qDebug() << "Destrutor";
	m_timer->stop();
	m_binaryLight->byebye();
}

void ApplicationUI::onSystemLanguageChanged() {
	QCoreApplication::instance()->removeTranslator(m_pTranslator);
	// Initiate, load and install the application translation files.
	QString locale_string = QLocale().name();
	QString file_name = QString("WANChat_%1").arg(locale_string);
	if (m_pTranslator->load(file_name, "app/native/qm")) {
		QCoreApplication::instance()->installTranslator(m_pTranslator);
	}
}

void ApplicationUI::callAppWorldVendorCard() {
	InvokeManager* invokeManager = new InvokeManager();
	InvokeRequest cardRequest;
	cardRequest.setTarget("sys.appworld"); // Aqui define o target~;
	cardRequest.setAction("bb.action.OPEN");
	cardRequest.setUri("appworld://vendor/55406"); // id vendor COMPELab: 55406
	InvokeTargetReply* reply = invokeManager->invoke(cardRequest);
	reply->setParent(this);
}

void ApplicationUI::postAReview() {
	InvokeManager* invokeManager = new InvokeManager();
	InvokeRequest cardRequest;
	cardRequest.setTarget("sys.appworld"); // Aqui define o target~;
	cardRequest.setAction("bb.action.OPEN");
	cardRequest.setUri("appworld://content/37260887"); // id FeetFit: 37260887
	InvokeTargetReply* reply = invokeManager->invoke(cardRequest);
	reply->setParent(this);
}

// Nao esta mais sendo usado esse metodo
void ApplicationUI::callBBMCard() {
	InvokeManager* invokeManager = new InvokeManager();
	InvokeRequest cardRequest;
	cardRequest.setTarget("sys.bbm.sharehandler"); // Aqui define o target~;
	cardRequest.setAction("bb.action.SHARE");
	cardRequest.setMimeType("text/plain");
	cardRequest.setData("Use BRisa Binary Light App for BlackBerry 10! Check BRisa Binary Light in the BlackBerry World! Find in http://appworld.blackberry.com/webstore/content/37260887");
	InvokeTargetReply* reply = invokeManager->invoke(cardRequest);
	reply->setParent(this);
}

void ApplicationUI::callSettingsCard(QString uri) {
	InvokeManager* invokeManager = new InvokeManager();
	InvokeRequest cardRequest;
	cardRequest.setTarget("sys.settings.target"); // Aqui define o target~;
	cardRequest.setAction("bb.action.OPEN");
	cardRequest.setMimeType("settings/view");
	cardRequest.setUri(uri);
	InvokeTargetReply* reply = invokeManager->invoke(cardRequest);
	reply->setParent(this);
}

void ApplicationUI::updateSettings() {
	m_settings->setValue("lamp", m_lamp);
	m_settings->setValue("flash", m_flash);
	m_settings->setValue("lampAndFlash", m_lampAndFlash);
	m_settings->setValue("screen", m_screen);
	if (m_lamp || m_lampAndFlash)
		m_settings->setValue("initImage", "asset:///images/offlight.png");
	else
		m_settings->setValue("initImage", "");
	m_settings->sync();
}

void ApplicationUI::stateChanged(StateVariable *var) {
	m_activeLamp = var->getValue().toBool();
	emit onActiveLamp();
	qDebug() << "State changed: " << var->getValue().toBool();
}

bool ApplicationUI::isConnected() {
	return m_connected;
}

bool ApplicationUI::activeLamp() {
	return m_activeLamp;
}

void ApplicationUI::setActiveLamp(bool activeLamp) {
	m_activeLamp = activeLamp;
	m_binaryLight->uiStateChanged(m_activeLamp);
	emit onActiveLamp();
}

bool ApplicationUI::lamp() {
	return m_lamp;
}

void ApplicationUI::setLamp(bool lamp) {
	m_lamp = lamp;
	emit onLamp();
}

bool ApplicationUI::flash() {
	return m_flash;
}

void ApplicationUI::setFlash(bool flash) {
	m_flash = flash;
	emit onFlash();
}

bool ApplicationUI::screen() {
	return m_screen;
}

void ApplicationUI::setScreen(bool screen) {
	m_screen = screen;
	emit onScreen();
}

bool ApplicationUI::lampAndFlash() {
	return m_lampAndFlash;
}

void ApplicationUI::setLampAndFlash(bool lampAndFlash) {
	m_lampAndFlash = lampAndFlash;
	emit onLampAndFlash();
}

void ApplicationUI::startBinaryLight() {
	m_binaryLight->start();
	m_binaryLightRunning = true;
}

QString ApplicationUI::initImage() {
	return m_settings->value("initImage").toString();
}

void ApplicationUI::byebyeBinaryLight() {
	m_binaryLight->byebye();
	m_binaryLightRunning = false;
}

void ApplicationUI::testConnection() {
	QNetworkConfigurationManager mgr;
	QList<QNetworkConfiguration> activeConfigs = mgr.allConfigurations(
			QNetworkConfiguration::Active);

	m_connected = (activeConfigs.count() > 0) ? mgr.isOnline() : false;

	if (m_connected) {
		if (!m_binaryLightRunning)
			startBinaryLight();
		m_binaryLight->uiStateChanged(m_activeLamp);
	} else {
		if (m_binaryLightRunning)
			byebyeBinaryLight();
	}

	emit onConnected();

	m_timer->start(5000);
}
