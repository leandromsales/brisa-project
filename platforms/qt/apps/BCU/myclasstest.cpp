#include "myclasstest.h"

using namespace brisa;
using namespace upnp;
using namespace controlpoint;

MyClassTest::MyClassTest()
{
}

void MyClassTest::startCP()
{
    qDebug() << "BCU: Starting";
    ControlPointBCU *controlPoint = new ControlPointBCU();
    controlPoint->start();
    controlPoint->discover();
    qDebug() << "BCU: Started";
}

void MyClassTest::compress(QString source, QString destination)
{
    FolderCompressor * fc = new FolderCompressor();
    bool status = fc->compressFolder(source, destination);
    qDebug() << "BCU: Compressed" << status;
}

void MyClassTest::uncompress(QString source, QString destination)
{
    FolderCompressor * fc = new FolderCompressor();
    bool status = fc->decompressFolder(source, destination);
    qDebug() << "BCU: Uncompressed" << status;
}

