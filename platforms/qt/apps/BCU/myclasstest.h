#ifndef MYCLASSTEST_H
#define MYCLASSTEST_H

#include "controlpointbcu.h"
#include "FolderCompressor.h"

class MyClassTest
{
public:
    MyClassTest();
    void startCP();
    void compress(QString source, QString destination);
    void uncompress(QString source, QString destination);
};

#endif // MYCLASSTEST_H
