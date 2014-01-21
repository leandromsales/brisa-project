/*
 * BinaryLight.h
 *
 *  Created on: 22/09/2013
 *      Author: wendell
 */

#ifndef BINARYLIGHT_H_
#define BINARYLIGHT_H_

#define DEVICE_TYPE              "urn:schemas-upnp-org:device:BinaryLight:1"
#define DEVICE_FRIENDLY_NAME     "Binary Light Device"
#define DEVICE_MANUFACTURER      "Brisa Team. Embedded Laboratory and INdT Brazil"
#define DEVICE_MANUFACTURER_URL  "https://garage.maemo.org/projects/brisa"
#define DEVICE_MODEL_DESCRIPTION "An UPnP Binary Light Device"
#define DEVICE_MODEL_NAME        "Binary Light Device"
#define DEVICE_MODEL_NUMBER      "1.0"
#define DEVICE_MODEL_URL         "https://garage.maemo.org/projects/brisa"
#define DEVICE_SERIAL_NUMBER     "1.0"
#define DEVICE_UDN               "uuid:367a4fa7-cf68-4cc8-844d-0af4c615cecb"

#include "applicationui.hpp"
#include "SwitchPower.hpp"
#include <upnp/device/brisadevice.h>
#include <upnp/brisautils.h>

class ApplicationUI;

using namespace Brisa;

class BinaryLight: public QObject {
	Q_OBJECT
public:
	BinaryLight(ApplicationUI *ui);

	void start();
	void byebye();

public slots:
	void uiStateChanged(bool);

private:
	ApplicationUI *m_ui;
    Brisa::BrisaDevice *m_binaryLight;
    BrisaStateVariable *m_status;
    BrisaStateVariable *m_target;
};

#endif /* BINARYLIGHT_H_ */
