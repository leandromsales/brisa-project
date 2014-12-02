/*
 * Universidade Federal de Campina Grande
 * Centro de Engenharia Elétrica e Informática
 * Laboratório de Sistemas Embarcados e Computação Pervasiva
 * BRisa Project / BRisa-Qt - http://brisa.garage.maemo.org
 * Filename: brisassdpclient.cpp
 * Created:
 * Description: This File implements the BrisaSSDPClient class.
 * Authors: Name <email> @since 2009
 *
 *
 * Copyright (C) <2009> <Embbeded Systems and Pervasive Computing Laboratory>
 *
 * BRisa-Qt is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ssdpclient.h"

namespace brisa {
using namespace upnp::controlpoint;
namespace shared {
namespace ssdp {

SSDPClient::SSDPClient(QObject *parent) :
	QObject(parent),
    running(false)
{
}

SSDPClient::~SSDPClient() {
    if (isRunning())
        stop();

    delete this->udpListener;
}

void SSDPClient::start() {
    if (!isRunning()) {
        this->udpListener = new UdpListener("239.255.255.250", 1900,
                                                 "SSDPClient");
        connect(this->udpListener, SIGNAL(readyRead()), this, SLOT(datagramReceived()));
        this->udpListener->start();
        running = true;
    } else {
        qDebug() << "Brisa SSDP Client: Already running!";
    }
}

void SSDPClient::stop() {
    if (isRunning()) {
        this->udpListener->disconnectFromHost();;
        running = false;
    } else {
        qDebug() << "Brisa SSDP Client: Already stopped!";
    }
}

bool SSDPClient::isRunning() const {
    return running;
}

void SSDPClient::datagramReceived() {
    while (this->udpListener->hasPendingDatagrams()) {
        QByteArray *datagram = new QByteArray();

        datagram->resize(udpListener->pendingDatagramSize());
        udpListener->readDatagram(datagram->data(), datagram->size());

        QString temp(datagram->data());
        emit messageReceived(temp);

        notifyReceived(getMapFromMessage(temp));

        delete datagram;
    }

}

void SSDPClient::notifyReceived(const QMap<QString, QString> &message) {
    qDebug() << "NOTIFICAÇÃO RECEBIDA";
    if (!message.contains("nts")) {
        qDebug() << "!NTS";
	        return;
    }

        if (message.value("nts") == "ssdp:alive") {
            emit newDeviceEvent(message.value("usn"),
                                message.value("location"), message.value("nt"),
                                message.value("ext"), message.value("server"),
                                message.value("cacheControl"));
	        qDebug() << "Brisa SSDP Client: Received alive from " <<
                    message.value("usn") << "";

        } else if (message.value("nts") == "ssdp:byebye") {
            emit removedDeviceEvent(message.value("usn"));
	        qDebug() << "Brisa SSDP Client: Received byebye from " <<
                    message.value("usn") << "";

	    } else {
	        qDebug() << "Brisa SSDP Client: Received unknown subtype: " <<
                    message.value("nts") << "";
	    }
}

QMap<QString, QString> SSDPClient::getMapFromMessage(QString message) {
	QStringList messageLines = message.split("\r\n");
	QMap<QString, QString> response;

	foreach(QString line, messageLines) {
		if((line = line.trimmed())!= "") {
			int divpos = line.indexOf(':');
			//TODO Definir se vai sempre fazer toLower aqui
			QString key = line.left(divpos).toLower();
			QString value = line.right(line.size() - divpos).remove(0, 1).trimmed();
			response[key] = value;
		} else {
			break;
		}
	}

	return response;
}

}
}  // namespace shared
}
