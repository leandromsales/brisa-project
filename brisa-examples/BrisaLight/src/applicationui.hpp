#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QList>
#include <QTimer>
#include <QString>
#include <QObject>
#include <QSettings>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>

#include <bb/cascades/Color>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Application>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>

#include <upnp/brisastatevariable.h>

#include "BinaryLight.h"
#include "activeframeqml.h"

namespace bb {
namespace cascades {
class Application;
class LocaleHandler;
}
}

class QTranslator;

/*!
 * @brief Application object
 *
 *
 */

using namespace bb::cascades;
using namespace bb::system;

using namespace brisa::upnp;

class BinaryLight;

class ApplicationUI: public QObject {
	Q_OBJECT
	Q_PROPERTY(QString initImage READ initImage FINAL)
	Q_PROPERTY(bool activeLamp READ activeLamp WRITE setActiveLamp NOTIFY onActiveLamp FINAL)
	Q_PROPERTY(bool isConnected READ isConnected NOTIFY onConnected FINAL)
	Q_PROPERTY(bool lamp READ lamp WRITE setLamp NOTIFY onLamp FINAL)
	Q_PROPERTY(bool flash READ flash WRITE setFlash NOTIFY onFlash FINAL)
	Q_PROPERTY(bool lampAndFlash READ lampAndFlash WRITE setLampAndFlash NOTIFY onLampAndFlash FINAL)
	Q_PROPERTY(bool screen READ screen WRITE setScreen NOTIFY onScreen FINAL)

public:
	ApplicationUI(bb::cascades::Application *app);
	virtual ~ApplicationUI();
	Q_INVOKABLE void callBBMCard();
	Q_INVOKABLE void callSettingsCard(QString uri);
	Q_INVOKABLE bool isConnected();
	Q_INVOKABLE bool lamp();
	Q_INVOKABLE void setLamp(bool lamp);
	Q_INVOKABLE bool lampAndFlash();
	Q_INVOKABLE void setLampAndFlash(bool lampAndFlash);
	Q_INVOKABLE bool flash();
	Q_INVOKABLE void setFlash(bool flash);
	Q_INVOKABLE bool screen();
	Q_INVOKABLE void setScreen(bool screen);
	Q_INVOKABLE bool activeLamp();
	Q_INVOKABLE void setActiveLamp(bool activeLamp);
	Q_INVOKABLE QString initImage();
	Q_INVOKABLE void updateSettings();

signals:
	void onConnected();
	void onLamp();
	void onFlash();
	void onLampAndFlash();
	void onScreen();
	void onActiveLamp();
	void onSwitchStatus(bool);

public slots:
	void stateChanged(BrisaStateVariable *);

private slots:
	void testConnection();
	void onSystemLanguageChanged();

private:
	QSettings *m_settings;
	QTranslator* m_pTranslator;
	bb::cascades::LocaleHandler* m_pLocaleHandler;

	bool m_activeLamp;
	bool m_lamp;
	bool m_flash;
	bool m_lampAndFlash;
	bool m_screen;
	bool m_connected;
	bool m_binaryLightRunning;
	QTimer *m_timer;
	QmlDocument *m_qml;
	AbstractPane *m_root;

	BinaryLight *m_binaryLight;

	void startBinaryLight();
	void byebyeBinaryLight();
};

#endif /* ApplicationUI_HPP_ */
