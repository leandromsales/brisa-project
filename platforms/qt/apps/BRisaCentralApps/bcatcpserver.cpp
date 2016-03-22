#include "bcatcpserver.h"

BCATcpServer::BCATcpServer(QString path, quint16 port, QObject *parent) : QObject(parent)
{
    m_path = path;
    m_server = new QTcpServer(this);
    if(!m_server->listen(QHostAddress::AnyIPv4,port)) qFatal("Unable to start the server: %1.");
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    qDebug() <<
                "The server is running on\n\nIP:" <<
                ipAddress << "\nport: "<<
                m_server->serverPort();
    connect(m_server,SIGNAL(newConnection()),
            this,SLOT(onNewConnection()));
    m_ip = ipAddress;
}

BCATcpServer::~BCATcpServer()
{
    qDebug() << "SERVER HAS BEEN DELETED!";
}


void BCATcpServer::onNewConnection()
{
    while(m_server->hasPendingConnections()) {
        QTcpSocket *socket = m_server->nextPendingConnection();
        qDebug() << "NEW CONNECTION ESTABLISHED!";
        QFile file(m_path);
        if(!file.exists()) { qDebug() << "FILE DOESNT EXISTS"; return; }
        if(!file.open(QIODevice::ReadOnly)) qFatal("FILE COULDNT OPEN!");
        socket->write(file.readAll());
        socket->flush();
        socket->close();
        file.close();
    }
    m_server->close();
    this->deleteLater();
}

QString BCATcpServer::ip() const
{
    return m_ip;
}

