#include "filemanager.h"


////////////////////////////////////////
QString FileManager::readFile(QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream inFromFile(&file);

    QString textFile;
    while(!inFromFile.atEnd()) {
        QString line = inFromFile.readLine();
        textFile.append(line);
    }
    file.close();
    return textFile;
}

void FileManager::writeFile(QString &fileName, QString text)
{
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream outToFile(&file);
    outToFile << text;
    file.close();
}

