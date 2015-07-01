#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QFile>
#include <QString>
#include <QTextStream>

class FileManager
{
public:
    static QString readFile(QString &fileName);
    static void writeFile(QString &fileName, QString text);
};

#endif // FILEMANAGER_H
