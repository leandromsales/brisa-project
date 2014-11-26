#include "shared/webserver/webserver.h"
#include "network/brisanetwork.h"

using namespace brisa;
using namespace network;
using namespace brisa::shared::webserver;

int main(int argc, char *argv[])
{
    // QString ipAddress = "172.20.8.208";
    QString ipAddress = "127.0.0.1";
    quint16 port = getPort();
    qDebug() << "Address " << ipAddress + ":" + QString::number(port);

    Webserver *webserver = new Webserver(QHostAddress(ipAddress), port);
    // webserver->addService ("/teste", new WebService());
    webserver->start ();
    sleep(15);
}


