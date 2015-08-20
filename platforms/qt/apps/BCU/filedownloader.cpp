#include "filedownloader.h"

FileDownloader::FileDownloader()
{
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),this, SLOT (fileDownloaded(QNetworkReply*)));
}

FileDownloader::FileDownloader(QUrl imageUrl) : QObject(0)
{
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),this, SLOT (fileDownloaded(QNetworkReply*)));

    setURL(imageUrl);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}

void FileDownloader::setURL(QUrl url)
{
    QNetworkRequest request(url);
    m_WebCtrl.get(request);
}
