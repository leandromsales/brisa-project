#include "ssdp.h"

namespace brisa {
namespace shared {
namespace ssdp {

SSDP::SSDP(int interval, QObject *parent) :
		QObject(parent) {

	this->interval = interval;
	this->discoverTimer = new QTimer(this);
	this->udpSocket = new QUdpSocket(this);
	this->init();
}

SSDP::~SSDP() {
	delete this->discoverTimer;
	delete this->udpSocket;
}

void SSDP::init() {

	qDebug() << "SSDP: initing...";
	QHostAddress groupAddress = QHostAddress(SSDP_ADDR);

	// Create a new UDP socket and bind it against port 1900
	if (!udpSocket->bind(groupAddress, SSDP_PORT,
			QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
		qDebug() << "cannot bind multicast socket in" << SSDP_ADDR;
	}

	// Tell the UDP socket which multicast group it should join
	Q_ASSERT_X(udpSocket->joinMulticastGroup(groupAddress), "SSDP::init()", "Could not join multicast group.");

	/**
	 * Create signal/slot connection to invoke datagramReceived() whenever
	 * a new multicast datagram is received by the socket.
	 */
	Q_ASSERT(connect(udpSocket, SIGNAL(readyRead()), this, SLOT(datagramReceived())));
}

void SSDP::start() {
	this->discoverTimer->start(interval * 1000);
	this->doubleDiscover();
	emit setupEvent("SSDP: started");
}

void SSDP::stop() {
	udpSocket->disconnectFromHost();
	this->discoverTimer->stop();
}

void SSDP::discover(const QString &type, const QString &mx,
		const QString &userAgent) {
	QString discoverMessage = SSDP_DISCOVERY_REQUEST.arg(type, mx, userAgent);

	udpSocket->moveToThread(this->thread());
	udpSocket->writeDatagram(discoverMessage.toUtf8(), QHostAddress(SSDP_ADDR),
			SSDP_PORT);
}

void SSDP::doubleDiscover(const QString &type, const QString &mx,
		const QString &userAgent) {
	discover(type, mx, userAgent);
	discover(type, mx, userAgent);
}

void SSDP::datagramReceived() {

	QUdpSocket *socket = static_cast<QUdpSocket *>(QObject::sender());

	while (socket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(socket->pendingDatagramSize());
		QHostAddress peerAddress;
		quint16 peerPort;
		socket->readDatagram(datagram.data(), datagram.size(), &peerAddress,
				&peerPort);
		QString message = QString(datagram);

		if (!message.startsWith("M-SEARCH", Qt::CaseInsensitive)) {
			qDebug() << "Message received";
			emit messageReceived(message);
		}
	}
}

/**
 * Parse SSDP message
 */
QMap<QString, QString> SSDP::getMapFromMessage(QString message) {
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
