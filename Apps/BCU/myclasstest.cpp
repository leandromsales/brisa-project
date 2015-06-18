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

