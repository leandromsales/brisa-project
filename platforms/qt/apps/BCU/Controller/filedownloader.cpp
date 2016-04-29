#include <QDir>
#include "filedownloader.h"
FileDownloader::FileDownloader(QDir *dirFiles, QUrl url, QString filename, QObject *parent) :
    QObject(parent)
{
    m_filesDir = dirFiles;
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),this, SLOT (fileDownloaded(QNetworkReply*)));

    m_filename = filename;
    m_url = url;

    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();

    QString url = m_url.toString();
    QImage image = QImage::fromData(m_DownloadedData);
    QFile outFile;

    if(m_filesDir->dirName() != "files") m_filesDir->cd("files");
    if(!m_filesDir->cd(m_filename)) {
        m_filesDir->mkdir(m_filename);
        m_filesDir->cd(m_filename);
    }

    if (url.endsWith(".jpg", Qt::CaseInsensitive)) {
        outFile.setFileName(m_filesDir->absoluteFilePath(m_filename + ".jpg"));
        image.save(&outFile, "JPEG");
    } else if (url.endsWith(".png", Qt::CaseInsensitive)) {
        outFile.setFileName(m_filesDir->absoluteFilePath(m_filename + ".png"));
        image.save(&outFile, "PNG");
    } else if (url.endsWith(".compe", Qt::CaseInsensitive)) {
        outFile.setFileName(m_filesDir->absoluteFilePath(m_filename + ".compe")); // importante esse caminho estar certo
        outFile.open(QIODevice::WriteOnly);
        if (!outFile.isOpen())
            qDebug() << "BCU: wrong path to save downloaded files";
        outFile.write(m_DownloadedData);
    } else
        qDebug() << "BCU: invalid extension" << url;

    outFile.close();
    qDebug() << "BCU: get file" << outFile.fileName();

    emit ready(m_filesDir->absolutePath());
    m_filesDir->cdUp();
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}
