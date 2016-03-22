#ifndef BCATCPSERVER_H
#define BCATCPSERVER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkInterface>
#include <QNetworkReply>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDebug>
#include <QFile>

class BCATcpServer : public QObject
{
    Q_OBJECT
public:
    explicit BCATcpServer(QString path, quint16 port, QObject *parent = 0);
    ~BCATcpServer();
    QString ip() const;
private slots:
    void onNewConnection();
private:
    QTcpServer *m_server;
    QString m_path;
    QString m_ip;
};

#endif // BCATCPSERVER_H
