#include "myclasstest.h"

using namespace brisa;
using namespace upnp;
using namespace controlpoint;

MyClassTest::MyClassTest()
{
    qDebug() << "Iniciando a classe de testes";
}

void MyClassTest::startCP()
{
    qDebug() << "Iniciando o cpBCU";
    ControlPointBCU *controlPoint = new ControlPointBCU();
    controlPoint->start();
    controlPoint->discover();
    qDebug() << "cpBCU iniciado";
}

void MyClassTest::startDevice()
{

}

void MyClassTest::compress(QString source, QString destination)
{
    FolderCompressor * fc = new FolderCompressor();
    bool status = fc->compressFolder(source, destination);
    qDebug() << "compressed: " << status;
}

void MyClassTest::uncompress(QString source, QString destination)
{
    FolderCompressor * fc = new FolderCompressor();
    bool status2 = fc->decompressFolder(source, destination);
    qDebug() << "uncompressed: " << status2;
}

